#include <iostream>
#include <unistd.h>
#include <string>
#include <inttypes.h>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include "workers/Find_Train_by_Distance.h"
#include "workers/Find_Train_by_GPS.h"
#include "utils/netfunctions.h"
#include "nlohman/json.hpp"
using json = nlohmann::json;
using namespace std;

string statsFile = "train_data/Gliwice_Czestochowa_stats.txt";
int32_t port = 30100;
std::string iface = "enp0s3";
string ipb = getIfBroadcastAddr("enp0s8");
int32_t client = 1;

string receiveFramesFromUdp()
{
    string frame;

    struct sockaddr_in resp;
    socklen_t slen = 0;
    char *buff = new char[2000];

    ssize_t result;

    result = recvfrom(client, buff,2000, 0, (struct sockaddr *)&resp, &slen);
    frame.assign(buff);
    return frame;
}

string prepare_string(string line){
    string data;

    data = line.substr(line.find("=") + 1);  // delete miliseconds and "DATA="

    data.erase(remove(data.begin(), data.end(), '|'), data.end());  // delete "|"
    data.erase(remove(data.begin(), data.end(), ';'), data.end());  // delete ";"

    return data;
}

int32_t main(int argc, char *argv[])
{   
    startReceiving(client,ipb,port);
    string frame;

    while (true)
    {
        frame = receiveFramesFromUdp();
        string data = prepare_string(frame);
        cout<<data<<endl;
        usleep(100000);
    }
    return 0;
}