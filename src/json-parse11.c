// gcc json-parse11.c -L/usr/local/lib -I/usr/local/include/json-c -ljson-c -o json-parse11
// see json-parse07.c

#include <stdio.h>
#include <string.h>
#include <json-c/json.h>
#include <json_visit.h>

#define EMAIL "ptunkiny@angelfire.com"

static int 
doit(json_object *obj, int flags, json_object *parent, const char *key,
                       size_t *index, void *data)
{
   if (!parent || flags==JSON_C_VISIT_SECOND || 
       json_object_get_type(obj) == json_type_object || 
       json_object_get_type(obj) == json_type_array)
      return JSON_C_VISIT_RETURN_CONTINUE;
      
   static const char *first_name = "";
   static const char *last_name = "";
   
   if (strcmp(key, "first_name") == 0)
       first_name = json_object_get_string(obj);
   else if (strcmp(key, "last_name") == 0)
      last_name = json_object_get_string(obj);

   if (strcmp(json_object_get_string(obj), EMAIL) == 0)
   {
      printf("Found: %s %s %s\n", first_name, last_name, json_object_to_json_string(obj));
      return JSON_C_VISIT_RETURN_STOP;
   }
   return JSON_C_VISIT_RETURN_CONTINUE;
}

int 
main(void)
{
   json_object *root = json_object_from_file("sample00.json");
   printf("Search for %s\n\n", EMAIL);
   json_c_visit(root, 0, doit, NULL);

   json_object_put(root);
   return 0;
}
