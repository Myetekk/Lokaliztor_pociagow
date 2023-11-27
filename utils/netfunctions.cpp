#include "netfunctions.h"

std::string getIPAddress(std::string iface)
{
    struct ifaddrs * ifAddrStruct=NULL;
    struct ifaddrs * ifa=NULL;
    void * tmpAddrPtr=NULL;

    getifaddrs(&ifAddrStruct);

    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next)
    {
        if (!ifa->ifa_addr)
            continue;

        if (ifa->ifa_addr->sa_family == AF_INET)
        {
        	if (!strcmp(ifa->ifa_name,iface.c_str()))
        	{
				tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
				char *addressBuffer = new char[INET_ADDRSTRLEN];
				inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
                std::string retval(addressBuffer);
                delete [] addressBuffer;
                if (ifAddrStruct!=NULL) freeifaddrs(ifAddrStruct);
                return retval;
        	}
        }
    }
    if (ifAddrStruct != NULL) freeifaddrs(ifAddrStruct);

    return "0.0.0.0";
}





std::string getIfBroadcastAddr(std::string iif)
{
    ifaddrs *ifaddr;
    int family, s, n=0;
    std::string host;
    host.resize(128);

    if (getifaddrs(&ifaddr) == -1) {
        std::cerr << "Could not get ifaddrs" << std::endl;
        return "";
    }

    for (ifaddrs *ifa=ifaddr; ifa != nullptr; ifa = ifa->ifa_next, n++)
    {
        if (ifa->ifa_addr == nullptr)
        {
            continue;
        }
        family = ifa->ifa_addr->sa_family;
        if (iif.compare(ifa->ifa_name) == 0 && family == AF_INET)
        {
            s = getnameinfo(ifa->ifa_ifu.ifu_broadaddr,
                               (family == AF_INET) ? sizeof(struct sockaddr_in) :
                                                     sizeof(struct sockaddr_in6),
                               &host[0], host.size(),
                               NULL, 0, NI_NUMERICHOST);
            if (s == -1) {
                std::cerr << "Could not get ifaddrs" << std::endl;
                return "";
            }
            return host;
        }
    }

    freeifaddrs(ifaddr);
    return "";
}





int32_t sendUdpBroadcast(std::string addr, int32_t port, std::vector<char> data)
{
   // Open a socket
    int sd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sd<0) {
        fprintf(stderr, "Error: socket error.");
        return (int32_t)Result::SOCKET_ERROR;
    }
    
    // Set socket options
    // Enable broadcast
    int broadcastEnable=1;
    int ret=setsockopt(sd, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));
    if (ret) {
        fprintf(stderr, "Error: socket error.");
        close(sd);
        return (int32_t)Result::SOCKET_ERROR;
    }
    
    // Configure the port and ip we want to send to
    struct sockaddr_in broadcastAddr; // Make an endpoint
    memset(&broadcastAddr, 0, sizeof broadcastAddr);
    broadcastAddr.sin_family = AF_INET;
    inet_pton(AF_INET, addr.c_str(), &broadcastAddr.sin_addr); // Set the broadcast IP address
    broadcastAddr.sin_port = htons(port);
    
    // Send the broadcast request, ie "Any upnp devices out there?"
    ret = sendto(sd, &data[0], data.size(), 0, (struct sockaddr*)&broadcastAddr, sizeof broadcastAddr);
    if (ret<0) {
        fprintf(stderr, "Error: Could not send broadcast to %s", addr.c_str());
        close(sd);
        return (int32_t)Result::CONNECTION_ERROR;
    }
    
    // Get responses here using recvfrom if you want...
    close(sd);

    //LOGD(1, "Sended data to %s data: %s", senderAddr.c_str(), &buff[0]);
    return (int32_t)Result::SUCCESS;
}





void startReceiving(int32_t &client, std::string &broadcastIf, int32_t &port)
{
    int32_t flag = 1;
    struct sockaddr_in in_addr;
    socklen_t sSize = sizeof(in_addr);

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 500000;

//    LOGD(3, "broadcastIf: %s", broadcastIf.c_str());
    std::string broadcastIp = getIfBroadcastAddr(broadcastIf);

    client = socket(AF_INET, SOCK_DGRAM, 0);
    in_addr.sin_family = AF_INET;
	in_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    in_addr.sin_port = htons(port);

    int32_t n;
    n = setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,sizeof(timeout));
    if (n < 0)
    {
        fprintf(stderr, "SO_RCVTIMEO set ERROR!\r\n");
    }
   	// n = setsockopt(client, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout,sizeof(timeout));
    // if (n < 0)
    // {
    //     fprintf(stderr, "SO_RCVTIMEO set ERROR!\r\n");
    // }
    n = setsockopt(client, SOL_SOCKET, SO_REUSEADDR, &flag,sizeof(flag));
    if (n < 0)
    {
        fprintf(stderr, "SO_REUSEADDR set ERROR!\r\n");
    }
    n = setsockopt(client, SOL_SOCKET, SO_REUSEPORT, &flag,sizeof(flag));
    if (n < 0)
    {
        fprintf(stderr, "SO_REUSEPORT set ERROR!\r\n");
    }

    int32_t bb = bind(client,(struct sockaddr*)&in_addr,sSize);
    if (bb >= 0)
    {
        fprintf(stderr, "BIND: [%s] [:%d] OK\r\n", broadcastIp.c_str(), port);
    }
    else
    {
        fprintf(stderr, "BIND: [%s] [:%d] ERROR\r\n", broadcastIp.c_str(), port);
    }
}





uint64_t currentTime()
{
	struct timespec spec;
	uint64_t  ms; // Milliseconds

	clock_gettime(CLOCK_REALTIME, &spec);

	ms = spec.tv_sec * 1000LL;	//milisec
	ms += spec.tv_nsec / 1000000LL; // nano to mili
    
	return ms;
}