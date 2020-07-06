# Parsing a Json object - part 5: Iteration thru a JSON obj

Looking at the parsing code discussed so far, you might wonder if there is an easier way to parse the file rather checking every attribute one at a time. Since we have been printing out the values for every attribute in the JSON file or string, shouldn't it be possible to simply iterate over the objects name/value pairs. Similar to a for loop only for json\_objects.

The answer is, naturally, yes.

In json-c, this is implemented as a macro. Typical usage is of the form:

```
json_object_object_foreach(obj, key, val)
{
    // processing code goes here
}
```

where obj is the JSON object you want to parse; key and value correspond to key: value pairs. However, key and value are placeholds and not actual variables you have to declare. They are declared by the macro itself, and you can use them inside the loop. Key is a char\* and val is a json_object\*. You can choose any _variable_ names to correspond to key and value if you wish.

## json_object_object_foreach: json-parse06.c

The simple program below illustrates the usage of this macro.

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

As usual create this file, save, compile and run. Expected output:

```
firstName  -> John
lastName  -> Smith
isAlive  -> true
age  -> 27
address  -> { "streetAddress": "21 2nd Street", "city": "New York", "state": "NY", "postalCode": "10021-3100" }
phoneNumbers  -> [ { "type": "home", "number": "212 555-1234" }, { "type": "office", "number": "646 555-4567" } ]
children  -> [ ]
spouse  -> (null)
```

As you can see we have looped over every Name/value pairs in our _*contact.json*_ file. The code is simple and easy to understand. As you may have noticed, the brackets are not needed if only a single statement is executed in the loop, just as is the case with other c style loops.

In cases where the value is another json\_object or a json\_array, the obj or array is not parsed. Simply printed as a string. Hence, the program is not generic in nature. However, we have all the tools to implement a recursive function to fully parse our _*contact.json*_ file, or in general any JSON.

## json_object_object_foreach: json-parse07.c

So let's apply this idea to our previous example _*json-parse04.c*_ and parse the entire file as before, printing out the name/value pairs.

```
#include <stdio.h>
#include <json-c/json.h>

void doarray(json_object *obj);

void
doit(json_object *obj)
{
   json_object_object_foreach(obj, key, val)
   {
      switch (json_object_get_type(val))
         {
          case json_type_array:
             printf("\n%s  \n\n", key);
             doarray(val);
             break;

          case json_type_object:
             printf("\n%s  \n\n", key);
             doit(val);
             break;

          default:
             printf("%s: %s\n", key, json_object_get_string(val));
         }
   }
}

void
doarray(json_object *obj)
{
   int temp_n = json_object_array_length(obj);
   for (int i = 0; i < temp_n; i++)
      doit(json_object_array_get_idx(obj, i));
}

int
main(void)
{
   json_object *root = json_object_from_file("contact.json");

   if (!root)
      return 0;
   doit(root);

   json_object_put(root);
   return 1;
}

```

You know what to do here. The expected output:

```
$ ./json-parse07
firstName: John
lastName: Smith
isAlive: true
age: 27

address 

streetAddress: 21 2nd Street
city: New York
state: NY
postalCode: 10021-3100

phoneNumbers 

type: home
number: 212 555-1234
type: office
number: 646 555-4567

children 

spouse: (null)

```

Ok, while this is not the *exact* output as our previous example _*json-parse04.c*_, it does illustrate the basic idea. And we have processed everything as before and printed the results.

# Problems

1. Print types of JSON objects in _*contact.json*_ using json_object_object_foreach as _*json-type00.c*_ does.

- Try to refactor _*json-parse07.c*_, so it produces _*exactly*_ the same output as _*json-parse04.c*_.

- Refactor _*json-parse07.c*_ to store the JSON values in a C struct.

- Why do you think I did not go over this first and forgo the more complex _*json-parse04.c*_?


