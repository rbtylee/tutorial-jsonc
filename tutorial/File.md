# Reading a JSON file

To ease into things, let's start by creating a simple utility function to parse a JSON file; later we'll delve into how we can control the parsing in more detail. 
First, we consider how to read a JSON file into memory and print the file contents as a c string. For this example and many of the rest of the examples, I am going to use the JSON representation of a person taken from the aforementioned [Wikipedia article](https://en.wikipedia.org/wiki/JSON), [_*contact.json*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/contact.json):

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

Now examine [_*json-file00.c*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/json-file00.c):

```C
#include <stdio.h>
#include <json-c/json.h>

int
main(void)
{
   json_object *root = json_object_from_file("contact.json");
   if (!root)
      return 1;
   printf("The json file:\n\n%s\n", json_object_to_json_string(root));
   json_object_put(root);
   return 0;
}

```
Compile with:

```
 gcc json-file00.c -ljson-c -o json-file00
```

Now run _*json-file00*_:

```
$ ./json-file00
The json file:

{ "firstName": "John", "lastName": "Smith", "isAlive": true, "age": 27, "address": { "streetAddress": "21 2nd Street", "city": "New York", "state": "NY", "postalCode": "10021-3100" }, "phoneNumbers": [ { "type": "home", "number": "212 555-1234" }, { "type": "office", "number": "646 555-4567" } ], "children": [ ], "spouse": null }

```

Admittedly, the output is not very pretty, all extraneous whitespace has been stripped from the document. That may or may not be what you want in your application. Fortunately, json-c  offers a little more control over the printing of the JSON object.

Now lets look at our next program: [_*json-file01.c*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/json-file01.c). Compile with
_*gcc json-file01.c -ljson-c -o json-file01*_:

```C
#include <stdio.h>
#include <json-c/json.h>

int
main(void)
{
   json_object *root = json_object_from_file("contact.json");
   if (!root)
      return 1;
   printf("The json file:\n\n%s\n", json_object_to_json_string_ext(root, JSON_C_TO_STRING_PRETTY));
   json_object_put(root);
   return 0;
}
```

The output from running _*json-file01*_ is:

```
$ ./json-file01
The json file:

{
  "firstName":"John",
  "lastName":"Smith",
  "isAlive":true,
  "age":27,
  "address":{
    "streetAddress":"21 2nd Street",
    "city":"New York",
    "state":"NY",
    "postalCode":"10021-3100"
  },
  "phoneNumbers":[
    {
      "type":"home",
      "number":"212 555-1234"
    },
    {
      "type":"office",
      "number":"646 555-4567"
    }
  ],
  "children":[
  ],
  "spouse":null
}

```

Here, the output is more readable having been reformatted somewhat, but it is certainly more suitable for human reading than before. Especially for more complex and long json files.

So let's examine what we have learned from the code above.

First and foremost, I have introduced a new 'type': _*json_object*_. As expected for a C program, _*json_object*_ is really an opaque structure whose internal implementation is not available in application code outside of the json-c library. Your code will only ever deal with a _pointer_ to a _*json_object*_, and all operations must be performed through the json-c API functions. Some programmers who wish to stress the idea _*json_object*_ is a *struct* would write the line of code:

```C
json_object *root = json_object_from_file("contact.json");
```

as

```C
struct json_object *root = json_object_from_file("contact.json");
```

That is a personal choice and do whichever you prefer. I prefer to leave the struct part off and think of it more like an additional basic type the library offers much like *int* or *float*.

Next I introduce the function:

- [json_object\* *json_object_from_file*(const char \*filename)](https://json-c.github.io/json-c/json-c-0.15/doc/html/json__util_8h.html#a03119ec0a71af4eee95318e9b2aaf05b)

This function should be self-explanatory: It reads the file *filename* and returns the _*json_object*_ representing the JSON data contained in the file. If there is a failure to read the file, this function returns NULL. Of course, as we will later see json-c can also create a _*json_object*_ from a string (technically a NULL-terminated character array in C, not that I am pedantic enough to care).

Now note, right before our program exits we have the function:

- [void json_object_put(json_object \* obj)](https://json-c.github.io/json-c/json-c-0.15/doc/html/json__object_8h.html#afabf61f932cd64a4122ca8092452eed5)

We call this function on the json_object we created using *json_object_from_file*. You can think of this as freeing the memory allocated by creating the json_object. Technically though, the memory is only freed if the reference count of the object is zero. The official documentation states _*json_object_put*_

> Decrements the reference count of json\_object and frees if it reaches zero. You must have ownership of obj prior to doing this or you will cause an imbalance in the reference count.

You may be thinking, "What is meant by _*ownership of an object*_?" More on that [latter](https://github.com/rbtylee/tutorial-jsonc/blob/master/tutorial/memory.md), but for now, note most of sample programs will follow this template:

|  Pseudo-Code                            | Ownership                                                                                         |
| :-------------------------------------- | :------------------------------------------------------------------------------------------- |
| Create or initialize a root JSON_object |  _*at this point we own the object root and roots reference count is 1*_ |
| Process JSON, saving or displaying JSON data as needed | _*we usually create or initialize other json objects here and transfer ownership of them to root. As longs as roots ownership is not transfered or shared with another object, roots reference count is unchanged*_ |
| Dereference our JSON object |  _*at this point we lose ownership of root and its reference count is decremented. If roots reference count is 0, roots memory and all objects owned by root with a reference count of 0 is freed*_ |

Finally, I introduced 2 functions and a constant to convert the json_object to a string:

- [const char\* json_object_to_json_string(json_object \*obj)](https://json-c.github.io/json-c/json-c-0.15/doc/html/json__object_8h.html#ab7390c22baa1700d977c2af6b22d43a4)
- [const char\* json_object_to_json_string_ext(json_object \*obj, int flags)](https://json-c.github.io/json-c/json-c-0.15/doc/html/json__object_8h.html#a9db613127bd4ef7db42307e43a85fc1b)
- [JSON_C_TO_STRING_PRETTY](https://json-c.github.io/json-c/json-c-0.15/doc/html/json__object_8h.html#a2025bc677c35f130e117dfda5bf1ef73)

First, the constant _*JSON_C_TO_STRING_PRETTY*_ used in the function _*json_object_to_json_string_ext*_ as a _formatting flag_. There are 6 such flags, the remaining ones are:

- [JSON_C_TO_STRING_PLAIN](https://json-c.github.io/json-c/json-c-0.15/doc/html/json__object_8h.html#a3294cb92765cdeb497cfd346644d1059)
- [JSON_C_TO_STRING_SPACED](https://json-c.github.io/json-c/json-c-0.15/doc/html/json__object_8h.html#aa821746c8668e6ad62bed90ec9e00103)
- [JSON_C_TO_STRING_PRETTY_TAB](https://json-c.github.io/json-c/json-c-0.15/doc/html/json__object_8h.html#afc1486af21f6b1653c6f523025bdfd3b)
- [JSON_C_TO_STRING_NOZERO](https://json-c.github.io/json-c/json-c-0.15/doc/html/json__object_8h.html#a34f027c147babf69fc530d088f2b49b0)
- [JSON_C_TO_STRING_NOSLASHESCAPE](https://json-c.github.io/json-c/json-c-0.15/doc/html/json__object_8h.html#a5c11d72c55f3ab7c088f19e7bf118163)

For information of the remaining flags your json-c version supports consult [the documentation](https://json-c.github.io/json-c/json-c-0.15/doc/html/json__object_8h.html).

These flags tell the function _*json_object_to_json_string_ext*_ how to format the JSON in the string representation. We have already seen the usage and effect of _*JSON_C_TO_STRING_PLAIN*_: The function _*json_object_to_json_string(obj)*_ is equivalent to _*json_object_to_json_string_ext(obj, JSON_C_TO_STRING_SPACED)*_. Here all superfluous white space is removed from the string representation.

Now, to see the effect of using JSON_C_TO_STRING_SPACED edit the file json-file01.c and add it as the flag instead of _*JSON_C_TO_STRING_PRETTY*_. What do think it does?

## Problems

1. What happens if our json file contains invalid json and we run _*json-file01*_ ? Alter the program to print a message informing the user the json is invalid.
