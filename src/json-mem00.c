//gcc json-mem00.c -ljson-c -o json-mem00

#include <stdio.h>
#include <json-c/json.h>
#include <json-c/json_object_private.h>
int
main(void)
{
   json_object *items, *item, *str;
   
   json_object *root = json_object_new_object();
   if (!root)
      return 1;

   // main array
   items = json_object_new_array();
   json_object_object_add(root, "items", items);
   //       item 1
   item = json_object_new_object();
   json_object_object_add(item, "id", json_object_new_int(3201));
   str =  json_object_new_string("online");
   json_object_object_add(item, "status", str);
   json_object_array_add(items, item);

   //       item 2
   item = json_object_new_object();
   json_object_object_add(item, "id", json_object_new_int(4678));
   json_object_get(str);
   json_object_object_add(item, "status", str);
   json_object_array_add(items, item);
   
   
      //       item 3
   item = json_object_new_object();
   json_object_object_add(item, "id", json_object_new_int(2345));
   json_object_get(str);
   json_object_object_add(item, "status", str);
   json_object_array_add(items, item);
   
   printf("The json representation:\n\n%s\n\n", json_object_to_json_string_ext(root, JSON_C_TO_STRING_PRETTY));

   // cleanup and exit
   json_object_put(root);
   //printf("SHIT %d\n", str->_ref_count);
   return 0;
}
