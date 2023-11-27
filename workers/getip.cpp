#include <iostream>
#include <string>
#include <inttypes.h>

#include "../utils/netfunctions.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage %s:\r\n", argv[0]);
        fprintf(stderr, "\t%s <iface>\r\n", argv[0]);
        return -1;
    }

    std::string iface = argv[1];
    int32_t port = atoi(argv[2]);
    std::string ip = getIPAddress(iface);
    std::string ipb = getIfBroadcastAddr(iface);

    // fprintf(stderr, "%s IP: %s\tBROAD: %s\r\n", iface.c_str(), ip.c_str(), ipb.c_str());

    std::string sendframe = "ABCDEF";
    std::vector<char> sendframevec(sendframe.begin(), sendframe.end());

    sendUdpBroadcast(ipb, port, sendframevec);
    
    



    return 0;
}
