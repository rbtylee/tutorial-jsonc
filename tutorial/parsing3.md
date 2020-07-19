# Parsing a Json object - part 3: JSON strings

At last, we have all the tools needed to parse a JSON file. Yet, the most common usage of json is to request the data from a website, download the data, and store it in a string.  Parsing, processing, and displaying the data are the last things we do. For such usage, we wouldn't use the _*json\_object\_from\_file*_ function. Admittedly we could: download the file and store it somewhere and then load the file into a JSON object. All this is unnecessary, wasteful, and slow. Json-c has a function to create a _*json\_object*_ from a C string:

- [json_object* json_tokener_parse(const char *str) ](https://json-c.github.io/json-c/json-c-0.14/doc/html/json__tokener_8h.html#a236ef64d079822a4411d13eae7190c4d)

The usage of this function is rather simple.

Consider _*json-str00.c*_:

```
#include <stdio.h>
#include <json-c/json.h>

int 
main(void)
{
   char * str = "{\"firstName\" : \"John\", \"lastName\" :  \"Smith\", \"email\":[{\"type\": \"personal\",\"url\": \"johnsmith0321@gmail.com\"},{\"type\": \"office\",\"url\": \"johnsmith@frankford.com\"} ] }";
   json_object *root = json_tokener_parse(str);
   printf("The json string:\n\n%s\n\n", json_object_to_json_string(root));
   
   printf("The json object to string:\n\n%s\n", json_object_to_json_string_ext(root, JSON_C_TO_STRING_PRETTY));
   json_object_put(root);
   return 0;
}

```

Once compiled and executed, the results:

```
$ ./json-str00 
The json string:

{ "firstName": "John", "lastName": "Smith", "email": [ { "type": "personal", "url": "johnsmith0321@gmail.com" }, { "type": "office", "url": "johnsmith@frankford.com" } ] }

The json object to string:

{
  "firstName":"John",
  "lastName":"Smith",
  "email":[
    {
      "type":"personal",
      "url":"johnsmith0321@gmail.com"
    },
    {
      "type":"office",
      "url":"johnsmith@frankford.com"
    }
  ]
}

```

Not much to say here, other than it is a hassle to escape all the quote characters in a JSON representation for usage in a C program. However, this is not a problem for downloading and storing JSON from the internet.

Problem:

1. Take _*json-parse04.c*_ previously discussed and add the macro below and use _*json\_tokener\_parse(SAMPLE_JSON)*_ instead of _*json\_object\_from_file*_. Save the file as _*json-parse05.c*_.

Verify everything works as before:

```
#include <stdio.h>
#include <json-c/json.h>
#define SAMPLE_JSON                            \
   "{"                                         \
   "  \"firstName\": \"John\","                \
   "  \"lastName\": \"Smith\","                \
   "  \"isAlive\": true,"                      \
   "  \"age\": 27,"                            \
   "  \"address\": {"                          \
   "    \"streetAddress\": \"21 2nd Street\"," \
   "    \"city\": \"New York\","               \
   "    \"state\": \"NY\","                    \
   "    \"postalCode\": \"10021-3100\""        \
   "  },"                                      \
   "  \"phoneNumbers\": ["                     \
   "    {"                                     \
   "      \"type\": \"home\","                 \
   "      \"number\": \"212 555-1234\""        \
   "    },"                                    \
   "    {"                                     \
   "      \"type\": \"office\","               \
   "      \"number\": \"646 555-4567\""        \
   "    }"                                     \
   "  ],"                                      \
   "  \"children\": ["                         \
   "    {"                                     \
   "      \"name\": \"Tammy Smith\","          \
   "      \"age\": 3"                          \
   "    },"                                    \
   "    {"                                     \
   "      \"name\": \"Judah Smith\","          \
   "      \"age\": 7"                          \
   "    }"                                     \
   "  ],"                                      \
   "  \"spouse\": {"                           \
   "      \"name\": \"Amanda Smith\","         \
   "      \"age\": 23"                         \
   "  }"                                       \
   "}"

int
main(void)
{
   int temp_n;
   json_object *root, *temp;

   root = json_tokener_parse(SAMPLE_JSON);
   if (!root)
      {
         printf("Root is null\n");
         return 1;
      }

   json_object *first_name = json_object_object_get(root, "firstName");
   printf("First name: %s\n", json_object_get_string(first_name));
   json_object *last_name = json_object_object_get(root, "lastName");
   printf("Last name: %s\n", json_object_get_string(last_name));
   json_object *is_alive = json_object_object_get(root, "isAlive");
   printf("Is Alive: %s\n", json_object_get_boolean(is_alive)? "yes": "No");
   json_object *age = json_object_object_get(root, "age");
   printf("Age: %d\n", json_object_get_int(age));

   printf("Address:\n");
   json_object *address = json_object_object_get(root, "address");
   json_object *st_address = json_object_object_get(address, "streetAddress");
   printf("\tStreet Address: %s\n", json_object_get_string(st_address));
   json_object *city = json_object_object_get(address, "city");
   printf("\tCity: %s\n", json_object_get_string(city));
   json_object *state = json_object_object_get(address, "state");
   printf("\tState: %s\n", json_object_get_string(state));
   json_object *zip = json_object_object_get(address, "postalCode");
   printf("\tZip Code %s\n", json_object_get_string(zip));

   printf("Phone Numbers:\n");
   json_object *phone_numbers = json_object_object_get(root, "phoneNumbers");
   temp_n = json_object_array_length(phone_numbers);
   for (int i = 0; i < temp_n; i++)
      {
         temp = json_object_array_get_idx(phone_numbers, i);
         json_object *type = json_object_object_get(temp, "type");
         json_object *number = json_object_object_get(temp, "number");
         printf("\tPhone number %d: %s: %s\n", i, json_object_get_string(type), json_object_get_string(number));
      }

   printf("Children:\n");
   json_object *children = json_object_object_get(root, "children");
   if (json_object_get_type(children) == json_type_null)
      printf("\tNo children\n");
   else
      {
         temp_n = json_object_array_length(children);
         if (temp_n)
            for (int i = 0; i < temp_n; i++)
               {
                  temp = json_object_array_get_idx(children, i);
                  json_object *name = json_object_object_get(temp, "name");
                  printf("\t Name: %s\n", json_object_get_string(name));
                  age = json_object_object_get(temp, "age");
                  printf("\t Age: %d\n\n", json_object_get_int(age));
               }
         else
            printf("\tUnknown\n");
      }

   printf("Spouse:\n");
   json_object *spouse = json_object_object_get(root, "spouse");
   if (json_object_get_type(spouse) == json_type_null)
      printf("\tNo Spouse Known\n");
   else
      {
         json_object *name = json_object_object_get(spouse, "name");
         printf("\t Name: %s\n", json_object_get_string(name));
         age = json_object_object_get(spouse, "age");
         printf("\t Age: %d\n\n", json_object_get_int(age));
      }

   json_object_put(root);
   return 0;
}

```

