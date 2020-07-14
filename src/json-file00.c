// gcc json-file00.c -ljson-c -o json-file00

#include <stdio.h>
#include <json-c/json.h>

int
main(void)
{
   json_object *root = json_object_from_file("contact.json");
   printf("The json file:\n\n%s\n", json_object_to_json_string(root));

   printf("The json file:\n\n%s\n", json_object_to_json_string_ext(root, JSON_C_TO_STRING_PRETTY));
   json_object_put(root);
   return 0;
}
