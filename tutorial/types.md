# json_types

As we have seen, extracting a string value for a _*json\_object*_ is rather easy. While we haven't considered it yet, the same holds when the value is a number, boolean, or a null value. Things get more complex, however, if the JSON value is another _*json_object*_ or a _*json_array*_. However, to fully parse our _*contact.json*_ file, we must be able to handle the attributes with values that are either _*json_objects*_ or json arrays.

Review the state diagram for JSON values again:

!["JSON state diagram for values"](https://github.com/rbtylee/tutorial-jsonc/blob/master/Images/value.png)

The json-c library has an enumerated type _*json\_type*_ which can be used with the appropriate function to test a _*json_object*_ in order to determine its type in advance. We have the following '_types_' defined:

- json_type_null
- json_type_boolean
- json_type_double
- json_type_int
- json_type_object
- json_type_array
- json_type_string

Note, in the JSON standards we only have one numeric type defined, _number_. Whereas in C, a [_typed_](https://en.wikipedia.org/wiki/Strong_and_weak_typing) language, we need to distinguish between ints, floats, doubles, et al. Json-c therefore distinguishes between _*json_type_int*_ and _*json_type_double*_. Furthermore, since the JSON standards have two boolean types, _true_ and _false_, and C completely lacks a boolean type, the type _*json_type_boolean*_ is in reality, an *int* in the json-c representation. Remember we have:

```
typedef int json_bool;
```

in the json-c header files.

It should also be noted that in the json-c library the value of json_type_int is a 64-bit value internally. In C, this corresponds to the type *int64\_t*. Some readers may be unfamiliar with the types, *int32\_t* and *int64\_t*. These "new" types are typedefs you can in the C header file *<stdint.h>*. They were introduced into the C language by the [C99 standard](https://en.wikipedia.org/wiki/C99). These are exact-width integer types that are guaranteed to have the same number of bits across all implementations. So *int32\_t* is a 32 bit integer while *int64\_t* is 64 bits. In general, it is safest to always use the int64 functions in the json-c library.

We have two functions in the json-c library to help us determine the type of a _*json_object*_:

- int json_object_is_type(json_object \*obj, json_type type)
- json_type json_type json_object_get_type(json_object \*obj)

We have seen the usage of the function _*json_object_get_string*_, json-c naturally also provides the functions:

- json_bool     json_object_get_boolean(json_object *obj)
- double        json_object_get_double(json_object *obj)
- int32_t       json_object_get_int(json_object *obj)
- int64_t       json_object_get_int64(json_object *obj)
- lh_table*     json_object_get_object(json_object *obj)
- array_list *  json_object_get_array(json_object *obj)

One can also see we have two new types:

- lh_table
- array_list

Both of course *structs* defined in the _*json_object.h*_ header file. For now, we can safely ignore these types as the details are not important here. Curious users can read the docs or look up the definitions in the header file. More details on these functions can be found in the json-c documentation. 

Now, let's demonstrate some of these function's usage with our _*contact.json*_ file example:

```
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

```

Save (_*json-type00.c*_) above, compile, and execute. Compare your results with the original _*contact.json*_ file to ensure the results make sense.

## Problems

1. In any language, write an implementation of a state machine to determine whether a JSON value is valid. Consult the state diagram for a JSON value above. You are free to use a state machine library. However, for such a simple example it is hardly necessary.

- If you choose a language other than C for Problem 1, explain why? Explain how difficult would it be to implement your non-C solution or something akin to it in C? If it is possible for you to do so in a reasonable amount of time, implement your non-C solution in C.

- If you solved problem 1. in C, try it again in a non-C language. Preferably a 'popular' well-known one.
