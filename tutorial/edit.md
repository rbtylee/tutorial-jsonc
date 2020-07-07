# Editing JSON and more

In working with JSON, it may be the case one needs to edit an existing JSON. If it is a file, one can edit it in a text editor or by using classic Unix tools, sed et al. But it may be the case that the application you are working on needs to modify the JSON itself, whether the JSON is a file or merely an internal representation.

Consider once again our _*contact.json*_, suppose John Smith got married to a beautiful and adopted his her daughter, plus his office phone number change and he had a birthday. Clearly, our JSON for John needs to be updated.

Let's approach changing these values in the JSON in the order they appear in the JSON file. 

Since we are merely incrementing John's age, json-c fortunately provides an easy to use function for that:

- int json_object_int_inc(json_object \*obj, int64_t val)

The variable value can be negative. Therefore, this function can also decrement. Hence:

```
   json_object *root = json_object_from_file("contact.json");
   json_object *age = _json_object_object_get(root, "age");
   json_object_int_inc(age, 1);
```

Next, we consider changing the office phone number in the JSON. Remember the _*phoneNumbers*_ attribute is a JSON array of two JSON objects:

```
  "phoneNumbers": [
    {
      "type": "home",
      "number": "212 555-1234"
    },
    {
      "type": "office",
      "number": "646 555-4567"
    }
  ]
```

We wish to change the second JSON objects number attribute's string to a new number, this array member has index 1. Assume the new number is _843 276-3583_.

The json-c library provides a set of functions of the form: json\_object\_set\_*. Such as  the one we need for this case:

- int json_object_set_string(json_object \*obj, const char \*new_value)

Our code snippet then looks like:

```
   json_object *root = json_object_from_file("contact.json");
   
   json_object *phone_numbers =  _json_object_object_get(root, "phoneNumbers");
   json_object *office = json_object_array_get_idx(phone_numbers, 1);
   json_object *number = _json_object_object_get(office,"number");
   json_object_set_string(number, "843 276-3583");
```

And works as expected.

Now we add the daughter. Recall the _*children*_ attribute of our JSON is also an array but in this case, it is empty:


```
"children": []
```

Let the adopted daughter's name be _*Paige Crawford*_, age 15. As expected, the below snippet handles this case with ease:

```
   json_object *root = json_object_from_file("contact.json");

   json_object *children =  _json_object_object_get(root, "children");
   json_object *paige = json_object_new_object();
   json_object_object_add(paige, "name", json_object_new_string("Paige Crawford"));
   json_object_object_add(paige, "age", json_object_new_int(15));
   json_object_array_add(children, paige);
```

Finally, let's add John's wife. If her name is Kate Smith, then you may think the problem would be as simple as:

```
   json_object *root = json_object_from_file("contact.json");
   json_object *spouse = _json_object_object_get(root, "spouse");
   json_object_set_string(spouse, "Kate Smith");
```

But no, our JSON is unchanged by this. The function _*json_object_set_string*_ behaves just like the related function:

- int json_object_set_string_len(json_object \*obj, const char \*new_value, int len)

In both cases, the JSON object type is checked and if it is not a _*json_type_string*_, 0 is returned and nothing is changed. If the type of the object is _*json_type_string*_, the object value is changed to new_value.

Makes sense. But here it is a minor complication, solvable in this case by deleting the object first and then re-adding it:

```
   json_object *root = json_object_from_file("contact.json");
   json_object *spouse = _json_object_object_get(root, "spouse");
   json_object_object_del(root,"spouse");
   json_object_object_add(root,"spouse",json_object_new_string("Kate Smith"));
```

Notice the introduction of the self-explanatory function:

- void 	json_object_object_del (json_object \*obj, const char \*key)

## json-edit00.c

Wrapping it up, our complete program:

```
#include <stdio.h>
#include <json-c/json.h>

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
   json_object *root = json_object_from_file("contact.json");
   
   printf("The json file:\n\n%s\n", json_object_to_json_string_ext(root, JSON_C_TO_STRING_PRETTY));
   // Edit age
   json_object *age = _json_object_object_get(root, "age");
   json_object_int_inc(age, 1);

   // Edit office phone number
   json_object *phone_numbers =  _json_object_object_get(root, "phoneNumbers");
   json_object *office = json_object_array_get_idx(phone_numbers, 1);
   json_object *number = _json_object_object_get(office,"number");
   json_object_set_string(number, "843 276-3583");
   // Add Daughter
   json_object *children =  _json_object_object_get(root, "children");
   json_object *paige = json_object_new_object();
   json_object_object_add(paige, "name", json_object_new_string("Paige Crawford"));
   json_object_object_add(paige, "age", json_object_new_int(15));
   json_object_array_add(children, paige);
   
   // Add Wife
   json_object *spouse = _json_object_object_get(root, "spouse");
   json_object_object_del(root,"spouse");
   json_object_object_add(root,"spouse",json_object_new_string("Kate Smith"));
   printf("The Edited JSON:\n\n%s\n", json_object_to_json_string_ext(root, JSON_C_TO_STRING_PRETTY));
   

   json_object_put(root);
   return 1;
}

```

Expected output:

```
$ ./json-edit00
The json file:

{
  "firstName":"John",
  "lastName":"Smith",
  "isAlive":true,
  "age":27,
  "address":{
    "streetAddress":"21 2nd Street",
    "city":"New York",
    "state":"NY",
    "postalCode":"10021-3100"
  },
  "phoneNumbers":[
    {
      "type":"home",
      "number":"212 555-1234"
    },
    {
      "type":"office",
      "number":"646 555-4567"
    }
  ],
  "children":[
  ],
  "spouse":null
}

The Edited JSON:

{
  "firstName":"John",
  "lastName":"Smith",
  "isAlive":true,
  "age":28,
  "address":{
    "streetAddress":"21 2nd Street",
    "city":"New York",
    "state":"NY",
    "postalCode":"10021-3100"
  },
  "phoneNumbers":[
    {
      "type":"home",
      "number":"212 555-1234"
    },
    {
      "type":"office",
      "number":"843 276-3583"
    }
  ],
  "children":[
    {
      "name":"Paige Crawford",
      "age":15
    }
  ],
  "spouse":"Kate Smith"
}

```

Problem:

1. Look over the _*json_object.h*_ header file or the documentation for other functions of the form: json\_object\_set\_* for the remaining JSON types. Write some sample code and if need sample JSON files, shwing how to use a few of these not adressed here.