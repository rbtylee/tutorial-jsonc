// gcc json-type00.c -ljson-c -o json-type00

#include <stdio.h>
#include <json-c/json.h>

void json_object_print_type(json_object *obj, const char *key);

int
main(void)
{
   json_object *root = json_object_from_file("contact.json");
   if (!root)
      return 1;

   json_object *first_name = json_object_object_get(root, "firstName");
   json_object_print_type(first_name, "firstName");
   json_object *last_name = json_object_object_get(root, "lastName");
   json_object_print_type(last_name, "lastName");
   json_object *is_alive = json_object_object_get(root, "isAlive");
   json_object_print_type(is_alive, "isAlive");
   json_object *age = json_object_object_get(root, "age");
   json_object_print_type(age, "age");
   json_object *address = json_object_object_get(root, "address");
   json_object_print_type(address, "address");
   json_object *phone_numbers = json_object_object_get(root, "phoneNumbers");
   json_object_print_type(phone_numbers, "phoneNumbers");
   json_object *children = json_object_object_get(root, "children");
   json_object_print_type(children, "children");
   json_object *spouse = json_object_object_get(root, "spouse");
   json_object_print_type(spouse, "spouse");

   json_object_put(root);
   return 0;
}

void
json_object_print_type(json_object *obj, const char *key)
{
   json_type type;

   type = json_object_get_type(obj);  /*Getting the type of the json object*/
   switch (type)
      {
       case json_type_null:
          printf("%s is json_type_null\n", key);
          printf("          value: %s\n", json_object_get_string(obj));
          break;

       case json_type_boolean:
          printf("%s is json_type_boolean\n", key);
          printf("          value: %s\n", json_object_get_boolean(obj)? "true": "false");
          break;

       case json_type_double:
          printf("%s is json_type_double\n", key);
          printf("          value: %lf\n", json_object_get_double(obj));
          break;

       case json_type_int:
          printf("%s is json_type_int\n", key);
          printf("          value: %d\n", json_object_get_int(obj));
          break;

       case json_type_object:
          printf("%s is json_type_object\n", key);
          printf("          value: %s\n", json_object_get_string(obj));
          break;

       case json_type_array:
          printf("%s is json_type_array\n", key);
          printf("          value: %s\n", json_object_get_string(obj));
          break;

       case json_type_string:
          printf("%s is json_type_string\n", key);
          printf("          value: %s\n", json_object_get_string(obj));
          break;

       default:
          printf("%s: WTF, unhandled case, type %d\n", key, type);
          printf("          value: %s\n", json_object_get_string(obj));
      }
}
