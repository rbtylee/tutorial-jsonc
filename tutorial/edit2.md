# JSON Pointers

Examining [_*json-edit00.c*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/json-edit00.c), one may note the code for changing John's office phone number seems unduly complex:

```C
   json_object *root = json_object_from_file("contact.json");
   
   json_object *phone_numbers =  json_object_object_get(root, "phoneNumbers");
   json_object *office = json_object_array_get_idx(phone_numbers, 1);
   json_object *number = json_object_object_get(office,"number");
   json_object_set_string(number, "843 276-3583");
```

Imagine how much more complex it would become in a JSON where objects are nested much more deeply. Surely there must be an easier way to access or modify JSON objects deeply nested within another JSON object. This is where JSON pointers come in: a JSON pointer uses a string syntax for identifying a specific value within a JSON object.

Consider again our example, John's Office phone number. For the JSON represented by our _*contact.json*_ file, the string "_*/phoneNumbers/1/number*_" is a JSON pointer to the value of his office number.

```json
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

 The '1' in the JSON pointer, "_*/phoneNumbers/1/number*_" is the array index, as the JSON object _*phoneNumbers*_ is an array. The rest of the string should be clear. For a full describtion of the JSON pointer standard, see [RFC 6901](https://tools.ietf.org/html/rfc6901). Don't worry it looks more complex than it is.
 
The json-c library, version 0.13 or later, has several functions which support this standard:
 
- [int json_pointer_get(json_object *obj, const char *path, json_object **res)](https://json-c.github.io/json-c/json-c-0.14/doc/html/json__pointer_8h.html#aff88937e32b0ba6ffbd07cb4b1919053)
- [int json_pointer_getf(json_object *obj, json_object **res, const char *path_fmt,...)](https://json-c.github.io/json-c/json-c-0.14/doc/html/json__pointer_8h.html#af0ac03df64b215d05041e8007ed0233d) 

- [int json_pointer_set(json_object **obj, const char *path, json_object *value)](https://json-c.github.io/json-c/json-c-0.14/doc/html/json__pointer_8h.html#aef0e651f63ce5ce35648503705e2586b)
- [int json_pointer_setf(json_object **obj, json_object *value, const char *path_fmt,...)](https://json-c.github.io/json-c/json-c-0.14/doc/html/json__pointer_8h.html#a66f1f98a2ce085c19f6750193b4c726d)


We have two sets of functions, one set to retrieve the value given the JSON pointer and one to set the value.

For more details on these functions, please consult the [documentation](https://json-c.github.io/json-c/json-c-0.14/doc/html/json__pointer_8h.html).

So here is the original code to change the office phone number:

```C
   json_object *root = json_object_from_file("contact.json");
   
   json_object *phone_numbers =  json_object_object_get(root, "phoneNumbers");
   json_object *office = json_object_array_get_idx(phone_numbers, 1);
   json_object *number = json_object_object_get(office,"number");
   json_object_set_string(number, "843 276-3583");
```

and here is the JSON pointer version:

```C
json_object *root = json_object_from_file("contact.json");

json_pointer_set(&root, "/phoneNumbers/1/number", json_object_new_string("843 276-3583"));

```

As you can see the usage of these functions is pretty clear. If you need more examples look at the file [_*test_json_pointer.c*_](https://github.com/json-c/json-c/blob/master/tests/test_json_pointer.c) in the json-c source code itself.


