#ifndef NODE_CONTROLLER_H
#define NODE_CONTROLLER_H

#include <node.h>
#include "Leap.h"
#include "NodeListener.h"

using namespace v8;

class NodeController : public node::ObjectWrap {
 public:
  static void Init(Handle<Object> target);

 private:
  NodeController(const Arguments& args);
  ~NodeController();

  Leap::Controller * _controller;
  NodeListener * _listener;

  static Handle<Value> New(const Arguments& args);
  static Handle<Value> Frame(const Arguments& args);
};

#endif
