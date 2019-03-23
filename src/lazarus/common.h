#pragma once

#include <stdexcept>

#ifndef NDEBUG
#include <cstdio>
#define DEBUG(...) do { printf(__VA_ARGS__); printf("\n"); } while (0)
#else
#define DEBUG(...)
#endif

class LazarusException : std::runtime_error
{
public:
  LazarusException(const std::string &msg);
  const char *c_str() const noexcept
  {
      return what();
  }
};
