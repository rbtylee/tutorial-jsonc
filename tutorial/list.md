# JSON Arrays: basics

The json-c library provides some convenience functions to sort and perform a binary search of a JSON array. But first, let us review and expand upon what we currently know about JSON arrays and their implementation in the json-c library. As such JSON arrays resemble arrays in JavaScript or lists in Python.

According to [The JSON Data Interchange Standard](http://www.ecma-international.org/publications/files/ECMA-ST/ECMA-404.pdf):
> An array structure is a pair of square bracket tokens surrounding zero or more values. The values are separated by commas. The JSON syntax does not define any specific meaning to the ordering of the values. However, the JSON array structure is often used in situations where there is some semantics to the ordering.

A few valid JSON arrays:

```
["foobar", "foo", "bar"]
```

```
["foo", "bar", 32, null]
```

```
[
   "firstName": "John",
   "lastName": "Smith",
   {
      "type": "home",
      "number": "212 555-1234"
    },
    {
      "type": "office",
      "number": "646 555-4567"
    },
    true
]
```

## Arrays as JSON Objects

In JSON, array values must be a valid JSON type, that is, of type string, number, object, array, boolean or null. Unlike arrays in C, JSON array values within an array do not all have to be the same type. However, in json-c JSON array values are all the same type, they are all _*json_objects*_. But the _*json_objects*_ an array contains in json-c have different _*json_types*_ corresonding to the C version of the types string, number, object, array, boolean or null.

Recall our discussion in [JSON_types](https://github.com/rbtylee/tutorial-jsonc/blob/master/tutorial/types.md):

The json-c library has an enumerated type [_*json_type*_](https://json-c.github.io/json-c/json-c-0.14/doc/html/json__types_8h.html) which can be used with the appropriate function to test a _*json_object*_ in order to determine its type in advance. We have the following '_types_' defined:

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

### json-array00.c

An important thing to note is a JSON array by itself is valid JSON.

Consider [_*json-array00.c*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/json-array00.c):

```
#include <stdio.h>
#include <json-c/json.h>

int
main(void)
{
   json_object *root = json_tokener_parse("[\"foo\", \"bar\", 32, null]");
   printf("The json string:\n\n%s\n\n", json_object_to_json_string(root));
   
   printf("The json object to string:\n\n%s\n", json_object_to_json_string_ext(root, JSON_C_TO_STRING_PRETTY));

   json_object_put(root);

   return 0;
}
```

## Accessing and Deleting Array Values

You access the array values by using the value's index number. Json-c provides two functions to access a JSON arrays value, one to get the value, and one to set the value as well as a function to delete the value (or values) at a specified index:

- [json_object\* json_object_array_get_idx(json_object \*obj, int idx)](https://json-c.github.io/json-c/json-c-0.14/doc/html/json__object_8h.html#a676711a76545d4ec65cc75f100f5fd19)
- [int json_object_array_put_idx(json_object *obj, size_t idx, json_object *val)](https://json-c.github.io/json-c/json-c-0.14/doc/html/json__object_8h.html#a1ac0ccdbc13a25da7d8b2dc9e421dfad)
- [int json_object_array_del_idx(json_object *obj, size_t idx, size_t count)](https://json-c.github.io/json-c/json-c-0.14/doc/html/json__object_8h.html#a722eca9f578704d3af38b97549242c1f)

### json-array01.c

Notice that the _*json_object_array_get_idx*_ returns the _*json_object*_ at the specified index. For example, if json_object_array_get_idx(root, 2) returns a _*json_object*_ of type _*json_type_int*_, to actually access the integer one has to use:

```
i = json_object_get_int(json_object_array_get_idx(root, 2))
```

Likewise, we can not set a JSON array value at a specific index unless the value is a _*json_object*_.  For example, we have:

```
json_object_array_put_idx(root, 2, json_object_new_string("foobar"))
```
and not:

```
json_object_array_put_idx(root, 2, "foobar")
```
If one tries the latter, the program will compile with a warning and if executed crash:

```
Segmentation fault (core dumped)
```
Also as per the documentation, unlike a C array, one can use _*json_object_array_put_idx*_ to set an index bigger than the length of the current array:

> The array size will be automatically be expanded to the size of the index if the index is larger than the current size.

In this instance, all of the missing array values that will be added during the expansion of the array are NULL (_*json_type_null*_)

To illustrate the usage of these functions, [_*json-array01.c*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/json-array01.c):

```
#include <stdio.h>
#include <json-c/json.h>

int
main(void)
{
   json_object *root = json_tokener_parse("[\"foo\", \"bar\", 32, null]");

   printf("The json representation:\n\n%s\n\n", json_object_to_json_string_ext(root, JSON_C_TO_STRING_PRETTY));

   int val = json_object_get_int(json_object_array_get_idx(root, 2));
   printf("The third value of the array is: %d\n\n", val);

   printf("Modifying the third value and deleting the fourth.\n\n");
   json_object_array_put_idx(root, 2, json_object_new_string("foobar"));
   json_object_array_del_idx(root, 3, 1);

   printf("The json representation is now:\n\n%s\n\n", json_object_to_json_string_ext(root, JSON_C_TO_STRING_PRETTY));

   json_object_put(root);

   return 0;
}

```

**Note:** in the section [JSON Pointers](https://github.com/rbtylee/tutorial-jsonc/blob/master/tutorial/edit2.md) we will see yet another method for accessing an arrays value at a specified index.

## Looping Through an Array

Recall the function _*doarray*_ in program [_*json-parse07.c*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/json-parse07.c). Note this function loops through the given JSON array in the same fashion as one loops thru a C array.

To illustrate in slightly simpler code, [_*json-array02.c*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/json-array02.c):

```
#include <stdio.h>
#include <json-c/json.h>

int
main(void)
{
   const char *str;
   json_object *root = json_tokener_parse("[\"foo\", \"bar\", 32, null]");

   printf("The json representation:\n\n%s\n\n", json_object_to_json_string_ext(root, JSON_C_TO_STRING_PRETTY));

   int n = json_object_array_length(root);
   for (int i=0;i<n; i++)
   {
      str= json_object_get_string(json_object_array_get_idx(root, i));
      printf("The value at %i position is: %s\n", i, str);
   }
   
   json_object_put(root);

   return 0;
}
```

