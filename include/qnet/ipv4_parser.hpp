#pragma once
#include <cstddef>

class IPv4Parser{
    public:
        static void parse(const unsigned char* data,std::size_t length);
};