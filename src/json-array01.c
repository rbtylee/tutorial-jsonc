// gcc json-array01.c -ljson-c -o json-array01

#include <stdio.h>
#include <json-c/json.h>

int
main(void)
{
   json_object *root = json_tokener_parse("[\"foo\", \"bar\", 32, null]");

   printf("The json representation:\n\n%s\n\n", json_object_to_json_string_ext(root, JSON_C_TO_STRING_PRETTY));

   int val = json_object_get_int(json_object_array_get_idx(root, 2));
   printf("The third value of the array is: %d\n\n", val);

   printf("Modifying the third value and deleting the fourth.\n\n");
   json_object_array_put_idx(root, 2, json_object_new_string("foobar"));
   json_object_array_del_idx(root, 3, 1);

   printf("The json representation is now:\n\n%s\n\n", json_object_to_json_string_ext(root, JSON_C_TO_STRING_PRETTY));

   json_object_put(root);

   return 0;
}
