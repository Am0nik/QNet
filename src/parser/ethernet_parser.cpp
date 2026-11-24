#include <iostream>
#include <iomanip>
#include <cstdint>

#include "qnet/ethernet_parser.hpp"
#include "qnet/ipv4_parser.hpp"

void EthernetParser::parse(const unsigned char* data,std::size_t length){
    if(length<14){
        std::cout << "Invalid Ethernet frame." << '\n';
        return;
    }
    std::cout << "Ethernet Frame\n";
    std::cout << "Destination MAC: ";

    for(int i=0;i<6;i++){
        if(i>0){
            std::cout << ':';
        }
        std::cout << std::hex << std::setw(2)
            << std::setfill('0')
            << static_cast<int>(data[i]);
    }
    std::cout << "\nSource MAC: ";
    for(int i=6;i<12;i++){
        if(i>6){
            std::cout<< ':';
        }
        std::cout << std::hex << std::setw(2)
            << std::setfill('0')
            << static_cast<int>(data[i]);
    }

    uint16_t etherType = (static_cast<uint16_t>(data[12]) << 8 | data[13]);

    std::cout << "\nEtherType: 0x"
        << std::setw(4)<<std::setfill('0')
        << etherType;

    if(etherType== 0x0800){
        std::cout << " (IPv4)";
        std::cout << std::dec << '\n';
        IPv4Parser::parse(data+14,length-14);
        return;
    }else if(etherType == 0x86DD){
        std::cout << " (IPv6)";
    }else if(etherType == 0x0806){
        std::cout << " (ARP)";
    }else if(etherType == 0x8100){
        std::cout << " (VLAN)";
    }else if(etherType == 0x88CC){
        std::cout << " (LLDP)";
    }else if(etherType == 0x888E){
        std::cout << " (EAP over LAN (802.1X))";
    }else if(etherType == 0x8847){
        std::cout << " (MPLS Unicast)";
    }else if(etherType == 0x8848){
        std::cout << " (MPLS Multicast)";
    }else if(etherType == 0x8035){
        std::cout << " (RARP)";
    }else if(etherType == 0x0842){
        std::cout << " (Wake-on-LAN (WoL))";
    }else if(etherType == 0x88E5){
        std::cout << " (MACsec (802.1AE))";
    }else if(etherType == 0x8864){
        std::cout << " (PPPoE Session)";
    }else if(etherType == 0x88A8){
        std::cout << " (Q-in-Q (802.1ad))";
    }else{
        std::cout << " (Unknown)";
    }
    std::cout << std::dec <<'\n';
}