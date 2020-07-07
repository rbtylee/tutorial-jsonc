// gcc json-parse01.c -ljson-c -o json-parse01 

#include <stdio.h>
#include <json-c/json.h>

int 
main(void)
{
   char fname[255], lname[255];
   json_object *root = json_object_from_file("contact.json");
   if (!root)
       return 0;
       
   json_object *first_name = json_object_object_get(root,  "firstName");
   json_object *last_name = json_object_object_get(root,  "lastName");
   // Store the string values of these json_objects in our char arrays
   snprintf(fname, 255, "%s", json_object_get_string(first_name));
   snprintf(lname, 255, "%s", json_object_get_string(last_name));
   // Lose ownership of our json_objects first_name and last_name
   json_object_put(root);
   // Verify our char arrays still have the proper values
   printf("%s, %s\n", fname, lname);
   
   return 1;
}
