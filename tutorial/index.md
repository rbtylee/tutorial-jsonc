# Json-c Tutorial Index

- [Intro](https://github.com/rbtylee/tutorial-jsonc/blob/master/tutorial/Intro.md)
    * contact.json introduced
- [Json-c](https://github.com/rbtylee/tutorial-jsonc/blob/master/tutorial/Json-c.md)
    * Installation
    * Print version:[ _*json-version.c*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/json-version.c)
- [Reading a JSON file](https://github.com/rbtylee/tutorial-jsonc/blob/master/tutorial/File.md)
    * Read contact.json from a file and print the JSON: [_*json-file00.c*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/json-file00.c)
    * Read contact.json from a file and print the JSON: [_*json-file01.c*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/json-file01.c)
- [Parsing a Json object - part 1: Intro](https://github.com/rbtylee/tutorial-jsonc/blob/master/tutorial/legacy.md)
    * Read contact.json  and print 2 name/attribute pairs: [_*json-parse00.c*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/json-parse00.c)
    * Simple reference count example: [_*json-parse01.c*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/json-parse01.c)
- [JSON_types](https://github.com/rbtylee/tutorial-jsonc/blob/master/tutorial/types.md)
    * Print types of the JSON objects in contact.json: [_*json-type00.c*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/json-type00.c)
- [Parsing a Json object - part 2 complex parsing](https://github.com/rbtylee/tutorial-jsonc/blob/master/tutorial/parsing2.md)
    * Print all values of all objects in contact.json: [_*json-parse04.c*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/json-parse04.c)
- [Parsing a Json object - part 3: Json strings](https://github.com/rbtylee/tutorial-jsonc/blob/master/tutorial/parsing3.md)
    * Create a JSON object from a string and print it: [_*json-str00.c*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/json-str00.c)
    * Print all values of all objects in contact.json: [_*json-parse05.c*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/json-parse05.c) (left as excersise)
- Curl/url example
    * use curl or ecore_con_url
- [Parsing a Json object - part 4: Iteration through a JSON obj](https://github.com/rbtylee/tutorial-jsonc/blob/master/tutorial/parsing4.md)
    * Illustrate iteration over a JSON object: example
    * Print all values of all objects in contact.json yet again: [_*json-parse06.c*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/json-parse06.c)
- [Creating and saving a JSON object](https://github.com/rbtylee/tutorial-jsonc/blob/master/tutorial/new.md)
    * Create, initialize and save a JSON: [_*json-new00.c*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/json-new00.c)
- [Editing a JSON Object](https://github.com/rbtylee/tutorial-jsonc/blob/master/tutorial/edit.md)
    * Change attribute values in a JSON: [_*json-edit00.c*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/json-edit00.c)
- [Memory management, ownership and references - part one: overview](https://github.com/rbtylee/tutorial-jsonc/blob/master/tutorial/memory.md)
    * Clarify these issues and answer the question of when to call json_object_put or json_object_get
- [Memory management, ownership and references - part two: examples](https://github.com/rbtylee/tutorial-jsonc/blob/master/tutorial/memory01.md)
    * Code illustrating memory issues.
    * [_*json-mem00.c*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/json-mem00.c)
    * [_*json-mem01.c*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/json-mem01.c)
- [JSON Circular references](https://github.com/rbtylee/tutorial-jsonc/blob/master/tutorial/circular01.md)
    * An example of a circular reference: [_*json-error00.c*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/json-error00.c)
- [JSON Comparison](https://github.com/rbtylee/tutorial-jsonc/blob/master/tutorial/equal.md)
    * Compare two JSONs for equality: [_*json-equal00.c*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/json-equal00.c)
- JSON Copying
   *
- [JSON Arrays: basics](https://github.com/rbtylee/tutorial-jsonc/blob/master/tutorial/list.md)
    * [_*json-array00.c*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/json-array00.c)
    * [_*json-array01.c*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/json-array01.c)
    * [_*json-array02.c*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/json-array02.c)
- [JSON Arrays: sorting and searching](https://github.com/rbtylee/tutorial-jsonc/blob/master/tutorial/sort.md)
    * Sorting and searching a JSON array: [_*json-array03.c*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/json-array03.c)
- [JSON Pointers](https://github.com/rbtylee/tutorial-jsonc/blob/master/tutorial/edit2.md)
    * Introduce using JSON pointers
- [Parsing a Json object - part 5: Iterators](https://github.com/rbtylee/tutorial-jsonc/blob/master/tutorial/parsing5.md)
    * Bettter Encapsulation using iterators
- [Parsing a Json object - part 6: Iteration yet again](https://github.com/rbtylee/tutorial-jsonc/blob/master/tutorial/parsing6.md)
    * Loop thru an array: [_*json-array04.c*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/json-array04.c)
    * Illustrate a depth-first search: [_*json-parse10.c*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/json-parse10.c)
    * Search a JSON for a particular value: [_*json-parse11.c*_](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/json-parse11.c)
- Error handling
- threads 
- debugging
- automake and meson

