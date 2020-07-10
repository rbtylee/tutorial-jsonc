// gcc json-equal00.c -ljson-c -o json-equal00

// Need more research in this referencing stuff
// purpose to show 2 attributes can have the same json obj as value


#include <stdio.h>
#include <json-c/json.h>

int
main(void)
{
   const char *filename = "example01.json";
   json_object *item, *location;
   
   json_object *doc1 = json_object_new_object();
   // JSON Document 1
   json_object_object_add(doc1, "firstName", json_object_new_string("John"));
   json_object_object_add(doc1, "lastName", json_object_new_string("Smith"));
   json_object_object_add(doc1, "age", json_object_new_int(27));
   printf("Document 1:\n\n%s\n\n", json_object_to_json_string_ext(doc1, JSON_C_TO_STRING_PRETTY));
   json_object *doc2 = json_object_new_object();
   // JSON Document 2
   json_object_object_add(doc2, "age", json_object_new_int(27));
   json_object_object_add(doc2, "firstName", json_object_new_string("John"));
   json_object_object_add(doc2, "lastName", json_object_new_string("Smith"));
   printf("Document 2:\n\n%s\n\n", json_object_to_json_string_ext(doc2, JSON_C_TO_STRING_PRETTY));
   
   if (json_object_equal(doc1,doc2))
      printf("Documents are equal!");
   else
      printf("Documents are NOT equal!");
   // cleanup and exit
   json_object_put(doc1);
   json_object_put(doc2); 
   return 1;
}
