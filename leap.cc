#define BUILDING_NODE_EXTENSION
#include <node.h>
#include "Leap.h"
#include "NodeController.h"

using namespace v8;

extern "C"{

  static void Init(Handle<Object> target) {
    NodeController::Init(target);
  }

  NODE_MODULE(leap, Init)
}
