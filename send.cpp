#include <iostream>
#include <string>
#include <fstream>
#include "utils/netfunctions.h"
using namespace std;

int32_t main(int argc, char *argv[])
{   
    //do przeniesienia jako argumenty!!!
    //chcemy zeby przekazywać zmienne poniżej jako argumenty w funkcji main
    //czyli przy uruchamianiu programu podajemy ze spacją i z myślinikiem
    //komenda do uruchomienia: g++ -o send send.cpp utils/netfunctions.cpp -lstdc++
    //komenda do podgladania wyslanych ramek w terminalu: sudo tcpdump -lni any dst port 30100 -A
    int32_t port = 30100;
    std::string iface = "enp0s3";
    string ipb = getIfBroadcastAddr("enp0s8");
    ifstream readFromFile("train_data/2022_07_22_08_10_Gliwice_Czestochowa_data");
    string frame;
    while (!readFromFile.eof())
    {
        getline(readFromFile,frame);
        std::vector<char> framevec(frame.begin(),frame.end());
        sendUdpBroadcast(ipb,port,framevec);
        cout<<frame<<endl;
        sleep(1);
    }
    return 0;
}