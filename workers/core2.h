#ifndef CORE2_H
#define CORE2_H

#include <string>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/thread.hpp"

using namespace std;

class Core2 : public Thread 
{
    public:
    Core2( string interface_name_, int32_t port_ );
    virtual ~Core2();
    void* run();

    private:
};

#endif