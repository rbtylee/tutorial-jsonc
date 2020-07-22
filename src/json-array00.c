// gcc json-array00.c -ljson-c -o json-array00

#include <stdio.h>
#include <json-c/json.h>

int
main(void)
{
   json_object *root = json_tokener_parse("[\"foo\", \"bar\", 32, null]");
   printf("The json string:\n\n%s\n\n", json_object_to_json_string(root));
   
   printf("The json object to string:\n\n%s\n", json_object_to_json_string_ext(root, JSON_C_TO_STRING_PRETTY));

   json_object_put(root);

   return 0;
}
