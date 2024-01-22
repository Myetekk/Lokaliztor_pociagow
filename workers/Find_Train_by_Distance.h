#ifndef CORE1_H
#define CORE1_H

#include <string>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/thread.hpp"
#include <iostream>
#include <unistd.h>
#include <inttypes.h>
#include <algorithm>
#include <fstream>
#include <iomanip>

#include "../utils/netfunctions.h"
#include "../nlohman/json.hpp"

using namespace std;

void Find_Train_by_Distance(float x,float y,string& stateDist,float current_distance,vector<string> route,vector<int> distance);
int getIndexOfFirstStation(float current_distance,vector<int>& distance);
void readDistances(vector<int>& distance);
#endif