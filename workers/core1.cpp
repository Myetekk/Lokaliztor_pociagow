#include <iostream>
#include <unistd.h>
#include <string>
#include <inttypes.h>
#include <algorithm>
#include <fstream>
#include <iomanip>

#include "../nlohman/json.hpp"
#include "core1.h"
#include "../utils/netfunctions.h"

using json = nlohmann::json;

using namespace std;

Core1::Core1()
{

}

Core1::~Core1() { };


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


void transform_info(string data, string info_name, int info_pos, int info_len){  // do zmiany- co gdy "len" inne niż "8"
    string info = data.substr(info_pos/4, info_len/4);  // find wanted info in string 
    int info_int;
    istringstream(info) >> hex >> info_int;  // convert info to decimal
    fprintf(stderr, "%s: %d \n", info_name.c_str(), info_int);  // print info
}

void* Core1::run()
{ 
    ifstream readFromFile("train_data/2022_07_22_08_10_Gliwice_Czestochowa_data");
    string line;
    while (!readFromFile.eof())
    {
        getline(readFromFile,line);
        for(int i=0; i<6; i++)
        {
            string data = prepare_string(line);

            array<string, 3> current_info = extract_info(i);
        
            transform_info( data, current_info[0], stoi(current_info[1]), stoi(current_info[2]) );
        }
        sleep(1);
    }
    return NULL;
}



