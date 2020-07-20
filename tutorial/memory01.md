# Memory management, ownership and references - part two: examples

If you are feeling somewhat confused by the previous discussion, this section will consider some concrete examples to illustrate the concepts of _*ownership*_ and _*references*_. To best utilize this section, one needs to be somewhat familiar with using to the tool _*Valgrind*_.

Recall the previous statement:
>You may think that for every json_object_get there should eventually follow a json_object_put, much like the classic c way of following an malloc with a free at some latter point. However, we will see this is not always the case.

The example below shows a user case where one reuses the same JSON object by adding it several times to the same JSON document.

Here is the JSON document we wish to create:

```
{
  "items":[
    {
      "id":3201,
      "status":"online"
    },
    {
      "id":4678,
      "status":"online"
    },
    {
      "id":2345,
      "status":"online"
    }
  ]
}
```

We see the field _*"status": "online"*_ occurs three times in this JSON. Suppose we just want to create one JSON object for this attribute/pair instead of using three separate JSON objects with the same content. Examine [_*json-mem00.c*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/json-mem00.c) which follows.

First, create the needed JSON string:

```
json_object *str =  json_object_new_string("online");
```

At this point we _*own*_ the object _*str*_ and its reference count is 1.

Next, add it the first item in the JSON array _*items*_:

```
json_object_object_add(item, "status", str);
```

After adding _*str*_ to the JSON object _*item*_, _*item*_ now _*owns*_ our JSON string, _*str*_ and _*str*_ still has a reference count of 1. We no longer own _*str*_. 

The documentation for _*json_object_object_add*_ states:
> Upon calling this, the ownership of val transfers to obj. Thus you must make sure that you do in fact have ownership over this object.

So before we can add _*str*_ to the remaining items in the JSON array _*items*_, we need to re-acquire _*ownership*_ of the JSON object. That is, we need a _*json_object_get(str)*_ statement before we try to add _*str*_ to another JSON object. So the remaining two times we add _*str*_ to an item in the _*items*_ array we must use:

```
   json_object_get(str);
   json_object_object_add(item, "status", str);
```

Each time we do this, the reference count for _*str*_ will be incremented.
Hence right before the program exits and immediately before the _*json_object_put(root)*_ statement, _str*_ has a reference count of 3. One for each time it was used in our JSON document.

The statement _*json_object_put(root)*_ decrements the reference count of the JSON object _*root*_. Since _*root*_ has only been referenced once in our program by the initial _*json_object *root = json_object_new_object()*_, the reference count for _*root*_ goes to 0. Hence, _*root*_ is freed and all objects owned by root have their reference count decremented and those with a reference count of zero are also freed. The result is everything contained within _*root*_ is freed.

Though you won't have explicit, individual calls to _*json_object_put()*_ in the application code, when _*json_object_put(root)*_ is executed the json-c library _*internally*_ performs roughly the following steps:

* json_object_put(items)
     * json_object_put(item <1st one>)
         * json_object_put(<json_type_int with value 3201>)
         * json_object_put(str) (refcount decremented, not yet freed)
     * json_object_put(item <2nd one>)
         * json_object_put(<json_type_int with value 4678>)
         * json_object_put(str) (refcount decremented, not yet freed)
     * json_object_put(item <3rd one>)
         * json_object_put(<json_type_int with value 2345>)
         * json_object_put(str) (refcount decremented, now it's 0, so it is freed)

**Note:** at no point did we in the application code have to _*json_object_put(str)*_ explicitly, as the management of _*str's*_ memory was handled by the json-c library when the _*json_object_put(root)*_ statement was executed. So as promised, we can see why it is not necessarily the case that for every _*json_object_get*_ there should eventually follow a _*json_object_put*_. It all depends upon the _*ownership*_ of the object.

What about the usage of _*str*_ after the execution of _*json_object_put(root)*_? The pointer _*str*_ is no longer valid. Trying to use it after the json_object_put statement is a _*use-after-free error*_, exactly as if one attempted to do:
```
char \*str = malloc(10); free(str); \*str = 'x';.
```

## json-mem00.c

```
#include <stdio.h>
#include <json-c/json.h>

int
main(void)
{
   json_object *items, *item, *str;
   
   json_object *root = json_object_new_object();
   if (!root)
      return 1;

   // main array
   items = json_object_new_array();
   json_object_object_add(root, "items", items);
   //       item 1
   item = json_object_new_object();
   json_object_object_add(item, "id", json_object_new_int(3201));
   str =  json_object_new_string("online");
   json_object_object_add(item, "status", str);
   json_object_array_add(items, item);

   //       item 2
   item = json_object_new_object();
   json_object_object_add(item, "id", json_object_new_int(4678));
   json_object_get(str);
   json_object_object_add(item, "status", str);
   json_object_array_add(items, item);
   //       item 3
   item = json_object_new_object();
   json_object_object_add(item, "id", json_object_new_int(2345));
   json_object_get(str);
   json_object_object_add(item, "status", str);
   json_object_array_add(items, item);
   
   printf("The json representation:\n\n%s\n\n", json_object_to_json_string_ext(root, JSON_C_TO_STRING_PRETTY));

   // cleanup and exit
   json_object_put(root);
   return 0;
}
```

As usual, compile and execute this program. Run the binary through Valgrind to ensure there are no memory errors.

Now experiment with removing one or more of the _*json_object_get(str)*_ statements in _*json-mem00.c*_. Does the code compile? Does it run? What does valgrind report?


## json-mem01.c

There's an alternate approach:  call _*json_object_get(str)*_ before *each* of the _*json_object_object_add(item, "status", str)*_ calls, even the first one, and then add a _*json_object_put(str)*_ at the end.  In a more complicated program, one where maybe the code to add each item is factored out into a separate function, this can result in a more natural flow.

To illustrate this approach examine [_*json-mem01.c*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/json-mem01.c) below, each new item is added to the items array in a seperate function, _*json_object \*new_item(int id, json_object \*str)*_. The function _*new_item*_ ensures it has ownership of _*str*_ when adding _*str*_ to the JSON object _*item*_ by calling _*json_object_get(str)*_. 

```
#include <stdio.h>
#include <json-c/json.h>

#define NUM_ID_ITEMS 3

json_object *
new_item(int id, json_object *str)
{
   json_object *item = json_object_new_object();
   json_object_object_add(item, "id", json_object_new_int(id));
   json_object_object_add(item, "status", json_object_get(str));
   return item;
}

int
main(void)
{
   json_object *items, *str;
   int id[3] = {3201, 4678, 2345};

   json_object *root = json_object_new_object();
   if (!root)
      return 1;

   // main array
   items = json_object_new_array();
   json_object_object_add(root, "items", items);
   str = json_object_new_string("online");

   // Add Items
   for(int i=0; i<NUM_ID_ITEMS; i++)
      json_object_array_add(items, new_item(id[i], str));

   printf("The json representation:\n\n%s\n\n", json_object_to_json_string_ext(root, JSON_C_TO_STRING_PRETTY));

   // cleanup and exit
   json_object_put(str);
   json_object_put(root);
   return 0;
}

```

As before, compile and execute this program. Run the binary through Valgrind to ensure there are no memory errors.

Other cases where one might need to increase the reference count of an object via _*json_object_get*_ are:

> - Using an object field or array index (retrieved through _*json_object_object_get*_ or _*json_object_array_get_idx*_) beyond the lifetime of the parent object.
> - Detaching an object field or array index from its parent object (using _*json_object_object_del*_ or _*json_object_array_del_idx*_) 
> - Sharing a json_object with multiple (not necesarily parallel) threads of execution that all expect to free it (with _*json_object_put*_) when they're done.
 
