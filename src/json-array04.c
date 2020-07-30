// gcc json-array04.c -L/usr/local/lib -I/usr/local/include/json-c -ljson-c -o json-array04.c
// see json-array02.c
#include <stdio.h>
#include <json-c/json.h>
#include <json_visit.h>

static int 
print_str(json_object *jso, int flags, json_object *parent, const char *key,
                       size_t *index, void *data)
{
  if (jso_index)
     printf("The value at %ld position is: %s\n", (long)* index, json_object_get_string(jso));
  return JSON_C_VISIT_RETURN_CONTINUE;
}

int 
main(void)
{
   json_object *root = json_object_from_file("names.json");
   json_c_visit(root, 0, print_str, NULL);

   json_object_put(root);
   return 0;
}
