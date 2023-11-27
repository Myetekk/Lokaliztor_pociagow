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
    std::ifstream f("data.json");
    json data = json::parse(f);
    string interface = data.value("interface_name", "not found");
    string port_ = data.value("port", "not found");
    int32_t port = stoi(port_);



    Core1 *core1 = new Core1( interface, port );
    core1 -> start();

    Core2 *core2 = new Core2( interface, port );
    core2 -> start();

    while (1 == 1)
    {
        sleep (200000);
    }

    core1 -> join();
    core1 -> stop();

    core2 -> join();
    core2 -> stop();



    return 0;
}