// gcc json-version.c -ljson-c -o json-version
#include <stdio.h>
#include <json-c/json.h>

int
main(void)
{
   printf("Version: %s\n", json_c_version());
   printf("Version Number: %d\n", json_c_version_num());
   return 0;
}
