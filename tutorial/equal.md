# JSON Comparison

Now for something easier. What does it mean for two JSON objects to be equal?

To test for equality of two JSON objects json-c provides the function:

- int json_object_equal(json_object *obj1, json_object *obj2)

From the documentation:

> If the passed objects are equal 1 will be returned. Equality is defined as follows:
> - json_objects of different types are never equal
> - json_objects of the same primitive type are equal if the c-representation of their value is equal
> - json-arrays are considered equal if all values at the same indices are equal (same order)
> - Complex json_objects are considered equal if all contained objects referenced by their key are equal, regardless their order.

## json-equal00.c

As expected some sample code illustrating the usage of this function:

```
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
      printf("Documents are equal!\n");
   else
      printf("Documents are NOT equal!\n");
   // cleanup and exit
   json_object_put(doc1);
   json_object_put(doc2); 
   return 0;
}

```
