#include "LeapConverter.h"

using namespace v8;

Handle<Object> LeapConverter::VectorToV8(Leap::Vector vec){
    HandleScope scope;
    Handle<Object> jsVec = Object::New();
    jsVec->Set(NODE_PSYMBOL("x"), Number::New(vec.x));
    jsVec->Set(NODE_PSYMBOL("y"), Number::New(vec.y));
    jsVec->Set(NODE_PSYMBOL("z"), Number::New(vec.z));

    return scope.Close(jsVec);
}
