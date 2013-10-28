#include <node.h>
#include <iostream>
#include "uv.h"
#include "libuvThreadClass.h"
#include "threadJobClass.h"

using namespace v8;

Handle<Value> LibuvThread::libuvThreadCC(const Arguments& args){
	HandleScope scope;
	ThreadJob *t_job_p = new ThreadJob();
	String::Utf8Value v1(args[0]->ToString());
    t_job_p->strFunc = *v1;
    t_job_p->callback = Persistent<Object>::New(args[1]->ToObject());
    t_job_p->uv_work.data = t_job_p;
    t_job_p->iserr = 0;
    uv_queue_work(uv_default_loop(), &t_job_p->uv_work, workerCallback, afterWorkerCallback);
	return scope.Close(Undefined()); 
};

void LibuvThread::workerCallback(uv_work_t* req){ //子线程中执行代码
	Isolate* isolate;
	Persistent<Context> context;
	ThreadJob* req_p = (ThreadJob *) req->data;

	isolate = Isolate::New();   //V8的isolate类
	isolate->Enter();
	context= Context::New();
	HandleScope scope1;
	TryCatch onError;
	Local<Value> result = Script::Compile(String::New(req_p->strFunc))->Run();
	if (!onError.HasCaught()){
		String::Utf8Value v2(result->ToString());	
		req_p->result = *v2;	
	}
	else{
		req_p->iserr = 1;
		Local<Value> err = onError.Exception();
		String::Utf8Value v3(err->ToString());
		req_p->result = *v3;
	}
	context.Dispose();
	isolate->Exit(); //退出 isolate
    isolate->Dispose(); //销毁 isolate
}


 void LibuvThread::afterWorkerCallback(uv_work_t *req, int status){//子线程执行完毕
 		HandleScope scope;
 		ThreadJob* req_p = (ThreadJob *) req->data;
 		Local<Value> argv[2];
		Local<Value> null;
 		if(req_p->iserr){
 			argv[0] = String::New(req_p->result);
	 		argv[1] = null; 		
 		}
 		else{
 			argv[0] = null;
	 		argv[1] = String::New(req_p->result);
 		}
 		req_p->callback->CallAsFunction(Object::New(), 2, argv); 
 		delete req_p;
}