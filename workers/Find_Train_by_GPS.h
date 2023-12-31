#ifndef FIND_TRAIN_BY_GPS_H
#define FIND_TRAIN_BY_GPS_H

#include <string>
#include <iostream>
#include "../utils/netfunctions.h"
#include <stdlib.h>
#include <stdint.h>
#include <map>
#include <vector>
#include <cmath>
void Find_Train_by_GPS(float x, float y,int& state,std::map<std::string,std::vector<float>>& coordinates,int& currentStation,std::vector<std::string>& route);
//enum class ResultS {SUCCESS=0x0};
enum class State{BEFORESTATION=0x0,APROACHINGSTATION=0X1,ONSTATON=0X2};

#endif