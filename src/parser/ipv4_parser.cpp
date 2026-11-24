#include <iostream>
#include <iomanip>
#include <cstdint>

#include "qnet/ipv4_parser.hpp"

void IPv4Parser::parse(const unsigned char* data,std::size_t length){
    if(length<20){
        std::cout << "Invalid Ipv4 packet" <<'\n';
        return;
    }

    uint8_t version =data[0] >>4;
    uint8_t headerLength = (data[0]& 0x0F) *4;

    if(version != 4 || headerLength < 20 || length < headerLength){
        std::cout << "Invalid IPv4 header" <<'\n';
        return;
    }

    uint16_t totalLength=(static_cast<uint16_t>(data[2])<<8) | data[3];

    if(totalLength < headerLength || totalLength > length){
        std::cout << "Invalid IPv4 packet length";
        return;
    }

    uint8_t ttl=data[8];
    uint8_t protocol = data[9];

    std::cout << "IPv4 Packet" << '\n';
    std::cout << "Source IP: ";

    for(int i=12;i<16;i++){
        if(i>12) std::cout <<'.';
        std::cout << static_cast<int>(data[i]);
    }

    std::cout << "\nDestination IP: ";

    for(int i=16;i<20;i++){
        if(i>16) std::cout << '.';

        std::cout << static_cast<int>(data[i]);
    }

    std::cout << "\nVersion: "<<static_cast<int>(version);
    std::cout << "\nHeader length: "<<static_cast<int>(headerLength);
    std::cout << "\nTotal length: "<<totalLength;
    std::cout << "\nTTL: "<<static_cast<int>(ttl);

    std::cout << "\nProtocol:";
    if (protocol==6)
        std::cout <<"TCP";
    else if (protocol==17)
        std::cout << "UDP";
    else if (protocol==1)
        std::cout << "ICMP";
    else
        std::cout << "Other (" <<static_cast<int>(protocol)<<")";

    std::cout << '\n';
}
