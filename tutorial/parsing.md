# Parsing a JSON object - part 2: Depreciated Function solution

In our last section, we noted the function _*json_object_object_get(obj, key)*_ is depreciated and gcc will throw some ugly warnings for each usage of this function when compiling the function. There are two reasons I covered this function first instead of using the function intended to replace it.

1. Usage of this function still occurs in current production code and it is best to be familar with it

2. I find using this function slightly more convenient than using the newer non-depreciated function.


But with depreciated functions it is best to not use them. Future updates may break your code or your code may not work as expected. Plus tons of compiler warns do not look very professional.

The function we should be using is:

- json_bool json_object_object_get_ex(json_object \*obj, const char *key, json_object \*\*value)

## json_bool

Note: this introduces a new type. _*json_bool*_, clearly as expected merely a convenience to deal with Boolean values.

We have
```
typedef int json_bool;
```
and
 
```
#define FALSE   ((json_bool) 0)
#define TRUE   ((json_bool) 1)
```

You can use these if you wish. Many other libraries also provide simliar 'types' and values for booleans.

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

Save (_*json-parse03.c *_), compile and execute.

## Problems

1. Verify the function _*_json_object_object_get*_ behaves more less the same as _*json_object_object_get*_ when it comes to dereferencing and freeing memory issues. Do this by playing with the code and using valgrind. How about my function _*\_json_object_object_get*_ used in _*json-parse03.c *_?

