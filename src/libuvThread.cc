#include <node.h>
#include "libuvThreadClass.h"
using namespace v8;

void Init(Handle<Object> target) {
  target->Set(String::NewSymbol("libuvThreadCC"),
           FunctionTemplate::New(LibuvThread::libuvThreadCC)->GetFunction());
}
NODE_MODULE(libuv_thread, Init)