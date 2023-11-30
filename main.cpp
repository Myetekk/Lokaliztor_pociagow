#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <unistd.h>
#include <inttypes.h>

#include "nlohman/json.hpp"
#include "workers/core1.h"
#include "workers/core2.h"

using json = nlohmann::json;

using namespace std;

int32_t main(int argc, char *argv[])
{
    Core1 *core1 = new Core1(  );
    core1 -> start();
    core1 -> join();
    core1 -> stop();

    // Core2 *core2 = new Core2( interface, port );
    // core2 -> start();
    // core2 -> join();
    // core2 -> stop();


    return 0;
}