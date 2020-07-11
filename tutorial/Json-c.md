# json-c


Json-c is a c library aiming to provide an object oriented like approach to using JSON objects in C. It aims to conform to RFC 7159 while being easy to use. Most of the memory management issues are handled by the library itself 'behind the scenes'.

The home page for json-c: [https://github.com/json-c/json-c](https://github.com/json-c/json-c).

This library allows one to easily construct JSON objects in C, read and write them, output them as JSON formatted strings and parse JSON formatted strings back into the C representation of JSON objects.

Linux users may be able to install json-c using their OS's packaging system. However, doing so may not result in the most recent version of the library. Currently, that is **[json-c-0.14-20200419](https://github.com/json-c/json-c/releases/tag/json-c-0.14-20200419)**. For use with this tutorial, it is recommended you use at least version 0.13. Not all of the sample programs will compile with earlier versions. To install from source, see the installation instructions on the [json-c github page](https://github.com/json-c/json-c#buildunix).

I will be using [Bodhi Linux 6.0](https://www.bodhilinux.com/) based on Ubuntu 20.04 as my Distro in this tutorial and gcc as my compiler. Bodhi 6.0 is currently unreleased but planned for this fall. So to install:

```
sudo apt install libjson-c-dev
```

This installs all the development (header) files as well as the library itself (libjson-c4).

For reference, the version installed is *0.13.1+dfsg-7ubuntu0.3*. Note, this corresponds to *Version 0.13.1*, released Mar 5, 2018, as is per the case for Ubuntu LTS releases. Rather dated but sufficient enough for this tutorial.

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
Now run _*json-version*_ to see what version of json-c you have installed. Not very exciting but it is a start.

Problem

1. How many packages in your distro depend upon json-c? How many of these are installed by default? Installed currently?
