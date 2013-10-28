#ifndef LIBUVTHREAD_H
#define LIBUVTHREAD_H
#include <node.h>
#include "threadJobClass.h"
using namespace v8;
class LibuvThread {
 public:
  static Handle<Value> libuvThreadCC(const Arguments& args);
  static void workerCallback(uv_work_t* req);
  static void threadWork(ThreadJob* req);
  static void afterWorkerCallback(uv_work_t *req, int status);
  LibuvThread(){};
  ~LibuvThread(){};
};
#endif