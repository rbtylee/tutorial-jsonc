// gcc json-parse02.c -ljson-c -o json-parse02

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
