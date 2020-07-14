// gcc json-parse07.c -ljson-c -o json-parse07

#include <stdio.h>
#include <json-c/json.h>

void doarray(json_object *obj);

void
doit(json_object *obj)
{
   json_object_object_foreach(obj, key, val)
   {
      switch (json_object_get_type(val))
         {
          case json_type_array:
             printf("\n%s  \n\n", key);
             doarray(val);
             break;

          case json_type_object:
             printf("\n%s  \n\n", key);
             doit(val);
             break;

          default:
             printf("%s: %s\n", key, json_object_get_string(val));
         }
   }
}

void
doarray(json_object *obj)
{
   int temp_n = json_object_array_length(obj);
   for (int i = 0; i < temp_n; i++)
      doit(json_object_array_get_idx(obj, i));
}

int
main(void)
{
   json_object *root = json_object_from_file("contact.json");

   if (!root)
      return 0;
   doit(root);

   json_object_put(root);
   return 0;
}
