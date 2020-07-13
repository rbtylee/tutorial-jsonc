# JSON Circular references

After the previous two sections, you should be pretty clear on when to use json-c's put and get functions. In this section, we will examine one possible problem that can result from the reckless use of json_object_object_add. The problem is trying to create a JSON that contains a reference to itself.

[Previously in a problem](https://github.com/rbtylee/tutorial-jsonc/blob/master/tutorial/new.md), you were asked to write a program to generate the following JSON:

```
{
  "items":
  {
    "properties":
    {
      "name":
      {
        "type": "unknown"
      }
    }
  },
  "common":
  {
    "properties": null
  }
}
```

Here is a possible solution, only it doesn't save the resulting JSON:

```
#include <stdlib.h>
#include <stdio.h>
#include <json-c/json.h>
int
main(void)
{
   json_object *root = json_object_new_object();
   if (!root)
      return 0;

   // main array
   json_object *items = json_object_new_object();
   json_object_object_add(root, "items", items);
   //       item 1
   
   json_object *properties = json_object_new_object();
   json_object_object_add(items, "properties", properties);
   json_object *name = json_object_new_object();
   json_object_object_add(properties, "name", name);
   json_object *type = json_object_new_string("unknown");
   json_object_object_add(name, "type", type);
   
   items = json_object_new_object();
   json_object_object_add(root, "common", items);
   json_object_object_add(items, "properties", NULL);

   printf("The json representation:\n\n%s\n\n", json_object_to_json_string_ext(root, JSON_C_TO_STRING_PRETTY));

   // cleanup and exit
   json_object_put(root);
   return 1;
}
```

Now change the line:
```
json_object_object_add(items, "properties", NULL
```
to
```
json_object_object_add(items, "properties", root);
```

This is a circular reference, the JSON object root contains itself. Json-c only checks for the simplist of errors of this sort. This code will compile but fails to run:

```
~$ ./json-error00
The json representation:

(null)

json-new02: json_object.c:189: json_object_put: Assertion `jso->_ref_count > 0' failed.
Aborted (core dumped)
```

A programmer is unlikely to do anything this **obvious**. But by keeping or owning references to existing JSON objects and perhaps by having several pointers pointing to the same JSON object, it would be possible to introduce such a circular reference in a manner that is not so obvious.

So view this section as a warning ...
