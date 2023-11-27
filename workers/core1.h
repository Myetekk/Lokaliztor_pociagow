#ifndef CORE1_H
#define CORE1_H

#include <string>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/thread.hpp"

using namespace std;

class Core1 : public Thread 
{
    public:
    Core1( string interface_name_, int32_t port_ );
    virtual ~Core1();
    void* run(  );

    private:
};

#endif