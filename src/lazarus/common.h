#pragma once

#include <stdexcept>

#ifndef NDEBUG
#include <cstdio>
#define DEBUG(...) do { printf(__VA_ARGS__); printf("\n"); } while (0)
#else
#define DEBUG(...)
#endif

using ulong = unsigned long;

/**
 * Class for exceptions related to the Lazarus engine.
 * 
 * Meant only for internal use.
 */
namespace __lz
{    
class LazarusException : public std::runtime_error
{
public:
    LazarusException(const std::string &msg)
        : std::runtime_error(msg)
    {
    }

    const char *c_str() const noexcept
    {
        return what();
    }
};
}
