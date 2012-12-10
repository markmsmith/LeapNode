#define BUILDING_NODE_EXTENSION
#include <node.h>
#include "NodeController.h"
#include "NodeListener.h"

using namespace v8;

NodeController::NodeController(const Arguments& args) {
  //TODO error if missing listener arg, or wrong type (or missing callback?)
  _listener = new NodeListener( Handle<Object>::Cast(args[0]) );
  _controller = new Leap::Controller(_listener);
};

NodeController::~NodeController() {
  delete _controller;
  delete _listener;
};

void NodeController::Init(Handle<Object> target) {
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(String::NewSymbol("Controller"));
  tpl->InstanceTemplate()->SetInternalFieldCount(2);

  // Prototype
  tpl->PrototypeTemplate()->Set(String::NewSymbol("frame"),
      FunctionTemplate::New(Frame)->GetFunction());

  Persistent<Function> constructor = Persistent<Function>::New(tpl->GetFunction());
  target->Set(String::NewSymbol("Controller"), constructor);
}

Handle<Value> NodeController::New(const Arguments& args) {
  HandleScope scope;

  NodeController* obj = new NodeController(args);
  obj->Wrap(args.This());

  return args.This();
}

Handle<Value> NodeController::Frame(const Arguments& args) {
  HandleScope scope;

  NodeController* nodeController = ObjectWrap::Unwrap<NodeController>(args.This());
  const Leap::Frame frame = nodeController->_controller->frame();
  const std::vector<Leap::Hand>& hands = frame.hands();
  const size_t numHands = hands.size();
  if (numHands >= 1) {
    // Get the first hand
    const Leap::Hand& hand = hands[0];

    // Check if the hand has any fingers
    const std::vector<Leap::Finger>& fingers = hand.fingers();
    const size_t numFingers = fingers.size();
    return scope.Close(Number::New(numFingers));
  }

  return scope.Close(Number::New(0));
}

//

