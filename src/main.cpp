#include <iostream>

#include "qnet/packet_capture.hpp"

int main(){
    std::cout << "QNet is starting...\n";
    std::cout << "QNet core modules\n";
    
    PacketCapture capture;
    capture.start();


    std::cout << "QNet is ready\n";
    return 0;
}