#pragma once
#include <cstddef>

class EthernetParser{
    public:
        static void parse(const unsigned char* data,std::size_t length);
};