# Parsing a Json object - part 5: Iterators

We introduced the function _*json_object_object_foreach*_ previously. However, json-c offers another iteration API more in a C++ iterator type style.

Consider our previous example: 

```C
#include <stdio.h>
#include <json-c/json.h>

int 
main(void)
{
   json_object *root = json_object_from_file("contact.json");
   json_object_object_foreach(root, key, val)
      printf("%s  -> %s\n", key, json_object_get_string(val));

   json_object_put(root);
   return 0;
}

```

If we refactor the above by replacing essentially implementing the macro _*json_object_object_foreach*_, we have:

```C
#include <stdio.h>
#include <json-c/json.h>

int 
main(void)
{
   struct lh_entry *entry; // Struct needed here
   char *key;
   json_object *root = json_object_from_file("contact.json");
   entry = json_object_get_object(root)->head;
   while( entry )
   {
       key = (char *)entry->k;
       json_object* val = (json_object *) entry->v;
       printf("%s  -> %s\n", key, json_object_get_string(val));
       entry = entry->next;
   }
   json_object_put(root);
   return 0;
}
```

Examining the above code, we are violating the Object-Oriented principle of [encapsulation](https://en.wikipedia.org/wiki/Encapsulation_(computer_programming)) and exposing the json-c internal representation of the _*json_object*_. This is not as apparent when using the macro _*json_object_object_foreach*_, yet even then the programmer has access to the internal struct lh_entry:

```C
#include <stdio.h>
#include <json-c/json.h>

int 
main(void)
{
   json_object *root = json_object_from_file("contact.json");
   json_object_object_foreach(root, key, val)
      printf("Entry %p\n",entrykey);

   json_object_put(root);
   return 0;
}

``` 

The json-c libraries offers a way to avoid this "_*problem*_". To illustrate, consider [_*json-parse08.c*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/json-parse08.c):

```C
#include <stdio.h>
#include <json-c/json.h>

int 
main(void)
{
   struct json_object_iterator it;
   struct json_object_iterator itEnd;

   json_object *root = json_object_from_file("contact.json");
   it = json_object_iter_init_default();
   it = json_object_iter_begin(root);
   itEnd = json_object_iter_end(root);

   while (!json_object_iter_equal(&it, &itEnd))
   {
      const char* key = json_object_iter_peek_name(&it);
      json_object* val = json_object_iter_peek_value(&it);
      printf("%s  -> %s\n", key, json_object_get_string(val));
      json_object_iter_next(&it);
   }
   json_object_put(root);
   return 0;
}
```

The code should explain itself, but consult the [documentation](https://json-c.github.io/json-c/json-c-0.15/doc/html/json__object__iterator_8h.html) for more details if needed.

**Note:** The usage of _*struct*_ in the declarations of _*it*_ and _*itEnd*_ are necessary for the data 'type' _*json_object_iterator*_.
