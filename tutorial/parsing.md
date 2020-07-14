# Parsing a JSON object - part 2: Depreciated Function solution

In our last section, we noted the function _*json_object_object_get(obj, key)*_ may throw depreciated function warnings depending upon what version of jcon-c you have installed. Gcc will throw these ugly warnings for each usage of this function when compiling the function.

It seems the function was depreciated and then latter this warning was removed. But regardless, compiler warnings do not look very professional.

An alternative to _*json_object_object_get(obj, key)*_  is the function _*json_object_object_get_ex*_. It will compile warning free.

- json_bool json_object_object_get_ex(json_object \*obj, const char *key, json_object \*\*value)

## json_bool

Note: this introduces a new type. _*json_bool*_, clearly as expected merely a convenience to deal with Boolean values.

We have
```
typedef int json_bool;
```
While some versions of json-c have TRUE and FALSE macros defined you probably shouldn't use them as they have removed in version 0.14. Many other libraries also provide similar 'types' and values for booleans.

Clearly the function _*json_object_object_get_ex(obj, key, &value)*_ returns TRUE (1) on success and FALSE (0) otherwise.

## json_object_object_get_ex

So here is _*json-parse00.c*_ refactored using this new function:

```
#include <stdio.h>
#include <json-c/json.h>

int 
main(void)
{
   json_object *first_name, *last_name;
   json_object *root = json_object_from_file("contact.json");
   if (!root)
       return 0;
       
   json_object_object_get_ex(root, "firstName", &first_name);
   json_object_object_get_ex(root, "lastName", &last_name);
   printf("%s, %s\n", json_object_get_string(last_name), json_object_get_string(first_name));
   json_object_put(root);
   return 1;
}

```

As usual save this file as_*json-parse02.c*_. Compile and  execute (_*gcc json-parse02.c -ljson-c -o json-parse02 *_).

No compiler warnings and the code is only marginally more complex.

## Roll your own json_object_object_get function.

**Note:** since _*json_object_object_get*_ is no longer depreciated, we will not be using the function _*/_json_object_object_get*_. It is only introduced here for historical reasons.

Since I like the simplicity of the depreciated function _*json_object_object_get*_, I prefer to create my own substitute and hence avoid the depreciated warnings. Hence, _*json-parse03.c*_ :

```
##include <stdio.h>
#include <json-c/json.h>

json_object *
_json_object_object_get(json_object *obj, const char *key)
{
   json_object *temp;
   if (json_object_object_get_ex(obj, key, &temp))
      return temp;
   printf("Error: in json obj or key\n");
   return NULL;
}

int
main(void)
{
   json_object *root = json_object_from_file("contact.json");
   if (!root)
      return 0;

   json_object *first_name = _json_object_object_get(root, "firstName");
   json_object *last_name = _json_object_object_get(root, "lastName");

   printf("%s, %s\n", json_object_get_string(last_name), json_object_get_string(first_name));
   json_object_put(root);
   return 1;
}

```

Save (_*json-parse03.c*_), compile and execute.

## Problems

1. Verify that the function _*_json_object_object_get*_ behaves more or less the same as _*json_object_object_get*_ when it comes to dereferencing and freeing memory issues. Do this by playing with the code and using Valgrind. How about my function _*\_json_object_object_get*_ used in _*json-parse03.c*_?

2. In what versions of json-c is _*json_object_object_get*_ depreciated?
