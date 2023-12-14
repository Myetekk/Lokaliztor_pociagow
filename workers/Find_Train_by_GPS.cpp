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

void Find_Train_by_GPS(float x, float y,int& state,std::map<std::string,std::vector<float>>& statsDetails,int& currentStation,std::vector<std::string>& route){
    int64_t dist;

    //std::cout<<"coordinatesX: "<< x <<"Y:"<<y<<"Map0:"<<statsDetails.at("Gliwice").at(0)<<"Map1:"<<statsDetails.at("Gliwice").at(1);
    distanceCal(x,y,statsDetails.at(route.at(currentStation)).at(0),statsDetails.at(route.at(currentStation)).at(1),&dist);
    dist=dist/1000;
    int radiusMargin = statsDetails.at(route.at(currentStation)).at(2);
    if(state == 0 && dist<radiusMargin){
        state = (int32_t)State::APROACHINGSTATION;
        cout<<"\n\nBy GPS: Zblianie sie do stacji: "<<route.at(currentStation)<<endl<<endl;
    }
    else if(state==1&&dist>radiusMargin*2){
        state = (int32_t)State::BEFORESTATION;
    }
    else if(state == 1 && dist<150){
        state = (int32_t)State::ONSTATON;
        cout<<"\n\nBy GPS:Na stacji:"<<route.at(currentStation)<<endl<<endl;
    }
    else if(state == 2 && dist>150){
        state =(int32_t)State::BEFORESTATION;
        cout<<"\n\nBy GPS: Opuszczono stacje:"<<route.at(currentStation)<<endl<<endl;
        currentStation+=1;
    }
}