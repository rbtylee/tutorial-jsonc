// gcc json-new01.c -ljson-c -o json-new01

// Need more research in this referencing stuff
// purpose to show 2 attributes can have the same json obj as value
// from docs: Remember, when using json_object_object_add or json_object_array_put_idx, ownership will transfer to the object/array. Call json_object_get if you want to maintain shared ownership or also add this object as a child of multiple objects or arrays.


#include <stdio.h>
#include <json-c/json.h>

int
main(void)
{
   const char *filename = "example01.json";
   json_object *item, *location;
   
   json_object *root = json_object_new_object();
   if (!root)
      return 0;

   // main array
   json_object *items = json_object_new_array();
   json_object_object_add(root, "items", items);
   //       item 1
   item = json_object_new_object();
   json_object_object_add(item, "id", json_object_new_int(3201));
   json_object_object_add(item, "status", json_object_new_string("online"));
   json_object_array_add(items, item);
   location = json_object_new_object();
      
   json_object_object_add(location, "latitude", json_object_new_double(-117.125));
   json_object_object_add(location, "longitude", json_object_new_double(31.125));
   json_object_object_add(item, "location", location);
   json_object_get(location);
   //       item 2
   item = json_object_new_object();
   json_object_object_add(item, "id", json_object_new_int(4678));
   json_object_object_add(item, "status", json_object_new_string("online"));
   json_object_array_add(items, item);
   json_object_object_add(item, "location", location);

   //       item 3
   item = json_object_new_object();
   json_object_object_add(item, "id", json_object_new_int(9823));
   json_object_object_add(item, "status", json_object_new_string("offline"));
   json_object_array_add(items, item);
   location = json_object_new_object();
   json_object_object_add(location, "latitude", json_object_new_double(-111.625));
   json_object_object_add(location, "longitude", json_object_new_double(35.625));
   json_object_object_add(item, "location", location);

   // print json
   printf("The json representation:\n\n%s\n\n", json_object_to_json_string_ext(root, JSON_C_TO_STRING_PRETTY));
   // save json
   if (json_object_to_file(filename, root))
      printf("Error: failed to save %s!!\n", filename);
   else
      printf("%s saved.\n", filename);

   // cleanup and exit
   
   json_object_put(root);
   return 1;
}
