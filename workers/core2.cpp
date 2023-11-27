#include <iostream>
#include <unistd.h>
#include <inttypes.h>

#include "core2.h"
#include "../utils/netfunctions.h"

// #define DATA_BUFF_SIZE 20000

using namespace std;

string interface_name2;
int32_t port2;

Core2::Core2(string interface_name_, int32_t port_)
{
    interface_name2 = interface_name_;
    port2 = port_;
}

Core2::~Core2() { };

void* Core2::run()
{
    int32_t i = 0;
    std::string ipb = getIfBroadcastAddr(interface_name2);
    int32_t client = 1;
    int32_t port = 31000;

    std:cout << interface_name2 << " "<< ipb << " " << port;
    startReceiving (client, ipb, port);
    std::cout << "\n";

    while (started){

        #define DATA_BUFF_SIZE 20000 
        char *rBuffer = new char [DATA_BUFF_SIZE+1];
        struct sockaddr_in resp;
        memset((char *)&resp, 0 , sizeof(sockaddr_in));
        socklen_t slen = 0 ;

        int32_t rLenght = recvfrom (client, rBuffer, DATA_BUFF_SIZE , 0 , (struct sockaddr *)& resp, &slen);
        if ( rLenght > 0 ){
            std::cout << "Wiadomosc odebrana: "<< rBuffer << "\n";
        }
    }
    return NULL;
}