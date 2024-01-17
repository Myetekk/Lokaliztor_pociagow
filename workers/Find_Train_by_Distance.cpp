#include "Find_Train_by_Distance.h"

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
int getIndexOfFirstStation(float current_distance,vector<int>& distances){
    if(current_distance==0) return 0;
    int n=distances.size();
    int i=0;
    while (i<n){
        if(current_distance<=distances[i]) return i;
        i++;
    }
    return 0;

}
string Find_Train_by_Distance(float current_distance,vector<string>& route,vector<int>& distance){
    string current_location;
    //vector<int> distance;
    for( size_t i=0; i<route.size(); i++ ) {
        if (current_distance >= distance[i]-200  &&  current_distance <= distance[i]+200){
            return route[i];
        }
        else if (current_distance >= distance[i]+200  &&  current_distance <= distance[i+1]-200 ) {
            return route[i] + " -> " + route[i+1];
        }
    }


    return "ERROR";
}









