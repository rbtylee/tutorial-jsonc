// gcc json-parse03.c -ljson-c -o json-parse03

#include <stdio.h>
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
