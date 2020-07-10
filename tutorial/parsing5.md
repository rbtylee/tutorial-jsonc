# Parsing a Json object - part 6: Iterators

We introduced the function _*json_object_object_foreach*_ previously. However, json-c offers another iteration API more in a C++ iterator type style.

Consider our previous example: 

```
#include <stdio.h>
#include <json-c/json.h>

int main(void)
{
   json_object *root = json_object_from_file("contact.json");
   json_object_object_foreach(root, key, val)
      printf("%s  -> %s\n", key, json_object_get_string(val));

   json_object_put(root);
}

```

If we refactor the above by replacing the macro _*json_object_object_foreach*_, we have:

```
#include <stdio.h>
#include <json-c/json.h>

int main(void)
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
}
```

From an Object-Oriented point of view, we are violating the principle of [encapsulation](https://en.wikipedia.org/wiki/Encapsulation_(computer_programming)) and exposing the json-c internal representation of the _*json_object*_. This is not as apparent when using the macro _*json_object_object_foreach*_, yet even then the programmer has access to the internal struct lh_entry:

```
#include <stdio.h>
#include <json-c/json.h>

int 
main(void)
{
   json_object *root = json_object_from_file("contact.json");
   json_object_object_foreach(root, key, val)
      printf("Entry %p\n",entrykey);

   json_object_put(root);
   return(0);
}

``` 

If we wish to avoid this "_*problem*_", we can refactor our code as follows:

```
#include <stdio.h>
#include <json-c/json.h>

int main(void)
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
}
```

Consult the documentation for more details.
