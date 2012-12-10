#define BUILDING_NODE_EXTENSION
#include <node.h>
#include "Leap.h"
#include "NodeListener.h"
#include "LeapConverter.h"

using namespace v8;

extern "C" {

  Handle<Value> convertCppFrameToV8(const Leap::Frame frame){

    const std::vector<Leap::Hand>& hands = frame.hands();
    const size_t numHands = hands.size();

    if (numHands >= 1) {
      // Get the first hand
      const Leap::Hand& hand = hands[0];

      // Check if the hand has any fingers
      const std::vector<Leap::Finger>& fingers = hand.fingers();
      const size_t numFingers = fingers.size();
      if (numFingers >= 1) {
        const Leap::Finger& finger = fingers[0];
        const Leap::Ray& tip = finger.tip();
        const Leap::Vector pos = tip.position;

        //return Local<Value>::New(Number::New(pos.x));

        //TODO convert the whole frame
        return LeapConverter::VectorToV8(pos);
      }
    }

    return Local<Value>::New(String::New("No fingers"));
  }

  static uv_mutex_t batonMutex;

  void handleCallbackInV8(uv_async_t *async, int status /*UNUSED*/){

    fprintf(stderr, "C++ Handling async message\n");
    HandleScope handleScope;

    fprintf(stderr, "C++ Getting baton\n");
    uv_mutex_lock(&batonMutex);
    LeapListenerBaton *baton = static_cast<LeapListenerBaton*>(async->data);
    fprintf(stderr, "C++ Getting messageType\n");
    Local<String> callbackName = Local<String>::New(String::New(baton->messageType));
    fprintf(stderr, "C++ Gettng nodeListener\n");
    NodeListener *nodeListener = baton->nodeListener;
    fprintf(stderr, "C++ Gettng jsListener from nodeListener at address %p\n", nodeListener);
    Handle<Object> jsListener = nodeListener->_jsListener;
    if( !jsListener->IsObject() ){
          fprintf(stderr, "Error, the supplied leap listener is not an object\n");
          //TODO display error
          return;
      }
    fprintf(stderr, "C++ Looking up callback '%s'\n", baton->messageType);
    Handle<Value> callbackVal = jsListener->Get(callbackName);
    fprintf(stderr, "C++ got callback '%s'\n", baton->messageType);

      if( !callbackVal->IsFunction() ){
          fprintf(stderr, "Error, the callback '%s' is not a function\n", baton->messageType);
          //TODO display error
          return;
      }
      Handle<Function> callbackFunction = Handle<Function>::Cast(callbackVal);

      if( strcmp(baton->messageType, "onFrame") == 0 ){
        unsigned argc = 1;

        //const char *frameJson = "{ 'hello': 'world' }";
        // Local<Value> argv[] = { Local<Value>::New(String::New(frameJson)) };

        const Leap::Frame frame = baton->frame;
        Local<Value> argv[] = { Local<Value>::New(convertCppFrameToV8(frame)) };

        callbackFunction->Call(Context::GetCurrent()->Global(), argc, argv);
      }
      else{
        unsigned argc = 0;
        Local<Value> argv[] = {};
        callbackFunction->Call(Context::GetCurrent()->Global(), argc, argv);
      }
      uv_mutex_unlock(&batonMutex);
  }

}

NodeListener::NodeListener(Handle<Object> jsListener){
    _jsListener = Persistent<Object>::New(jsListener);

    _loop = uv_default_loop();

    uv_mutex_init(&batonMutex);

    _baton = new LeapListenerBaton();
    _baton->nodeListener = this;
    uv_async_init(_loop, &_baton->async, handleCallbackInV8);
    _baton->async.data = _baton;
}

NodeListener::~NodeListener(){
  uv_close((uv_handle_t*) &_baton->async, NULL);
  delete _baton;
  _jsListener.Dispose();
}

void NodeListener::onInit(const Leap::Controller& controller) {
  fprintf(stderr, "C++ Got onInit callback\n");
  uv_mutex_lock(&batonMutex);
    _baton->messageType = "onInit";
  uv_mutex_unlock(&batonMutex);
    // uv_async_send(&_baton->async);
}

void NodeListener::onConnect(const Leap::Controller& controller) {
  fprintf(stderr, "C++ Got onConnect callback\n");
  // _baton->messageType = std::string("onConnect");
  // uv_async_send(&_baton->async);
}

void NodeListener::onDisconnect(const Leap::Controller& controller) {
  fprintf(stderr, "C++ Got onDisconnect callback\n");
  // _baton->messageType = std::string("onDisconnect");
  // uv_async_send(&_baton->async);
}

void NodeListener::onFrame(const Leap::Controller& controller) {
    fprintf(stderr, "C++ Got onFrame callback\n");
    uv_mutex_lock(&batonMutex);
    _baton->messageType = "onFrame";
    fprintf(stderr, "Asking for frame\n");
    _baton->frame = controller.frame();
    uv_mutex_unlock(&batonMutex);
    fprintf(stderr, "C++ Got frame, async sending\n");
    uv_async_send(&_baton->async);
    fprintf(stderr, "C++ async sent\n");
}
