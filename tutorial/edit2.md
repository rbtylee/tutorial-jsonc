# JSON Pointers

Examining _*json-edit00.c*_, one may note the code for changing John's office phone number seems unduly complex:

```
   json_object *root = json_object_from_file("contact.json");
   
   json_object *phone_numbers =  _json_object_object_get(root, "phoneNumbers");
   json_object *office = json_object_array_get_idx(phone_numbers, 1);
   json_object *number = _json_object_object_get(office,"number");
   json_object_set_string(number, "843 276-3583");
```

Imagine how much more complex it would become in a JSON where objects are nested much more deeply. Surely there must be an easier way to access or modify JSON objects deeply nested within another JSON object. This is where JSON pointers come in: JSON pointer uses a string syntax for identifying a specific value within a JSON object.

Consider again our example, John's Office phone number. For the JSON represented by our _*contact.json*_ file, the string "/phoneNumbers/1/number" is a JSON pointer to the value of his office number.

```
{
  "firstName": "John",
  ...
  "phoneNumbers": [
    {
      "type": "home",
      "number": "212 555-1234"
    },
    {
      "type": "office",
      "number": "646 555-4567"
    }
  ],
...
}

```

 The '1' in the JSON pointer, "/phoneNumbers/1/number" is the array index, as the JSON object _*phoneNumbers*_ is an array. The rest of the string should be clear. For a full describtion of the JSON pointer standard, see [RFC 6901](https://tools.ietf.org/html/rfc6901). Don't worry it looks more complex than it is.
 
The json-c library, version 0.13 or later, has several functions which support this standard:
 
- int json_pointer_get(json_object *obj, const char *path, json_object **res)
- int json_pointer_getf(json_object *obj, json_object **res, const char *path_fmt,...)  

- int json_pointer_set(json_object **obj, const char *path, json_object *value)
- int json_pointer_setf(json_object **obj, json_object *value, const char *path_fmt,...) 


We have two sets of functions, one set to retrieve the value given the JSON pointer and one to set the value.

For more details on these functions, please consult the documentation.

So here is the original code to change the office phone number:

```
   json_object *root = json_object_from_file("contact.json");
   
   json_object *phone_numbers =  _json_object_object_get(root, "phoneNumbers");
   json_object *office = json_object_array_get_idx(phone_numbers, 1);
   json_object *number = _json_object_object_get(office,"number");
   json_object_set_string(number, "843 276-3583");
```

and here is the JSON pointer version:

```
json_object *root = json_object_from_file("contact.json");

json_pointer_set(&root, "/phoneNumbers/1/number", json_object_new_string("843 276-3583"));

```

As you can see the usage of these functions is pretty clear. If you need more examples look at the file _*test_json_pointer.c*_ in the source code itself.


