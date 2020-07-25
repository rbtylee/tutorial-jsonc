# Parsing a Json object - part 6: Iteration yet again

## json-array04.c

```
#include <stdio.h>
#include <json-c/json.h>
#include <json_visit.h>

static int 
print_str(json_object *jso, int flags, json_object *parent_jso, const char *jso_key,
                       size_t *jso_index, void *userarg)
{
  if (jso_index)
     printf("The value at %ld position is: %s\n", (long)* jso_index, json_object_get_string(jso));
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
```

## json-parse10.c

```
#include <stdio.h>
#include <json-c/json.h>
#include <json_visit.h>

static int 
doit(json_object *jso, int flags, json_object *parent_jso, const char *jso_key,
                       size_t *jso_index, void *userarg)
{
  printf("flags: 0x%x, key: %s, index: %ld, value: %s\n", flags,
     (jso_key ? jso_key : "(null)"), (jso_index ? (long)*jso_index : -1L),
      json_object_to_json_string(jso));
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

```

## json-parse11.c

```
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
```
