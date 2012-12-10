#ifndef LEAPCONVERTER_H
#define LEAPCONVERTER_H

#include <node.h>
#include "Leap.h"

using namespace v8;

class LeapConverter{

    public:
        static Handle<Object> VectorToV8(Leap::Vector);
};

#endif
