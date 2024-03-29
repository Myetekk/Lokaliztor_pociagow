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

//  locates the train by the distance
void Find_Train_by_Distance(float x, float y, string& stateDist, float current_distance, vector<string> route, vector<int> distance){
    string current_location;
    string current_message = stateDist;

    for( size_t i=0; i<route.size(); i++ ) {
        if (current_distance >= distance[i]-300  &&  current_distance < distance[i]-150){
            current_message = "Zblianie sie do stacji: " + route[i];
            break;
        }
        else if (current_distance >= distance[i]-150  &&  current_distance <= distance[i]+150 ) {
            current_message = "Na stacji: " + route[i];
            break;
        }
        else if (current_distance > distance[i]+150  &&  current_distance <= distance[i]+300 ) {
            if  (i != route.size()-1)
                current_message = "Opuszczono stacje: " + route[i] + "  Następna stacja: " + route[i+1];
            break;
        }
    }

    if (current_message != stateDist) {
        cout << "BY DISTANCE: \n";
        cout << setprecision(8) << x << " " << y << ", dist: " << current_distance/1000 << "\n" << current_message << "\n\n";
        stateDist = current_message;
    }

}









