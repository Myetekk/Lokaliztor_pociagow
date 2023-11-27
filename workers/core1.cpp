#include <iostream>
#include <unistd.h>
#include <string>
#include <inttypes.h>

#include "core1.h"
#include "../utils/netfunctions.h"

using namespace std;

string interface_name1;
int32_t port1;

Core1::Core1(string interface_name_, int32_t port_)
{
    interface_name1 = interface_name_;
    port1 = port_;
}

Core1::~Core1() { };

void* Core1::run()
{ 
    int i = 0;
    
    std::string ipb = getIfBroadcastAddr(interface_name1);

    while(started)
    {
        std::string sendframe = to_string(i);
        std::vector<char> sendframevec(sendframe.begin(), sendframe.end());
        sendUdpBroadcast(ipb, port1, sendframevec);
        // cout << i <<endl;
        fprintf(stderr, "\nWiadomość wysłana: %s \n", to_string(i).c_str());
        i++;
        sleep(3);
    }

    return NULL;
}