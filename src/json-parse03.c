// gcc json-parse03.c -ljson-c -o json-parse03

#include <stdio.h>
#include <json-c/json.h>

int
main(void)
{
   json_object *root = json_object_from_file("contact.json");
   if (!root)
      return 1;

   json_object *first_name = json_object_object_get(root, "firstName");
   json_object *last_name = json_object_object_get(root, "lastName");

   printf("%s, %s\n", json_object_get_string(last_name), json_object_get_string(first_name));
   json_object_put(root);
   return 0;
}
