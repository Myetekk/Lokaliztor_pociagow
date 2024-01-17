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
std::string iface = "enp0s3";
struct sockaddr_in resp;
socklen_t slen = 0;
int32_t port = 30100;
int32_t client = 0;
std::string ipb;
//////////////////////////////////////////////////////////////
//to pomiędzy komentarzami potem będzie do przeniesiena do osobnych plików
//narazie zrobiłem bigos żeby sprawdzić czy działa :)
std::map<std::string, std::vector<float>> readCoordinatesFromJSON(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Nie można otworzyć pliku." << std::endl;
        return {};
    }

    json jsonData;
    file >> jsonData;

    std::map<std::string, std::vector<float>> coordinates;

    for (const auto& entry : jsonData.items()) {
        std::string location = entry.key();
        std::vector<float> coords = entry.value().get<std::vector<float>>();
        coordinates[location] = coords;
    }

    return coordinates;
}
//////////////////////////////////////////////////////////////
string prepare_string(string line){
    string data;

    data = line.substr(line.find("=") + 1);  // delete miliseconds and "DATA="

    data.erase(remove(data.begin(), data.end(), '|'), data.end());  // delete "|"
    data.erase(remove(data.begin(), data.end(), ';'), data.end());  // delete ";"

    return data;
}
array<string, 3> extract_info(int info_index){
    std::ifstream infile("train_data/data_instrukcja.json");
    json data = json::parse(infile);  // parse json
    json firstAwiaParams = data["AwiaParams"][0];  // enter table AwiaParams
    json paramsArray = firstAwiaParams["params"][info_index];  // enter table Params
    array<string, 3> info;
    info[0] = paramsArray.value("key", "not found");  // get value "key"
    info[1] = to_string(paramsArray.at("awiabit"));  // get value "awiabit"
    info[2] = to_string(paramsArray.at("len"));  // get value "len"
    return info;
}
void byte_swap(string& text){  // Funtion swapping bytes from little endian to big endian, works only for numbers of length 8,16,24 and so on....
        int length = text.length();
        if(length<=2)return;  // no need to change bytes
        int x = length/2;
        char temp;
        for(int i=0;i<x;i+=2){
                temp=text[i];
                text[i]=text[length-2-i];
                text[length-2-i]=temp;
                temp=text[i+1];
                text[i+1]=text[length-1-i];
                text[length-1-i]=temp;
        }
}
void transform_info(string data, string info_name, int info_pos, int info_len,float &coord)
{ 
    info_pos = info_pos/4;
    info_len = info_len/4;
    if(info_len<=2){
        string info = data.substr(info_pos, info_len);  // find wanted info in string 
        int info_int;
        istringstream(info) >> hex >> info_int;  // convert info to decimal
        // fprintf(stderr, "%s: %d \n", info_name.c_str(), info_int);  // print info
    }
    else if(info_pos==52){
        string info = data.substr(info_pos, info_len);  // find wanted info in string 
        byte_swap(info);
        float info_float;
        int info_int;
        istringstream(info) >> hex >> info_int;  // convert info to decimal
        coord = info_int;
    }
    else{
        string info = data.substr(info_pos,info_len);
        byte_swap(info);
        union ulf
        {
            unsigned long ul;
            float f;
        };
        ulf u;
        string str = info;
        stringstream ss(str);
        ss >> hex >> u.ul;
        float f = u.f;
        coord = u.f;
        // fprintf(stderr, "%s: %f \n", info_name.c_str(), f);
    }
}
void getCoords(string data, float& x, float& y, int& distance){
    //reading latitude from file
    float distance_temp;
    array<string, 3> current_info = extract_info(14);
    transform_info(data,current_info[0],stoi(current_info[1]),stoi(current_info[2]),x);
    //reading longitude from file
    current_info = extract_info(15);
    transform_info(data, current_info[0], stoi(current_info[1]), stoi(current_info[2]),y);
    current_info = extract_info(18);
    //reading distance from file
    transform_info(data, current_info[0], stoi(current_info[1]), stoi(current_info[2]),distance_temp);
    distance = distance_temp;
}
int findFirstStation(float& x,float& y){
    return 0;
}
// Reading train's distance on the begining of the run
void distanceOnStart(int &distance_on_start){
    float distance_on_start_temp;

    ifstream readFromFile("train_data/2022_07_22_08_10_Gliwice_Czestochowa_data");
    string line;
    getline(readFromFile,line);
    string data = prepare_string(line);

    array<string, 3> current_info = extract_info(18);
    transform_info(data, current_info[0], stoi(current_info[1]), stoi(current_info[2]), distance_on_start_temp);
    distance_on_start = distance_on_start_temp;
}
void getStatsInfoFromLine(map<string,vector<float>>& stations,string line,vector<string>& namesOfStations){ //Function processing file containg information about stations
        vector<float> temp;
        string radius;
        string coords = line.substr(line.find('[')+1,line.find(']')-line.find('[')-1);
        temp.push_back(stof(coords.substr(0,coords.find(","))));
        temp.push_back(stof(coords.substr(coords.find(",")+1)));
        int radPos;
        radPos =line.find("Rad: ")+5;
        radius = line.substr(radPos,line.find("  ",radPos)-radPos);
        temp.push_back(stof(radius));
        int nameposition = line.find("Name:");
        string stationName = line.substr(line.find("Name:")+6,line.find("  ",6+nameposition)-6-nameposition);
        stations.insert(make_pair(stationName,temp));
        namesOfStations.push_back(stationName);
}
void getStatsFromFile(std::map<std::string, std::vector<float>>& coordinates1,vector<string>& namesOfStationsTest){
    ifstream f(statsFile);
    string line;
    while(getline(f,line)){
        getStatsInfoFromLine(coordinates1,line,namesOfStationsTest);
    }
}
int32_t main(int argc, char *argv[])
{   
    char* buff = new char[300];
    int32_t mass;
    vector<string> route;
    std::map<std::string, std::vector<float>> coordinates;
    getStatsFromFile(coordinates,route);
    //std::map<std::string, std::vector<float>> coordinates = readCoordinatesFromJSON("train_data/station.json");
    //Receiving data through network 
    //ipb = getIfBroadcastAddr(iface);
    //startReceiving(client,ipb,port);
    //while(1==1){
    //mass=recvfrom(client,buff,3000,0,(struct sockaddr *)&resp,&slen);
    //std::cout<<buff<<endl;}
    //
    //pętla do wyświetlania wszystkich miast z mapy i ich współrzędnych 
    //jako robocze tylko do podglądu.
    for (const auto& city : coordinates) {
        std::cout << "Współrzędne dla " << city.first << ": ";
        for (const auto& coord : city.second) {
            std::cout << coord << " ";
        }
        std::cout << std::endl;
    }
    ifstream readFromFile("train_data/2022_07_22_08_10_Gliwice_Czestochowa_data");
    string line;
    float x,y;
    int distance_from_start;
    int distance_on_start;
    float current_distance = (distance_from_start - distance_on_start);
    float current_distance_prev = -1;
    int firststation = -1;
    int state = 0;
    int currentStation = 0;
    distanceOnStart(distance_on_start);
    while (!readFromFile.eof())
    {
        getline(readFromFile,line);
        string data = prepare_string(line);
        //reading coords, and distance from file
        getCoords(data, x, y, distance_from_start);
        //żeby nie wypisywać kilka razy danych z tego samego miejsca (gdy pociąg stoi na stacji, często na trasie reportuje 2 razy to samo)
        current_distance = (distance_from_start - distance_on_start);
        // if (current_distance != current_distance_prev){
            cout << setprecision(8) << endl << "X: " << x << endl << "Y: " << y << endl;
            cout << "Distance from start: " << current_distance / 1000 << endl;
            cout << "Current location: " << Find_Train_by_Distance(current_distance,route) << endl;
            cout << "\n";
            //usleep(400000);
        // }
        current_distance_prev = current_distance;
        if(firststation == -1){
            findFirstStation(x,y);
        }
        //usleep(500000);
        Find_Train_by_GPS(x,y,state,coordinates,currentStation,route);
    }
    return 0;
}
