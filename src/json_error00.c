// gcc json-json_error00.c -ljson-c -o json_error00
//      This program illustrates a circular reference. It will crash if you run it!!
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
   json_object_object_add(items, "properties", root);

   printf("The json representation:\n\n%s\n\n", json_object_to_json_string_ext(root, JSON_C_TO_STRING_PRETTY));

   // cleanup and exit
   json_object_put(root);
   return 1;
}
