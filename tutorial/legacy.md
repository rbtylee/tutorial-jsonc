# Parsing a Json object - part 1: Intro

Printing a json\_object is rather boring and mostly useful for debugging purposes. Naturally one would expect a JSON library to include functions to make extracting the needed data from the JSON file as easy and simple as possible.

So let's examine [_*contact.json*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/contact.json) again:

```json
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
Suppose we wish to read this JSON file and print only the first and last name. Examine [_*json-parse00.c*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/json-parse00.c) below. Compile with _*gcc json-parse00.c -ljson-c -o json-parse00*_:

```C
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

- [json_object\* json\_object\_object\_get(json_object \* obj, const char \*key)](https://json-c.github.io/json-c/json-c-0.15/doc/html/json__object_8h.html#a1a097805abb53b4c8a60d573730a8939)
- [const char\* json_object_get_string(json_object \*obj)](https://json-c.github.io/json-c/json-c-0.15/doc/html/json__object_8h.html#a9ee29ca8d79896e15007131527f6002e)


## json_object_object_get
The first function, _*json\_object\_object\_get(obj, key)*_, simply returns the _*json_object*_ associated with the given key. When you're done with the returned json_object there's no need to release it with a _*json_object_put()*_ as no reference counts are changed. According to the documentation, "Ownership of the returned value is retained by obj. " This means that if the owning object is freed (by a  _*json_object_put(obj)*_ function call then any usage of the _*json_object*_  returned by
 _*json_object_object_get(obj, key)*_ will return garbage as this pointer is no longer valid.

In our example, we have

```C
json_object *first_name = json_object_object_get(root,  "firstName");
```

If we wish to keep the _*json_object \*first_name*_  *AFTER* executing _*json_object_put(root)*_, we must use the function _*json_object_get(first_name)* *BEFORE* executing the _*json_object_put(root)*_ command!! If we gain ownership of _*json_object \*first_name*_ and do not transfer that ownership to another JSON object, then we must latter lose that ownership with a _*json_object_put(first_name)*_ statement.

This point is very important. To see this play around with the code some by moving the line _*json_object_put(root);*_ around in the source and then again after adding some _*json_object_get*_ function calls on the appropriate objects. Each time not only execute the binary file but also run the code through  [_*Valgrind*_](https://www.valgrind.org/).

Hopefully, all this is clear by now. If not, we will [later](https://github.com/rbtylee/tutorial-jsonc/blob/master/tutorial/memory.md) [examine](https://github.com/rbtylee/tutorial-jsonc/blob/master/tutorial/memory01.md) the concept of ownsership and references in quite a bit of detail.

So let's examine what the function _*json_object_object_get(obj, key)*_ actually does. In our case the line of code:

```C
json_object *first_name = json_object_object_get(root,  "firstName");
```

looks through the _*json_object*_ root, which is the internal memory representation of our file contact.json for an attribute called _*firstName*_ and returns the _*json_object*_ representing the value of this attribute: "John".

You can see this if you insert the statement below into the code:

```C
 printf("%s\n", json_object_to_json_string_ext(first_name, JSON_C_TO_STRING_PRETTY));
```

Why does it not simply return the string itself with no quotes? Because remember a _*json_object*_  is not necessarily always a string. It can be a number, a boolean, an array of _*json_objects*_, or even another _*json_object*_ . In general, we don't know unless we know the _*semantics*_ of the JSON data being parsed in advance or have some way of testing the _*type*_ of the _*json_object*_ . (More on JSON types latter).

!["JSON state diagram for values"](https://github.com/rbtylee/tutorial-jsonc/blob/master/Images/value.png)

What about error handling you might ask? What happens if the function _*json_object_object_get(obj, key)*_ is unable to find the value associate with the key in the _*json_object*_? In this case, NULL is returned. Again play with the code to see the results of such an invalid search. In production code, we would include some error handling code.

## json\_object\_get\_string

It should be intuitively clear what the function _*json\_object\_get_string(obj)*_ does. It simply returns the string value (const char*) of the _*json_object*_. According to the docs:
>If the passed object is not of type json\_type_string then the JSON representation of the object is returned.

Hence, it is safe to call this function with a NULL object, in which case (null) is returned.

It is important to note that the returned "string memory is managed by the json\_object and will be freed when the reference count of the json\_object drops to zero." Therefore, if one wishes to 'keep' the value of this string around, one has to store it in a char array. For example, [_*json-parse01.c*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/json-parse01.c):

```C
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <json-c/json.h>

int
main(void)
{
   char *fname, *lname;
   json_object *root = json_object_from_file("contact.json");
   if (!root)
       return 1;

   json_object *first_name = json_object_object_get(root,  "firstName");
   json_object *last_name = json_object_object_get(root,  "lastName");
   // Store the string values of these json_objects in our char arrays
   fname = strdup(json_object_get_string(first_name));
   lname = strdup(json_object_get_string(last_name));
   // Lose ownership of our json_objects first_name and last_name
   json_object_put(root);
   // Verify our char arrays still have the proper values
   printf("%s, %s\n", fname, lname);
   // Cleanup
   free(fname);
   free(lname);
   return 0;
}

```

Compile as usual (_*gcc json-parse01.c -ljson-c -o json-parse01*_)
Verify this works as expected and does not leak memory using Valgrind.

## json_object_object_get_ex

We noted the function _*json_object_object_get(obj, key)*_ may throw depreciated function warnings json-c version 0.12. This function was depreciated then but latter this warning was removed. There are no issues with using this function.

However, an alternative to _*json_object_object_get(obj, key)*_  is the function _*json_object_object_get_ex*_. It will compile warning free in json-c 0.12.

- [json_bool json_object_object_get_ex(json_object \*obj, const char *key, json_object \*\*value)](https://json-c.github.io/json-c/json-c-0.15/doc/html/json__object_8h.html#a90d5f16d58636f01d2ed1a6030c7366a)

## json_bool

Note: this introduces a new type. _*json_bool*_, clearly as expected merely a convenience to deal with Boolean values.

We have
```C
typedef int json_bool;
```
While some versions of json-c have TRUE and FALSE macros defined you probably shouldn't use them as they have removed in version 0.14. Many other libraries also provide similar 'types' and values for booleans.

Clearly the function _*json_object_object_get_ex(obj, key, &value)*_ returns TRUE (1) on success and FALSE (0) otherwise.

## json_object_object_get_ex

So here is _*json-parse00.c*_ refactored using this new function:

```C
#include <stdio.h>
#include <json-c/json.h>

int 
main(void)
{
   json_object *first_name, *last_name;
   json_object *root = json_object_from_file("contact.json");
   if (!root)
       return 1;
       
   json_object_object_get_ex(root, "firstName", &first_name);
   json_object_object_get_ex(root, "lastName", &last_name);
   printf("%s, %s\n", json_object_get_string(last_name), json_object_get_string(first_name));
   json_object_put(root);
   return 0;
}

```

As usual save this file as _*json-parse02.c*_. Compile and  execute ( _*gcc json-parse02.c -ljson-c -o json-parse02*_ ).

No compiler warnings and the code is only marginally more complex.

## Problems

1. Rewrite json-parse00.c with proper error checks on the returned values before the print statements. In other words, if the key is not found do not print (null) for either the first name and last name.

