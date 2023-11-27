#ifndef NETFUNCTIONS_H_
#define NETFUNCTIONS_H_

#include <stdint.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#include <cstring>
#include <iostream>
#include <vector>
#include <unistd.h>

enum class Result {SUCCESS=0x0, SOCKET_ERROR=0x1, CONNECTION_ERROR=0x2};

std::string getIPAddress(std::string iface);
std::string getIfBroadcastAddr(std::string iif);
int32_t sendUdpBroadcast(std::string addr, int32_t port, std::vector<char> data);

void startReceiving(int32_t &client, std::string &broadcastIf, int32_t &port);
uint64_t currentTime();

#endif