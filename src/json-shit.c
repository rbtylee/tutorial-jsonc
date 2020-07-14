// gcc json-new00.c -ljson-c -o json-new00

#include <stdio.h>
#include <json-c/json.h>

int
main(void)
{
   json_object *root = json_object_new_object();
   json_object_object_add(root, "id", NULL);
   printf("The json representation:\n\n%s\n\n", json_object_to_json_string_ext(root, JSON_C_TO_STRING_PRETTY));

   json_object_put(root);
   return 0;
}
