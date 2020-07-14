# Parsing a Json object - part 1: Intro

Printing a json\_object is rather boring and mostly useful for debugging purposes. Naturally one would expect a JSON library to include functions to make extracting the needed data from the JSON file as easy and simple as possible.

So let's examine _*contact.json*_ again:

```
{
  "firstName": "John",
  "lastName": "Smith",
  "isAlive": true,
  "age": 27,
  "address": {
    "streetAddress": "21 2nd Street",
    "city": "New York",
    "state": "NY",
    "postalCode": "10021-3100"
  },
  "phoneNumbers": [
    {
      "type": "home",
      "number": "212 555-1234"
    },
    {
      "type": "office",
      "number": "646 555-4567"
    }
  ],
  "children": [],
  "spouse": null
}
```
Suppose we wish to read this JSON file and print only the first and last name. Create the file below, _*json-parse00.c*_ and compile _*gcc json-parse00.c -ljson-c -o json-parse00*_:

```
#include <stdio.h>
#include <json-c/json.h>

int
main(void)
{
   json_object *root = json_object_from_file("contact.json");
   if (!root)
      return 1;

   json_object *first_name = json_object_object_get(root, "firstName");
   json_object *last_name = json_object_object_get(root, "lastName");
   printf("%s, %s\n", json_object_get_string(last_name), json_object_get_string(first_name));

   json_object_put(root);
   return 0;
}
```

The expected output from execution:

```
$ ./json-parse00
Smith, John

```

Now depending upon what json-c version you have installed when compiling this file you may get depreciated function warnings:

```
$ gcc json-parse00.c -ljson-c -o json-parse00
json-parse00.c: In function ‘main’:
json-parse00.c:13:4: warning: ‘json_object_object_get’ is deprecated [-Wdeprecated-declarations]
    json_object *first_name = json_object_object_get(root,  "firstName");
    ^~~~~~~~~~~
In file included from /usr/include/json-c/linkhash.h:16:0,
                 from /usr/include/json-c/json.h:22,
                 from json-parse00.c:4:
/usr/include/json-c/json_object.h:290:1: note: declared here
 THIS_FUNCTION_IS_DEPRECATED(extern struct json_object* json_object_object_get(struct json_object* obj,

```

I will address this issue in a bit, but first let's examine the two new functions used in the code above.

- json_object\* json\_object\_object\_get(json_object \* obj, const char \*key)
- const char\* json_object_get_string(json_object \*obj)


## json_object_object_get
The first function, _*json\_object\_object\_get(obj, key)*_, simply returns the _*json_object*_ associated with the given key. There is no need to dereference the returned _*json_object*_ with a _*json_object_put*_ as no reference counts are changed. According to the documentation, "Ownership of the returned value is retained by obj. " This means that if the owning object is freed (by a  _*json_object_put(obj)*_ function call then any usage of the _*json_object*_  returned by
 _*json_object_object_get(obj, key)*_ will return garbage as this pointer is no longer valid.

In our example, we have

```
json_object *first_name = json_object_object_get(root,  "firstName");
```

If we wish to keep the _*json_object \*first_name*_  *AFTER* executing _*json_object_put(root)*_, we must use the function _*json_object_get(first_name)* *BEFORE* executing the _*json_object_put(root)*_ command!! If we gain ownership of _*json_object \*first_name*_, we must latter dereference it with a _*json_object_put(first_name)*_ statement.

This point is very important. To see this play around with the code some by moving the line _*json_object_put(root);*_ around in the source and then again after adding some _*json_object_get*_ function calls on the appropriate objects. Each time not only execute the binary file but also run the code through  [_*Valgrind*_](https://www.valgrind.org/).

Hopefully, all this is clear by now. So let's examine what the function _*json_object_object_get(obj, key)*_ actually does. In our case the line of code:

```
json_object *first_name = json_object_object_get(root,  "firstName");
```

looks through the _*json_object*_ root, which is the internal memory representation of our file contact.json for an attribute called _*firstName*_ and returns the _*json_object*_ representing the value of this attribute: "John".

You can see this if you insert the statement below into the code:

```
 printf("%s\n", json_object_to_json_string_ext(first_name, JSON_C_TO_STRING_PRETTY));
```

Why does it not simply return the string itself with no quotes? Because remember a _*json_object*_  is not necessarily always a string. It can be a number, a boolean, an array of _*json_objects*_, or even another _*json_object*_ . In general, we don't know unless we know the _*semantics*_ of the JSON data being parsed in advance or have some way of testing the _*type*_ of the _*json_object*_ . (More on JSON types latter).

!["JSON state diagram for values"](https://github.com/rbtylee/tutorial-jsonc/blob/master/Images/value.png)

What about error handling you might ask? What happens if the function _*json_object_object_get(obj, key)*_ is unable to find the value associate with the key in the _*json_object*_? In this case, NULL is returned. Again play with the code to see the results of such an invalid search. In production code, we would include some error handling code.

## json\_object\_get\_string

It should be intuitively clear what the function _*json\_object\_get_string(obj)*_ does. It simply returns the string value (const char*) of the _*json_\object*_. According to the docs:
>If the passed object is not of type json\_type_string then the JSON representation of the object is returned.

Hence, it is safe to call this function with a NULL object, in which case (null) is returned.

It is important to note that the returned "string memory is managed by the json\_object and will be freed when the reference count of the json\_object drops to zero." Therefore, if one wishes to 'keep' the value of this string around, one has to store it in a char array. For example:

```
#include <stdio.h>
#include <json-c/json.h>

int
main(void)
{
   char fname[255], lname[255];
   json_object *root = json_object_from_file("contact.json");
   if (!root)
       return 1;

   json_object *first_name = json_object_object_get(root,  "firstName");
   json_object *last_name = json_object_object_get(root,  "lastName");
   // Store the string values of these json_objects in our char arrays
   snprintf(fname, 255, "%s", json_object_get_string(first_name));
   snprintf(lname, 255, "%s", json_object_get_string(last_name));
   // Lose ownership of our json_objects first_name and last_name
   json_object_put(root);
   // Verify our char arrays still have the proper values
   printf("%s, %s\n", fname, lname);

   return 0;
}

```

Store this as _*json-parse01.c*_ and compile as usual (_*gcc json-parse01.c -ljson-c -o json-parse01*_)
Verify this works as expected and does not leak memory using Valgrind.

## Problems

1. Rewrite json-parse00.c with proper error checks on the returned values before the print statements. In other words, if the key is not found do not print (null) for either the first name and last name.

- Example json-parse01.c is perhaps not ideal, as suppose our JSON data has a first or last name longer than 255 characters. In which case, the printed or stored value would be truncated, while this perhaps seems unlikely it is not wise to assume anything about a person's name. Refactor the code and alloc or calloc the needed memory yourself and of course free it before the program exits. You will need to use the function: _*int json\_object\_get\_string\_len(json_object *obj)*_ to determine how much memory to allocate.
