# Parsing a JSON object - part 3 complex parsing

At this point in the tutorial, we have almost all the tools needed to completely parse the file _*contact.json*_.

However, there is some ambiquity in the file _*contact.json*_ we need to address first.

Consider the two attributes, "children" and "spouse":

```
  "children": [],
  "spouse": null
```
For *children*, assume a value of null means _*No Children*_ whereas an empty array means _*Unknown*_.
Where there are children, record the full name and age as in, for each child:

```
  "children": [
    {
      "name": "Tammy Smith",
      "age": 3
    },
    {
      "name": "Judah Smith",
      "age": 7
    }
  ]
```
For Spouse, a value of null will mean _*No Spouse Known*_. 
Where is a known spouse, record the full name and age as in:

```
  "spouse": {
      "name": "Amanda Smith",
      "age": 23
  }
```

## Some needed Array functions

With ambiquity issue addressed, we precede to read every value of every field in our contact.json file and print the results. To accomplish this we need first to determine the length of a json\_array in order to deal with the **Children** attribute. Json-c provides a function much like _*json_object_get_string_len*_ only for json_arrays:

- int json_object_array_length(json_object \*obj)

The usage of this function is should be intuitively clear. It does what it says it does.

Once we know the length of a json\_array, we have to be able to loop thru every _*json_object*_ the array contains. That is we need some function to _index_ the json_array. Hence:

- json_object\* json_object_array_get_idx(json_object \*obj, int idx)

This function retrives and returns the element at specificed index of the json_array or in the case of failure returns NULL.

With these two functions now _under our belt_ the rest is just *grunt work*: 

## json-parse04.c

```
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
      return 0;

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
   return 1;
}


```

Save, compile and execute _*json-parse04.c*_ above. 

Note: latter a simpler solution to this problem will be presented.

Problems

1. Change the values if the children and/or spouse fields in _*contact.json*_ following the conventions introduced above to eliminate the ambiguity of our file. Rerun json-parse04 on the new JSON file. Does it properly handle all possible cases?

- Refactor _*json-parse04.c*_ to store the json values in a C struct.

- Write a classic C program that does the same thing as _*json-parse04.c*_ using only functions found in the C standard library. Simplicity is better than a complex solution. Depending upon your programming ability, this may be a difficult problem.

- If have completed the above problem, discuss the pros and cons of using your classic C solution verses a json-c implemtation.
