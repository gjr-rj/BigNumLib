#pragma once

#define VERSION_MAJOR  1
#define VERSION_MIDDLE 0
#define VERSION_MINOR  1
#define VERSION_BUILD  0

#define STR(x)       #x
#define STRINGIFY(x) STR(x)

#define VERSION_STRING                                                        \
    STRINGIFY(VERSION_MAJOR)                                                  \
    "." STRINGIFY(VERSION_MIDDLE) "." STRINGIFY(VERSION_MINOR) "b" STRINGIFY( \
            VERSION_BUILD)
