// gcc json-parse10.c -L/usr/local/lib -I/usr/local/include/json-c -ljson-c -o json-parse10
// see json-parse07.c

#include <stdio.h>
#include <json-c/json.h>
#include <json_visit.h>

static int 
doit(json_object *jso, int flags, json_object *parent_jso, const char *jso_key,
                       size_t *jso_index, void *userarg)
{
  printf("flags: 0x%x, key: %s, index: %ld, value: %s\n", flags,
     (jso_key ? jso_key : "(null)"), (jso_index ? (long)*jso_index : -1L),
      json_object_to_json_string(jso));
  return JSON_C_VISIT_RETURN_CONTINUE;
}

int 
main(void)
{
   json_object *root = json_object_from_file("contact.json");
   json_c_visit(root, 0, doit, NULL);

   json_object_put(root);
   return 0;
}

/*
 * 
 * output:
 * 
flags: 0x0, key: (null), index: -1, value: { "firstName": "John", "lastName": "Smith", "isAlive": true, "age": 27, "address": { "streetAddress": "21 2nd Street", "city": "New York", "state": "NY", "postalCode": "10021-3100" }, "phoneNumbers": [ { "type": "home", "number": "212 555-1234" }, { "type": "office", "number": "646 555-4567" } ], "children": [ ], "spouse": null }
flags: 0x0, key: firstName, index: -1, value: "John"
flags: 0x0, key: lastName, index: -1, value: "Smith"
flags: 0x0, key: isAlive, index: -1, value: true
flags: 0x0, key: age, index: -1, value: 27
flags: 0x0, key: address, index: -1, value: { "streetAddress": "21 2nd Street", "city": "New York", "state": "NY", "postalCode": "10021-3100" }
flags: 0x0, key: streetAddress, index: -1, value: "21 2nd Street"
flags: 0x0, key: city, index: -1, value: "New York"
flags: 0x0, key: state, index: -1, value: "NY"
flags: 0x0, key: postalCode, index: -1, value: "10021-3100"
flags: 0x2, key: address, index: -1, value: { "streetAddress": "21 2nd Street", "city": "New York", "state": "NY", "postalCode": "10021-3100" }
flags: 0x0, key: phoneNumbers, index: -1, value: [ { "type": "home", "number": "212 555-1234" }, { "type": "office", "number": "646 555-4567" } ]
flags: 0x0, key: (null), index: 0, value: { "type": "home", "number": "212 555-1234" }
flags: 0x0, key: type, index: -1, value: "home"
flags: 0x0, key: number, index: -1, value: "212 555-1234"
flags: 0x2, key: (null), index: 0, value: { "type": "home", "number": "212 555-1234" }
flags: 0x0, key: (null), index: 1, value: { "type": "office", "number": "646 555-4567" }
flags: 0x0, key: type, index: -1, value: "office"
flags: 0x0, key: number, index: -1, value: "646 555-4567"
flags: 0x2, key: (null), index: 1, value: { "type": "office", "number": "646 555-4567" }
flags: 0x2, key: phoneNumbers, index: -1, value: [ { "type": "home", "number": "212 555-1234" }, { "type": "office", "number": "646 555-4567" } ]
flags: 0x0, key: children, index: -1, value: [ ]
flags: 0x2, key: children, index: -1, value: [ ]
flags: 0x0, key: spouse, index: -1, value: null
flags: 0x2, key: (null), index: -1, value: { "firstName": "John", "lastName": "Smith", "isAlive": true, "age": 27, "address": { "streetAddress": "21 2nd Street", "city": "New York", "state": "NY", "postalCode": "10021-3100" }, "phoneNumbers": [ { "type": "home", "number": "212 555-1234" }, { "type": "office", "number": "646 555-4567" } ], "children": [ ], "spouse": null }
*/
