#pragma once

#ifndef NDEBUG
#include <cstdio>
#define DEBUG(...) do { printf(__VA_ARGS__); printf("\n"); } while (0)
#else
#define DEBUG(...)
#endif

using ulong = unsigned long;
