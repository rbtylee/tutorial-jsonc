// gcc json-parse04.c -ljson-c -o json-parse04

#include <stdio.h>
#include <json-c/json.h>

void json_object_print_type(json_object *obj, const char *key);

json_object *
_json_object_object_get(json_object *obj, const char *key)
{
   json_object *temp;
   if (json_object_object_get_ex(obj, key, &temp))
      return temp;
   printf("Error: in json obj or key\n");
   return NULL;
}

int
main(void)
{
   int temp_n;
   json_object *root, *temp;

   root = json_object_from_file("contact.json");
   if (!root)
      return 1;

   json_object *first_name = _json_object_object_get(root, "firstName");
   printf("First name: %s\n", json_object_get_string(first_name));
   json_object *last_name = _json_object_object_get(root, "lastName");
   printf("Last name: %s\n", json_object_get_string(last_name));
   json_object *is_alive = _json_object_object_get(root, "isAlive");
   printf("Is Alive: %s\n", json_object_get_boolean(is_alive)? "yes": "No");
   json_object *age = _json_object_object_get(root, "age");
   printf("Age: %d\n", json_object_get_int(age));

   printf("Address:\n");
   json_object *address = _json_object_object_get(root, "address");
   json_object *st_address = _json_object_object_get(address, "streetAddress");
   printf("\tStreet Address: %s\n", json_object_get_string(st_address));
   json_object *city = _json_object_object_get(address, "city");
   printf("\tCity: %s\n", json_object_get_string(city));
   json_object *state = _json_object_object_get(address, "state");
   printf("\tState: %s\n", json_object_get_string(state));
   json_object *zip = _json_object_object_get(address, "postalCode");
   printf("\tZip Code %s\n", json_object_get_string(zip));

   printf("Phone Numbers:\n");
   json_object *phone_numbers = _json_object_object_get(root, "phoneNumbers");
   temp_n = json_object_array_length(phone_numbers);
   for (int i = 0; i < temp_n; i++)
      {
         temp = json_object_array_get_idx(phone_numbers, i);
         json_object *type = _json_object_object_get(temp, "type");
         json_object *number = _json_object_object_get(temp, "number");
         printf("\tPhone number %d: %s: %s\n", i, json_object_get_string(type), json_object_get_string(number));
      }

   printf("Children:\n");
   json_object *children = _json_object_object_get(root, "children");
   if (json_object_get_type(children) == json_type_null)
      printf("\tNo children\n");
   else
      {
         temp_n = json_object_array_length(children);
         if (temp_n)
            for (int i = 0; i < temp_n; i++)
               {
                  temp = json_object_array_get_idx(children, i);
                  json_object *name = _json_object_object_get(temp, "name");
                  printf("\t Name: %s\n", json_object_get_string(name));
                  age = _json_object_object_get(temp, "age");
                  printf("\t Age: %d\n\n", json_object_get_int(age));
               }
         else
            printf("\tUnknown\n");
      }

   printf("Spouse:\n");
   json_object *spouse = _json_object_object_get(root, "spouse");
   if (json_object_get_type(spouse) == json_type_null)
      printf("\tNo Spouse Known\n");
   else
      {
         json_object *name = _json_object_object_get(spouse, "name");
         printf("\t Name: %s\n", json_object_get_string(name));
         age = _json_object_object_get(spouse, "age");
         printf("\t Age: %d\n\n", json_object_get_int(age));
      }

   json_object_put(root);
   return 0;
}
