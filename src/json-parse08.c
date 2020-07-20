// gcc json-parse08.c -ljson-c -o json-parse08

#include <stdio.h>
#include <json-c/json.h>

int 
main(void)
{
   struct json_object_iterator it;
   struct json_object_iterator itEnd;

   json_object *root = json_object_from_file("contact.json");
   it = json_object_iter_init_default();
   it = json_object_iter_begin(root);
   itEnd = json_object_iter_end(root);

   while (!json_object_iter_equal(&it, &itEnd))
   {
      const char* key = json_object_iter_peek_name(&it);
      json_object* val = json_object_iter_peek_value(&it);
      printf("%s  -> %s\n", key, json_object_get_string(val));
      json_object_iter_next(&it);
   }
   json_object_put(root);
   return 0;
}
