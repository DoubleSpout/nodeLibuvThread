#ifndef THREADJOB_H
#define THREADJOB_H
#include <node.h>

using namespace v8;
class ThreadJob {
 public:
    char *strFunc;//保存字符串的函数
    char *result;//结果
    int iserr;
    Persistent<Object> callback;//保存js的回调函数
    uv_work_t uv_work;
    ThreadJob(){};
    ~ThreadJob(){
        delete []result;
    	delete []strFunc;
    	callback.Dispose();
    }; 
};
#endif