// gcc json-parse11.c -L/usr/local/lib -I/usr/local/include/json-c -ljson-c -o json-parse11
// see json-parse07.c

#include <stdio.h>
#include <json-c/json.h>
#include <json_visit.h>

static int 
doit(json_object *jso, int flags, json_object *parent_jso, const char *jso_key,
                       size_t *jso_index, void *userarg)
{
  if (!parent_jso || flags ==2 ) return JSON_C_VISIT_RETURN_CONTINUE;
  
  if (jso_key && json_object_get_type(jso) == json_type_object)
         printf("\n%s\n\n", jso_key);
  else if (json_object_get_type(jso) == json_type_array)
        printf("\n%s  \n\n", jso_key);
  else if (json_object_get_type(jso) != json_type_object)
           printf("%s: %s\n", jso_key, (jso?  json_object_to_json_string(jso):"(null)"));
     
  return JSON_C_VISIT_RETURN_CONTINUE;
}

int 
main(void)
{
   json_object *root = json_object_from_file("contact.json");
   json_c_visit(root, 0, doit, NULL);

   json_object_put(root);
   return 0;
}
