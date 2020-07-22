# JSON Arrays: sorting and searching

The json-c library provides some convenience functions so sort and perform a binary search of a JSON array. But first, let us review and expand upon what we currently know about JSON arrays and their implementation in the json-c library. As such JSON arrays resemble arrays in JavaScript or lists in Python.

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

In JSON, array values must be a valid JSON type, that is, of type string, number, object, array, boolean or null. Unlike arrays in C, JSON array values within an array do not all have to be the same type.

Recall our discussion in [JSON_types](https://github.com/rbtylee/tutorial-jsonc/blob/master/tutorial/types.md):

The json-c library has an enumerated type [_*json\_type*_](https://json-c.github.io/json-c/json-c-0.14/doc/html/json__types_8h.html) which can be used with the appropriate function to test a _*json_object*_ in order to determine its type in advance. We have the following '_types_' defined:

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

Consider _*json-array00.c*_:

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
