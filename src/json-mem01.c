//gcc json-mem01.c -ljson-c -o json-mem01

#include <stdio.h>
#include <json-c/json.h>

#define NUM_ID_ITEMS 3

json_object *
new_item(int id, json_object *str)
{
   json_object *item = json_object_new_object();
   json_object_object_add(item, "id", json_object_new_int(id));
   json_object_object_add(item, "status", json_object_get(str));
   return item;
}

int
main(void)
{
   json_object *items, *str;
   int id[3] = {3201, 4678, 2345};

   json_object *root = json_object_new_object();
   if (!root)
      return 1;

   // main array
   items = json_object_new_array();
   json_object_object_add(root, "items", items);
   str = json_object_new_string("online");

   // Add Items
   for(int i=0; i<NUM_ID_ITEMS; i++)
      json_object_array_add(items, new_item(id[i], str));

   printf("The json representation:\n\n%s\n\n", json_object_to_json_string_ext(root, JSON_C_TO_STRING_PRETTY));

   // cleanup and exit
   json_object_put(str);
   json_object_put(root);
   return 0;
}
