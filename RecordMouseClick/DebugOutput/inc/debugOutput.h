#pragma once

#define DEBUG_OUTPUT_CONSOLE 0

#if DEBUG_OUTPUT_CONSOLE
#define OUTPUT(format, ...) printf(format, __VA_ARGS__)
#else
#define OUTPUT(format, ...)
#endif