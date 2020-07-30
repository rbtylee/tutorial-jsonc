# JSON Arrays: sorting and searching


Now that we have reviewed JSON arrays, let's examine the json-c libraries functions to sort and search arrays.

First to sort a JSON array use:
- [_*void json_object_array_sort(json_object \*jso, int(\*)(const void \*, const void \*) 	sort_fn)*_](https://json-c.github.io/json-c/json-c-0.14/doc/html/json__object_8h.html#a5584e2f2051cd1faa7fafd07ba888fd1)

This function is very much like the C library function - [_*qsort*_](https://linux.die.net/man/3/qsort): it relies upon the application code providing a comparison function, _*sort_fn*_. The comparison function behaves like the C function [_*strcmp*_](https://linux.die.net/man/3/strcmp).

From the documentation:
> Sorts the elements of jso of type json_type_array
>
> Pointers to the json_object pointers will be passed as the two arguments to sort_fn

An example of a _*sort_fn*_ for use with an array containing only intergers is found in the json-c program, [_*test1.c*_](https://github.com/json-c/json-c/blob/master/tests/test1.c):

```C
static int sort_fn(const void *j1, const void *j2)
{
	json_object *const *jso1, *const *jso2;
	int i1, i2;

	jso1 = (json_object *const *)j1;
	jso2 = (json_object *const *)j2;
	if (!*jso1 && !*jso2)
		return 0;
	if (!*jso1)
		return -1;
	if (!*jso2)
		return 1;

	i1 = json_object_get_int(*jso1);
	i2 = json_object_get_int(*jso2);

	return i1 - i2;
}

```

This is the general form all _*sort_fns*_ are going to take:

- Declare and initialize our variables.
- Test for NULL values.
- Get the values we need for our comparison from the two JSON objects.
- perform the needed comparison and return the result.

## json-array03.c

Consider a JSON array whose values are first names. [_*names.json*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/names.json). Our JSON has the form:

```json
["Torin", "Stefin", "Emerson", "Lisandro", "Caidan", ...]
```

This array is clearly unsorted. In json-c, sorting this array is as easy as:

```C
   json_object *root = json_object_from_file("names.json");
   json_object_array_sort(root, sort_fn);
```

provided one has implemented a _*sort_fn*_ that handles strings. A naive _*sort_fn*_ for such a case can be found in _*json-array03.c*_ below.

-  [json_object* json_object_array_bsearch(	const struct json_object *key, const struct json_object *jso, int(*)(const void *, const void *) sort_fn)](https://json-c.github.io/json-c/json-c-0.14/doc/html/json__object_8h.html#aed353084ed3ad84e7b7575afbe7e719d)

From the documentation:

> Binary search a sorted array for a specified key object.
>
> It depends on your compare function what's sufficient as a key. Usually you create some dummy object with the parameter compared in it, to identify the right item you're actually looking for.

Wrapping it all up here is a program to sort the JSON array _*names.json*_ and search for two keys, one key present and one key not present: [_*json-array03.c*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/json-array03.c)

```C
#include <stdio.h>
#include <json-c/json.h>
#include <string.h>

#define NUM_KEYS 2

static int
sort_fn(const void *j1, const void *j2)
{
   json_object *const *jso1, *const *jso2;
   const char *str1, *str2;

   jso1 = (json_object *const *) j1;
   jso2 = (json_object *const *) j2;

   if (!*jso1 && !*jso2)
       return 0;
   if (!*jso1)
       return -1;
   if (!*jso2)
       return 1;

   str1 = json_object_get_string(*jso1);
   str2 = json_object_get_string(*jso2);

   return strcmp(str1, str2);
}

int
main(void)
{
   json_object *str, *temp;
   char *keys[NUM_KEYS] = {"Vladimir", "Nathaniel"};

   json_object *root = json_object_from_file("names.json");
   printf("The json representation:\n\n%s\n\n", json_object_to_json_string_ext(root, JSON_C_TO_STRING_PRETTY));

   json_object_array_sort(root, sort_fn);
   printf("The json representation:\n\n%s\n\n", json_object_to_json_string_ext(root, JSON_C_TO_STRING_PRETTY));

   for(int i=0; i <NUM_KEYS; i++)
   {
     str = json_object_new_string(keys[i]);
     temp = json_object_array_bsearch(str, root, sort_fn);
     printf("Search for key '%s' yields: %s\n", keys[i], json_object_get_string(temp));
     json_object_put(str);
   }

   json_object_put(root);

   return 0;
}
```

Works as expected.

Remember I called the _*sort_fn*_ in _*json-array03.c*_ above **naive**. While this _*sort_fn*_ works as expected here, it is worth noting that it performs an ASCII sort using the C function, _*strcmp*_. This was sufficient for our case, but will produce some non-intuitive results for some strings. Read Jeff Atwood's blog, [Sorting for Humans : Natural Sort Order](https://blog.codinghorror.com/sorting-for-humans-natural-sort-order/).

To see this in action, modify [_*json-array03.c*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/json-array03.c) to sort the JSON array [_*atwood.json*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/atwood.json). It gives the same result as noted in Jeff's blog.

To fix this issue to produce more _*natural looking results*_ see Problem one below.

It should be noted that in many _*Real World™*_ uses of a JSON array, the array is more complex than a simple list of names or numbers. For example, consider a JSON array of the form:

```json
[
  {
    "age": 27,
    "name": "John Doe"
  },
  {
    "age": 59,
    "name": "Alice Duncan"
  },
  ...
]
```

Here we could have several possible  _*sort_fns*_, perhaps one to sort by name (last name  then first) or perhaps another to sort by age. The actual implementation of the  _*sort_fn*_ would more complex than the simple example given above.

## Problems

1. Modify [_*json-array03.c*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/json-array03.c) to sort the JSON array, [_*atwood.json*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/atwood.json) using Martin Pool's [_*strnatcmp function*_](https://github.com/sourcefrog/natsort) instead of _*strcmp*_. Does this produce the desired result? Would this solution be sufficient to use in **Production Code**? Explain why or why not.
2. Actually write the two  _*sort_fns*_ mentioned above, to sort by name (last name then first) and to sort by age. Expand the given JSON array above to include more similar JSON objects and write a program to test both your sort function implementations. Do you need to consider the '_*Natural sort order*_' issues mentioned in Jeff's Blog? What if a person has no last name? Or whose name had a suffix like _*Sr.*_ or _*Jr.*_? Or whose name was non-english and as a result had characters not found in english or ASCII? As you can see sorting can be a rather difficult problem. In Application code always consider what your users would expect.
3. Why do we need a _*json_object_put(temp) *_ in [_*json-array03.c*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/json-array03.c)?
4. In [_*json-array03.c*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/json-array03.c) we sort the JSON array first before doing the searches. What happens if we do not sort the array and then do the search? Does it work? Explain why?
