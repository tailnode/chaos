#pragma once
#include <stdio.h>

#define DEBUG_OUTPUT_CONSOLE 1

#if DEBUG_OUTPUT_CONSOLE
#define OUTPUT(format, ...) printf(format, __VA_ARGS__)
#else
#define OUTPUT(format, ...)
#endif