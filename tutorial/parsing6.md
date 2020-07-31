# Parsing a Json object - part 6: Iteration yet again

We have previously discussed two methods for iterating over _*json_type_object*_ objects, here we yet another one inspired from [graph theory](https://en.wikipedia.org/wiki/Graph_theory). We shall illustrate its usage with a very simple example. Recall program [_*json-array02.c*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/json-array02.c) demonstrating looping over all values in a JSON array. Now consider the program below, [_*json-array04*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/json-array04.c), which iterates through all the values in the JSON array _*names.json*_, printing each value and the index of the value in the array:

## json-array04.c

```C
#include <stdio.h>
#include <json-c/json.h>
#include <json_visit.h>

static int 
print_str(json_object *obj, int flags, json_object *parent, const char *key,
                       size_t *index, void *data)
{
  if (index)
     printf("The value at %ld position is: %s\n", (long)* index, json_object_get_string(obj));
  return JSON_C_VISIT_RETURN_CONTINUE;
}
```

The function that does all the work here is _*json_c_visit*_. The first thing to note is that we must include the _*json_visit.h*_ header file to use this function. Now for the specifics:

- [int json_c_visit(json_object *jso, int future_flags, json_c_visit_userfunc *userfunc, void *userarg)](https://json-c.github.io/json-c/json-c-0.15/doc/html/json__visit_8h.html)

From the documentation:

> Visit each object in the JSON hierarchy starting at jso. For each object, userfunc is called, passing the object and userarg. If the object has a parent (i.e. anything other than jso itself) its parent will be passed as parent_jso, and either jso_key or jso_index will be set, depending on whether the parent is an object or an array.
> 
> Nodes will be visited depth first, but containers (arrays and objects) will be visited twice, the second time with JSON_C_VISIT_SECOND set in flags.
> 
> userfunc must return one of the defined return values, to indicate whether and how to continue visiting nodes, or one of various ways to stop.
> 
> Returns 0 if nodes were visited successfully, even if some were intentionally skipped due to what userfunc returned. Returns <0 if an error occurred during iteration, including if userfunc returned JSON_C_VISIT_RETURN_ERROR.

For our example, [_*json-array04*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/json-array04.c), we call _*json_c_visit*_ on our JSON array, _*root*_ with a userfunc, _*print_str*_ which merely prints the array index and array value at that index. Note that the _*userfunc*_ is declared to be type _*json_c_visit_userfunc*_. This a typedef in the _*json_visit.h*_ header file:

```C
typedef int(json_c_visit_userfunc)(json_object *jso, int flags, json_object *parent_jso, const char *jso_key, size_t *jso_index, void *userarg)
```

The _*userfunc*_ must return one of the below defined return values:

- JSON_C_VISIT_RETURN_CONTINUE
- JSON_C_VISIT_RETURN_ERROR
- JSON_C_VISIT_RETURN_POP
- JSON_C_VISIT_RETURN_SKIP
- JSON_C_VISIT_RETURN_STOP

These return values determine how the path through the JSON continues. Notice our function _*print_str*_ returns _*JSON_C_VISIT_RETURN_CONTINUE*_ for all values visited. We wish to loop over the entire array.

Also note: the very first time _*print_str*_ is called it is called with the entire JSON array itself, and hence the argument _*jso_index*_ is NULL. We are not interested in printing this value or the JSON array itself so our _*print_str*_ function tests for this condition:

```C
if (index)
     printf("The value at %ld position is: %s\n", (long)* index, json_object_get_string(obj));
```

Using  _*json_c_visit*_ to loop over a JSON array as above is perhaps overkill. For production code, I would recommend using a simple loop as previously discussed. The _*json_c_visit*_ solution is provided merely for completeness sake and to introduce the subject with a rather easy to understand example.

## Depth-first search

The function _*json_c_visit*_ is best illustrated by considering an actual example involving a nonlinear graph. The array example above fails to illustrate the path _*json_c_visit*_ takes through a JSON document. Recall from the documentation:

> Visit each object in the JSON hierarchy starting at jso.
> Nodes will be visited depth first, but containers (arrays and objects) will be visited twice, the second time with JSON_C_VISIT_SECOND set in flags.

A more general example of a Depth-first search would take a more complex data structure. 

I am going to take the [Animated example of a depth-first search](https://en.wikipedia.org/wiki/Depth-first_search#/media/File:Depth-First-Search.gif) from [wikipedia](https://en.wikipedia.org/wiki/Depth-first_search). Illustrated below along with a JSON document with a similar structure:

<table>
<tr>
  <td><a href="https://github.com/rbtylee/tutorial-jsonc/blob/master/src/depth.json"> Json</a> </td> <td><a href="https://github.com/rbtylee/tutorial-jsonc/blob/master/Images/Depth-First-Search.gif">Graph</a> </td>
</tr>
<tr>
<td>

```json
{
  "1": {
    "2": {
      "3": {
        "4": 4
      }
    },     
    "5": {
        "6": {
          "7": 7
        },
        "8": 8
      },
      "9": {
        "10": 10
      }
  }
}
```
</td>
<td> <img src="https://github.com/rbtylee/tutorial-jsonc/blob/master/Images/Depth-First-Search.gif" width=200 height=200> </td>
</tr>
</table>

Now, the defintion of a JSON also from wikipedia:

> Depth-first search (DFS) is an algorithm for traversing or searching tree or graph data structures. The algorithm starts at the root node (selecting some arbitrary node as the root node in the case of a graph) and explores as far as possible along each branch before backtracking.

To properly illustrate a depth-first search, we note the function _*json_c_visit*_ visists container nodes twice. So in our _*json_c_visit_userfunc*_ we need to test for that and simply return as in:

```C
if (!parent || flags==JSON_C_VISIT_SECOND) return JSON_C_VISIT_RETURN_CONTINUE;
```
Note: we need to ignore the case where no parent JSON exists also. The case corresponds to the entire JSON document itself. It will be visisted both first and last as it also is a '_*container node*_'. 

Putting it all together [_*json-parse10.c*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/json-parse10.c) reads [_*depth.json*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/depth.json) and prints the key and value of each node in the a depth-first search:

## json-parse10.c

```C
#include <stdio.h>
#include <json-c/json.h>
#include <json_visit.h>

static int 
doit(json_object *obj, int flags, json_object *parent, const char *key,
                       size_t *index, void *data)
{
   if (!parent || flags==JSON_C_VISIT_SECOND) return JSON_C_VISIT_RETURN_CONTINUE;
   printf("key: %s, value: %s\n", key, json_object_to_json_string(obj));
   return JSON_C_VISIT_RETURN_CONTINUE;
}

int 
main(void)
{
   json_object *root = json_object_from_file("depth.json");
   json_c_visit(root, 0, doit, NULL);

   json_object_put(root);
   return 0;
}

```

Expected Output:

```
key: 1, value: { "2": { "3": { "4": 4 } }, "5": { "6": { "7": 7 }, "8": 8 }, "9": { "10": 10 } }
key: 2, value: { "3": { "4": 4 } }
key: 3, value: { "4": 4 }
key: 4, value: 4
key: 5, value: { "6": { "7": 7 }, "8": 8 }
key: 6, value: { "7": 7 }
key: 7, value: 7
key: 8, value: 8
key: 9, value: { "10": 10 }
key: 10, value: 10
```
## Serching a JSON

We have already discussed _*json_object_array_bsearch*_ which can be used to search a **sorted** JSON array, but what if we wish to search a JSON object array or otherwise for a particular key or value? And terminate the search once the key or value is found.

In [_*json-parse11.c*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/json-parse11.c) we wish to find the name, first and last, of the user with a particular email address, _*ptunkiny@angelfire.com*_ in the JSON document,[_*sample00.json*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/sample00.json).

This JSON consists of an array containing many records of the form:

```json
{
  "id": 1,
  "first_name": "Catie",
  "last_name": "Gregorin",
  "email": "cgregorin0@elegantthemes.com",
  "gender": "Female",
  "ip_address": "172.219.194.172"
}
```

As this JSON document is an array, one could use _*json_object_array_bsearch*_ to search the array with the right _*sort_fn*_. However, this has the disadvantage that the array must be sorted first. So consider [_*json-parse11.c*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/json-parse11.c) below:


## json-parse11.c

```C
#include <stdio.h>
#include <string.h>
#include <json-c/json.h>
#include <json_visit.h>

#define EMAIL "ptunkiny@angelfire.com"
#define JSON_OBJECT_STR(obj, key) json_object_get_string(json_object_object_get(obj, key))

static int 
doit(json_object *obj, int flags, json_object *parent, const char *key,
                       size_t *index, void *data)
{
   if (!parent || flags==JSON_C_VISIT_SECOND || 
       json_object_get_type(obj) == json_type_object || 
       json_object_get_type(obj) == json_type_array)
      return JSON_C_VISIT_RETURN_CONTINUE;
      
   if (strcmp(json_object_get_string(obj), EMAIL) == 0)
   {
      printf("Found: %s %s %s\n", JSON_OBJECT_STR(parent, "first_name"), 
             JSON_OBJECT_STR(parent, "last_name"), json_object_to_json_string(obj));
      return JSON_C_VISIT_RETURN_STOP;
   }
   return JSON_C_VISIT_RETURN_CONTINUE;
}

int 
main(void)
{
   json_object *root = json_object_from_file("sample00.json");
   printf("Search for %s\n\n", EMAIL);
   json_c_visit(root, 0, doit, NULL);

   json_object_put(root);
   return 0;
}

```

The code is fairly straightforward, note the usage of _*JSON_C_VISIT_RETURN_STOP*_ to stop _*json_c_visit*_ from visiting any further nodes in the JSON document.

From the [documentation](https://json-c.github.io/json-c/json-c-0.15/doc/html/json__visit_8h.html#a5956f41bed48f90a127f9b37fad7ea97):
> This json_c_visit_userfunc return value indicates that iteration should stop immediately, and cause json_c_visit to return success.

This is precisely what we wish to do at this step, terminate the search once we have found the required record.

## Problems

1. Rewrite _*json-parse11.c*_ to use _*json_object_array_bsearch*_.
2. Review _* JSON_C_VISIT_RETURN_POP*_ and _*JSON_C_VISIT_RETURN_SKIP*_ in the [docs](https://json-c.github.io/json-c/json-c-0.15/doc/html/json__visit_8h.html), can you think of any user case for these?
