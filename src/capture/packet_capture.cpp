#include <iostream>
#include <vector>
#include <pcap.h>
#include <csignal>
#include <iomanip>

#include "qnet/packet_capture.hpp"
#include "qnet/ethernet_parser.hpp"

volatile std::sig_atomic_t running = 1;

void stop_capture(int signal){
    running=0;
}


void PacketCapture::start(){
    std::cout << "Packet capture is starting..."<<'\n';
    std::cout << "Npcap ver:" << pcap_lib_version() << '\n';

    pcap_if_t *alldevs = nullptr;
    char errbuf[PCAP_ERRBUF_SIZE]={};
    if(pcap_findalldevs(&alldevs,errbuf) == -1){//if no interfaces are found
        std::cout << "ERROR:" << errbuf << '\n';
        return;
    }

    std::vector<pcap_if_t*> devices;//devices list

    std::cout << "Available network interfaces:\n";

    for( pcap_if_t *dev = alldevs;dev!=nullptr;dev=dev->next){
        devices.push_back(dev);//add a new device to list
        std::cout << devices.size() << ". ";

        /*std::cout << "Name: "<<dev->name<<'\n';*/
        if(dev->description != nullptr){ //presence of desc.
            std::cout << dev->description <<'\n';
        }else{
            std::cout << dev->name << '\n';
        }
        //std::cout << "----------------" << '\n';
    }

    if(devices.empty()){
        std::cout << "No network interfaces found" << '\n';
        pcap_freealldevs(alldevs);
        return;
    }

    std::cout << "Select interface:" << '\n';

    int choice;
    //if choise is invalid
    if(!(std::cin >> choice) || choice < 1 || choice > static_cast<int>(devices.size())){
        std::cout << "Invalid interface number!" << '\n';
        pcap_freealldevs(alldevs);
        return;
    }

    pcap_t *handle = pcap_open_live(
        devices[choice-1]->name, //selected devices name
        65536,0,1000,errbuf
    );

    pcap_freealldevs(alldevs);

    if(handle==nullptr){
        std::cout << "ERROR: "<<errbuf<<'\n';
        return; 
    }
    
    std::cout << "Interface opened successfully" << '\n';
    std::cout << "Capturing packets. Press CNTR+C to stop" << '\n';

    std::signal(SIGINT,stop_capture);

    pcap_pkthdr *header;
    const u_char *data;

    unsigned long long packet_count =0;

    while(running){

        int result = pcap_next_ex(handle,&header,&data);

        if(result==1){
            packet_count++;

            std::cout 
                << '#' << packet_count
                << " | Time: " << header->ts.tv_sec
                << "." << std::setfill('0') << std::setw(6)
                << header->ts.tv_usec << std::setfill(' ')
                << " | Length: "<<header->len << " bytes"
                << " | Captured: "<<header->caplen << " bytes"
                <<'\n';
            EthernetParser::parse(data, header->caplen);
            
        }else if(result == 0){
            continue;
        }else if(result == -1){
            std::cout << "Capture error:" << pcap_geterr(handle)<<'\n';
            break;
        }
        else if(result==-2){
            break;
        }

    }

    pcap_close(handle);

    std::cout << "Capture stopped\n";
    std::cout << "Total packets: "<< packet_count << '\n';

}