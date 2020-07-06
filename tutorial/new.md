# Creating and saving a JSON object

It is perhaps traditional when discussing an Object Oriented library to first introduce creating and deleting a new object. But, we have taken an ass backwards appproach: Introducing initializing a json\_object from a file or string first and then going start to parsing the JSON data contained within. Now it is time to introducing creating a new JSON object and adding data to it and saving it as a file.

## Json Constructors

Not surprisingly, the constructor for a new _*json_object*_ is:

- json_object\* json_object_new_object(void)

The documentation says this function creates:

>a new empty object with a reference count of 1. The caller of this object initially has sole ownership. Remember, when using json_object_object_add or json_object_array_put_idx, ownership will transfer to the object/array. Call json_object_get if you want to maintain shared ownership or also add this object as a child of multiple objects or arrays. Any ownerships you acquired but did not transfer must be released through json_object_put.


Consider:

```
#include <stdio.h>
#include <json-c/json.h>

int
main(void)
{
   json_object *root = json_object_new_object();
   printf("The json representation:\n\n%s\n\n", json_object_to_json_string_ext(root, JSON_C_TO_STRING_PRETTY));

   json_object_put(root);
   return 1;
}
```

The expected output of the above program:

```
$ ./json-shit 
The json representation:

{
}

```

After creating a _*json\_object*_, you usually intialize it the data you want to store in it. Remember the data is always in name/value pairs where name is a string and value is another _*json_object*_ of a particular _*json_type*_. The json-c function for this operation is:

- void json_object_object_add(json_object *obj, const char * key, json_object *val)


Consider the code snipplet:

```
    json_object *root = json_object_new_object();
    json_object_object_add(root, "id", NULL);
```

This creates the JSON:

```
{
  "id":null
}
```

This examples covers the user case of adding a key with _*json\_type\_null*_ value to a *_json\_object*_.

Functions for adding other *_json\_types*_ are listed below and there usage should be intuitively obvious:

1. json_object\* json_object_new_boolean(json_bool b)
- json_object\* json_object_new_double(double d)
- json_object\* json_object_new_int(int32_t i)
- json_object\* json_object_new_int64(int64_t i)
- json_object\* json_object_new_object(void)
- json_object\* json_object_new_array(void)
- json_object\* json_object_new_string(const char \*s)
- json_object\* json_object_new_string_len(const char \*s, int len)

## Adding elements to json arrays

- int json_object_array_add(json_object \* obj, json_object \* val)

This function returns 0 on success and -1 on failure.

## json-new00.c

As an example of using these functions along with a new function to write the JSON to your disk, consider the program, _*json-new00.c*_, below. This program creates a new contact JSON file with the same JSON structure as our _*contact.json*_. It prints the JSON and saves it to the hard drive.

```
#include <stdio.h>
#include <json-c/json.h>

int
main(void)
{
   const char *filename = "contact2.json";
   json_object *root = json_object_new_object();
   if (!root)
      return 0;

   // basic data
   json_object_object_add(root, "firstName", json_object_new_string("Bart"));
   json_object_object_add(root, "lastName", json_object_new_string("Johnson"));
   json_object_object_add(root, "isAlive", json_object_new_boolean(TRUE));
   json_object_object_add(root, "age", json_object_new_int(57));
   // address json
   json_object *address = json_object_new_object();
   json_object_object_add(address, "streetAddress", json_object_new_string("105 Murdock"));
   json_object_object_add(address, "city", json_object_new_string("Princeton"));
   json_object_object_add(address, "state", json_object_new_string("WV"));
   json_object_object_add(address, "postalCode", json_object_new_string("24740"));
   json_object_object_add(root, "address", address);
   // phone numbers array
   json_object *phone_numbers = json_object_new_array();
   json_object_object_add(root, "phoneNumbers", phone_numbers);
   //       home
   json_object *phone_home = json_object_new_object();
   json_object_object_add(phone_home, "type", json_object_new_string("home"));
   json_object_object_add(phone_home, "number", json_object_new_string("304-888-5686"));
   json_object_array_add(phone_numbers, phone_home);
   //       cell
   json_object *phone_cell = json_object_new_object();
   json_object_object_add(phone_cell, "type", json_object_new_string("cell"));
   json_object_object_add(phone_cell, "number", json_object_new_string("304-888-3023"));
   json_object_array_add(phone_numbers, phone_cell);

   // children array
   json_object *children = json_object_new_array();
   json_object_object_add(root, "children", children);
   //       daughter
   json_object *justine = json_object_new_object();
   json_object_object_add(justine, "name", json_object_new_string("Justine Johnson"));
   json_object_object_add(justine, "age", json_object_new_int(29));
   json_object_array_add(children, justine);

   // spouse
   json_object_object_add(root, "spouse", json_object_new_string("Bobbi Johnson"));

   // print json
   printf("The json representation:\n\n%s\n\n", json_object_to_json_string_ext(root, JSON_C_TO_STRING_PRETTY));
   // save json
   if (json_object_to_file(filename, root))
      printf("Error: failed to save %s!!\n", filename);
   else
      printf("%s saved.\n", filename);

   // cleanup and exit
   json_object_put(root);
   return 1;
}

```

Expected output:

```
$ ./json-new00 
The json representation:

{
  "firstName":"Bart",
  "lastName":"Johnson",
  "isAlive":true,
  "age":57,
  "address":{
    "streetAddress":"105 Murdock",
    "city":"Princeton",
    "state":"WV",
    "postalCode":"24740"
  },
  "phoneNumbers":[
    {
      "type":"home",
      "number":"304-888-5686"
    },
    {
      "type":"cell",
      "number":"304-888-3023"
    }
  ],
  "children":[
    {
      "name":"Justine Johnson",
      "age":29
    }
  ],
  "spouse":"Bobbi Johnson"
}

contact2.json saved.

```

By now you know what to do here ...

## Problems

1. What happens if _*json\_object \*address*_ is NULL in the above code, _*json-new00.c*_? Refactor the code above to prevent all such unlikely but serious cases of that happening.
