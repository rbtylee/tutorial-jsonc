// gcc json-parse06.c -ljson-c -o json-parse06

#include <stdio.h>
#include <stdlib.h>
#include <json-c/json.h>

int
main(void)
{
   json_object *root = json_object_from_file("contact.json");
   json_object_object_foreach(root, key, val)
   printf("%s  -> %s\n", key, json_object_get_string(val));

   json_object_put(root);
   return 0;
}
