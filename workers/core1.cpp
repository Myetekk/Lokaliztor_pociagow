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


void byte_swap(string& text){//Funtion swapping bytes from little endian to big endian, works only for numbers of length 8,16,24 and so on....

        int length = text.length();
        if(length<=2)return;//no need to change bytes
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

void transform_info(string data, string info_name, int info_pos, int info_len)
{ 
    // do zmiany- co gdy "len" inne niż "8"
    // W tej funkcji trzeba poprawić odczytywanie współrzędnych szerokości i wysokości geograficznej

    info_pos = info_pos/4;
    info_len = info_len/4;
    if(info_len<=2){
        string info = data.substr(info_pos, info_len);  // find wanted info in string 
        int info_int;
        
        istringstream(info) >> hex >> info_int;  // convert info to decimal
        fprintf(stderr, "%s: %d \n", info_name.c_str(), info_int);  // print info
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

        fprintf(stderr, "%s: %f \n", info_name.c_str(), f);



    }


}

void* Core1::run()
{ 
    ifstream readFromFile("train_data/2022_07_22_08_10_Gliwice_Czestochowa_data");
    string line;
    while (!readFromFile.eof())
    {
        getline(readFromFile,line);
        string data = prepare_string(line);

        for(int i=0; i<6; i++)
        {
            array<string, 3> current_info = extract_info(i);  
            transform_info( data, current_info[0], stoi(current_info[1]), stoi(current_info[2]) );
        }

        //reading latitude from file
        array<string, 3> current_info = extract_info(14);
        transform_info( data, current_info[0], stoi(current_info[1]), stoi(current_info[2]) );

        //reading longitude from file
        current_info = extract_info(15);
        transform_info( data, current_info[0], stoi(current_info[1]), stoi(current_info[2]) );

        cout << "\n";
        sleep(1);
    }
    return NULL;
}



