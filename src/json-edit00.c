// gcc json-edit00.c -ljson-c -o json-edit00

#include <stdio.h>
#include <json-c/json.h>

int
main(void)
{
   json_object *root = json_object_from_file("contact.json");
   
   printf("The json file:\n\n%s\n\n", json_object_to_json_string_ext(root, JSON_C_TO_STRING_PRETTY));
   // Edit age
   json_object *age = json_object_object_get(root, "age");
   json_object_int_inc(age, 1);

   // Edit office phone number
   json_object *phone_numbers =  json_object_object_get(root, "phoneNumbers");
   json_object *office = json_object_array_get_idx(phone_numbers, 1);
   json_object *number = json_object_object_get(office,"number");
   json_object_set_string(number, "843 276-3583");
   // Add Daughter
   json_object *children =  json_object_object_get(root, "children");
   json_object *paige = json_object_new_object();
   json_object_object_add(paige, "name", json_object_new_string("Paige Crawford"));
   json_object_object_add(paige, "age", json_object_new_int(15));
   json_object_array_add(children, paige);
   
   // Add Wife
   json_object *spouse = json_object_object_get(root, "spouse");
   json_object_object_del(root,"spouse");
   json_object_object_add(root,"spouse",json_object_new_string("Kate Smith"));
   printf("The Edited JSON:\n\n%s\n", json_object_to_json_string_ext(root, JSON_C_TO_STRING_PRETTY));
   

   json_object_put(root);
   return 0;
}
