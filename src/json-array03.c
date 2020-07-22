// gcc json-array03.c -ljson-c -o json-array03

#include <stdio.h>
#include <json-c/json.h>
#include <string.h>

#define NUM_KEYS 2

static int
sort_fn(const void *j1, const void *j2)
{
   json_object *const *jso1, *const *jso2;
   const char *str1, *str2;

   jso1 = (json_object *const *) j1;
   jso2 = (json_object *const *) j2;

   if (!*jso1 && !*jso2)
       return 0;
   if (!*jso1)
       return -1;
   if (!*jso2)
       return 1;

   str1 = json_object_get_string(*jso1); // This will always work even if not string
   str2 = json_object_get_string(*jso2);

   return strcmp(str1, str2);
}

int
main(void)
{
   json_object *str, *temp;
   char *keys[NUM_KEYS] = {"Vladimir", "Nathaniel"};

   json_object *root = json_object_from_file("names.json");
   printf("The json representation:\n\n%s\n\n", json_object_to_json_string_ext(root, JSON_C_TO_STRING_PRETTY));

   json_object_array_sort(root, sort_fn);
   printf("The json representation:\n\n%s\n\n", json_object_to_json_string_ext(root, JSON_C_TO_STRING_PRETTY));

   for(int i=0; i <NUM_KEYS; i++)
   {
     str = json_object_new_string(keys[i]);
     temp = json_object_array_bsearch(str, root, sort_fn);
     printf("Search for key '%s' yields: %s\n", keys[i], json_object_get_string(temp));
     json_object_put(str);
   }

   json_object_put(root);

   return 0;
}
