#include <iostream>
#include <unistd.h>
#include <string>
#include <inttypes.h>
#include <algorithm>
#include <fstream>
#include <iomanip>

#include "Find_Train_by_Distance.h"
#include "../utils/netfunctions.h"
#include "../nlohman/json.hpp"
using json = nlohmann::json;

using namespace std;

//Reading routes from file
void readStations(vector<string>& route){
    fstream file_route("train_data/route.json");
    json route_json = json::parse(file_route);
    route = route_json.get<vector<string>>();
}
//Reading distances from file
void readDistances(vector<int>& distance){
    fstream file_distance("train_data/distances.json");
    json distance_json = json::parse(file_distance);
    distance = distance_json.get<vector<int>>();
}
string Find_Train_by_Distance(float current_distance){
    string current_location;
    vector<string> route;
    vector<int> distance;

    readStations(route);
    readDistances(distance);

    for( size_t i=0; i<route.size(); i++ ) {
        if (current_distance >= distance[i]  &&  current_distance <= distance[i]+1000){
            return route[i];
        }
        else if (current_distance >= distance[i]+1000  &&  current_distance <= distance[i+1]) {
            return route[i] + " -> " + route[i+1];
        }
    }


    return "ERROR";
}









