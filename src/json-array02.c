// gcc json-array02.c -ljson-c -o json-array02

#include <stdio.h>
#include <json-c/json.h>

int
main(void)
{
   const char *str;
   json_object *root = json_tokener_parse("[\"foo\", \"bar\", 32, null]");

   printf("The json representation:\n\n%s\n\n", json_object_to_json_string_ext(root, JSON_C_TO_STRING_PRETTY));

   int n = json_object_array_length(root);
   for (int i=0;i<n; i++)
   {
      str= json_object_get_string(json_object_array_get_idx(root, i));
      printf("The value at %i position is: %s\n", i, str);
   }

   json_object_put(root);

   return 0;
}
