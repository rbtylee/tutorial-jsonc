# json-c


Json-c is a c library aiming to provide an object oriented like approach to using JSON objects in C. It aims to conform to RFC 7159 while being easy to use. Most of the memory management issues are handled by the library itself 'behind the scenes'.

The home page for json-c: [https://github.com/json-c/json-c](https://github.com/json-c/json-c).

Linux users may be able to install json-c using their OS's packaging system. Doing so may not result in the most recent version of the library tho. While you may  install by following the installations instructions found on the Github page, this is only recommended if you really know what you are doing. Many other packages may depend upon json-c. Note, the documentation can be generated directly from the source codes using Doxygen or accessed [online](http://json-c.github.io/json-c/).

I will be using [Bodhi Linux 5.1](https://www.bodhilinux.com/) based on Ubuntu 18.04 as my Distro in this tutorial and gcc as my compiler. So to install:


```
sudo apt install libjson-c-dev
```

This installs all the development (header) files as well as the library itself (libjson-c3).

For reference, the version installed is *0.12.1-1.3ubuntu0.3*. Note, this corresponds to *Version 0.12.1*, released Jun 7, 2016, as is per the case for Ubuntu LTS releases. Rather dated but sufficient enough for this tutorial.

Without much explanation, as it is rather self-explanatory,  let's write our first json-c program, _*json-version.c*_:

````
#include <stdio.h>
#include <json-c/json.h>

int
main(void)
{
   printf("Version: %s\n", json_c_version());
   printf("Version Number: %d\n", json_c_version_num());
   return 1;
}
````

To compile:

```
gcc json-version.c -ljson-c -o json-version
```
Now run json-version to see what version of json-c you have installed. Not very exciting but it is a start.

Problem

1. How many packages in your distro depend upon json-c?
