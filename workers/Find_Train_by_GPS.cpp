#include "Find_Train_by_GPS.h"
#define MATH_PI 3.14159265358979323846
#define earthRadiusKm 6371.0

using namespace std;
// This function converts decimal degrees to radians
double deg2rad(double deg) {
  return (deg * MATH_PI / 180);
}

//  This function converts radians to decimal degrees
double rad2deg(double rad) {
  return (rad * 180 / MATH_PI);
}

//  calculates distance between points [lat1,lon1] and [lat2,lon2]
int32_t distanceCal(double lat1, double lon1, double lat2, double lon2, int64_t *odist)
{
    if ((lat1 == lat2) && (lon1 == lon2))
    {
        *odist = 0;
        return (int32_t)Result::SUCCESS;
    }
    double lat1r, lon1r, lat2r, lon2r, u, v;
    lat1r = deg2rad(lat1);
    lon1r = deg2rad(lon1);
    lat2r = deg2rad(lat2);
    lon2r = deg2rad(lon2);
    u = sin((lat2r - lat1r)/2);
    v = sin((lon2r - lon1r)/2);
    *odist = 1000 * 1000 * 2.0 * earthRadiusKm * asin(sqrt(u * u + cos(lat1r) * cos(lat2r) * v * v));
    return (int32_t)Result::SUCCESS;
}

//  finds the closest station to the current location (executed only once on start of the program)
void findCurrentStation(float x, float y, int& currentStation,  float current_distance, vector<string> route, vector<int> distance) {

    for (int i=0; i<route.size()-1; i++) { 

        if (current_distance >= distance[i]-300  &&  current_distance <= distance[i]+300){
            currentStation = i;
            break;
        }
        else if (current_distance > distance[i]+300  &&  current_distance < distance[i+1]-300){
            currentStation = i+1;
            break;
        }
    }

}

//  locates the train by the GPS coordinates
void Find_Train_by_GPS(float x, float y, int& state, std::map<std::string, std::vector<float>>& statsDetails, int& currentStation, std::vector<std::string>& route, bool& GPS_OK){
    int64_t dist;
    
    //std::cout<<"coordinatesX: "<< x <<"Y:"<<y<<"Map0:"<<statsDetails.at("Gliwice").at(0)<<"Map1:"<<statsDetails.at("Gliwice").at(1);
    distanceCal(x, y, statsDetails.at(route.at(currentStation)).at(0), statsDetails.at(route.at(currentStation)).at(1), &dist);
    dist=dist/1000;
    int radiusMargin = statsDetails.at(route.at(currentStation)).at(2);
    if(x==0||y==0){ 
        GPS_OK = false;
        state = (int32_t)State::BEFORESTATION;
        cerr<<"Test1";
    }
    if(state == 0 && dist<radiusMargin){
        state = (int32_t)State::APROACHINGSTATION;
        cout << "BY GPS: \n";
        cout << setprecision(8) << x << " " << y << "\n";
        cout<<"Zblianie sie do stacji: "<<route.at(currentStation)<<endl<<endl;
    }
    else if(state == 1  &&  dist > radiusMargin*2){
        state = (int32_t)State::BEFORESTATION;
    }
    else if(state == 1 && dist<150){
        state = (int32_t)State::ONSTATON;
        cout << "BY GPS: \n";
        cout << setprecision(8) << x << " " << y << "\n";
        cout<<"Na stacji: "<<route.at(currentStation)<<endl<<endl;
    }
    else if(state == 2 && dist>150){
        state =(int32_t)State::BEFORESTATION;
        cout << "BY GPS: \n";
        cout << setprecision(8) << x << " " << y << "\n";
        cout<<"Opuszczono stacje: "<<route.at(currentStation)<<"  Następna stacja: "<<route.at(currentStation+1)<<endl<<endl;
        currentStation+=1;
    }



    // cout << "\n\n state:" << state << "\n\n";


}
