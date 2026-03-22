#pragma once

#include <stdio.h>
#include <stdlib.h>

#define ASSERT(x) do { \
    if (!(x)) { \
        fprintf(stderr, "ASSERTION FAILED: %s\n", #x); \
        exit(1); \
    } \
} while(0)

#define LOG(x) do { \
    printf("%s\n", x); \
} while(0)
