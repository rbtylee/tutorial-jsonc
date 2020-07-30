# Parsing a Json object - part 3: JSON strings

At last, we have all the tools needed to parse a JSON file. Yet, the most common usage of json is to request the data from a website, download the data, and store it in a string.  Parsing, processing, and displaying the data are the last things we do. For such usage, we wouldn't use the _*json\_object\_from\_file*_ function. Admittedly we could: download the file and store it somewhere and then load the file into a JSON object. All this is unnecessary, wasteful, and slow. Json-c has a function to create a _*json\_object*_ from a C string:

- [json_object* json_tokener_parse(const char *str) ](https://json-c.github.io/json-c/json-c-0.15/doc/html/json__tokener_8h.html#a236ef64d079822a4411d13eae7190c4d)

The usage of this function is rather simple.

Consider [_*json-str00.c*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/json-str00.c):

```C
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

```C
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

```

