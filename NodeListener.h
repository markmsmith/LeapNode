#ifndef NODE_LISTENER_H
#define NODE_LISTENER_H

#include <node.h>
#include "Leap.h"

using namespace v8;

class NodeListener;

struct LeapListenerBaton {
 uv_async_t async;

 NodeListener *nodeListener;

 const char *messageType;
 Leap::Frame frame;
};

class NodeListener : public Leap::Listener {
  public:
    NodeListener(Handle<Object> nodeListener);
    ~NodeListener();
    virtual void onInit(const Leap::Controller&);
    virtual void onConnect(const Leap::Controller&);
    virtual void onDisconnect(const Leap::Controller&);
    virtual void onFrame(const Leap::Controller&);

    Persistent<Object> _jsListener;

  private:
    uv_loop_t *_loop;
    LeapListenerBaton *_baton;
};

#endif
