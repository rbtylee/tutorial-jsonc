// gcc json-parse10.c -L/usr/local/lib -I/usr/local/include/json-c -ljson-c -o json-parse10
// see json-parse07.c

#include <stdio.h>
#include <json-c/json.h>
#include <json_visit.h>

static int 
doit(json_object *obj, int flags, json_object *parent, const char *key,
                       size_t *index, void *data)
{
   if (!parent || flags==JSON_C_VISIT_SECOND) return JSON_C_VISIT_RETURN_CONTINUE;
   printf("key: %s, value: %s\n", key, json_object_to_json_string(obj));
   return JSON_C_VISIT_RETURN_CONTINUE;
}

int 
main(void)
{
   json_object *root = json_object_from_file("depth.json");
   json_c_visit(root, 0, doit, NULL);

   json_object_put(root);
   return 0;
}
