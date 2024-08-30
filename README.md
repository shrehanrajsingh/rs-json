# rs-json

<picture>
  <source media="(prefers-color-scheme: dark)" srcset="https://user-images.githubusercontent.com/25423296/163456776-7f95b81a-f1ed-45f7-b7ab-8fa810d529fa.png">
  <source media="(prefers-color-scheme: light)" srcset="https://user-images.githubusercontent.com/25423296/163456779-a8556205-d0a5-45e2-ac17-42d089e3c3f8.png">
  <img alt="Shows an illustrated sun in light mode and a moon with stars in dark mode." src="https://user-images.githubusercontent.com/25423296/163456779-a8556205-d0a5-45e2-ac17-42d089e3c3f8.png">
</picture>

This is a JSON preprocessor written in C. It can be built using CMake.

## Introduction

The JSON Preprocessor is a tool that allows you to preprocess JSON files before using them in your application. 

## Installation

To build the JSON Preprocessor, you will need CMake installed on your system. Follow these steps to build the project:

1. Clone the repository: `git clone https://github.com/shrehanrajsingh/rs-json.git`
2. Create a build directory: `mkdir build && cd build`
3. Generate the build files: `cmake ..`
4. Build the project: `cmake --build .`

## Usage
`test.json`
```json
{
  "name": "John Doe",
  "age": 30,
  "email": "johndoe@example.com"
}
```

`test.c`
```c
#include <rsjson.h>

int
main (int argc, char const *argv[])
{
  /**
   * Create a new json instance
   */
  json_t *t = rs_json_new ();

  char *fd = readfile ("/path/to/test.json"); /* This function is also provided in the library */

  /**
   * Parse!
   */
  if (rs_json_parse (fd, t) != JSON_ESUCCESS)
    {
        printf ("failed to parse json.\n");
        exit (EXIT_FAILURE);
    }

  /* `t` now has parsed json stored as C structs. */

  char *name = JSON_STRING (
    rs_json_query ("$['name']", t)); /* '$' refers to json instance `t`. */

  int age = (int) JSON_INT (
    rs_json_query ("$['age']", t)); /* better to cast explicitly in case of numerical data types. */

  char *email = JSON_STRING (
    rs_json_query ("$['name']", t));

  return 0;
}
```
