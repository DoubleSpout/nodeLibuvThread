#include <node.h>
#include <iostream>
#include <stdlib.h>
#include "uv.h"
#include "libuvThreadClass.h"
#include "threadJobClass.h"

using namespace v8;

Handle<Value> LibuvThread::libuvThreadCC(const Arguments& args){
	HandleScope scope;
	ThreadJob *t_job_p = new ThreadJob();
	String::Utf8Value v1(args[0]->ToString());
	t_job_p->strFunc = new char[strlen(*v1)+1];
	strcpy(t_job_p->strFunc,*v1);
	t_job_p->strFunc[strlen(*v1)] = '\0';
    t_job_p->callback = Persistent<Object>::New(args[1]->ToObject());
    t_job_p->uv_work.data = t_job_p;
    t_job_p->iserr = 0;
    int r = uv_queue_work(uv_default_loop(), &(t_job_p->uv_work), workerCallback, afterWorkerCallback);
	return scope.Close(Number::New(r)); 
};

void LibuvThread::workerCallback(uv_work_t* req){ //子线程中执行代码
	ThreadJob* req_p = (ThreadJob *) req->data;
	Isolate* isolate = Isolate::New();   //V8的isolate类
	if (Locker::IsActive()) {
		Locker myLocker(isolate);			
		isolate->Enter();	
		threadWork(req_p);
	}
	else{
		isolate->Enter();
		threadWork(req_p);
	}
	isolate->Exit(); //退出 isolate
	isolate->Dispose(); //销毁 isolate
}

void LibuvThread::threadWork(ThreadJob *req_p){
		HandleScope scope;
		Persistent<Context> context = Context::New();
		context->Enter();
		TryCatch onError;
		String::Utf8Value *v2;

		Local<Value> result = Script::Compile(String::New(req_p->strFunc))->Run();
		if (!onError.HasCaught()){
			v2 = new String::Utf8Value(result->ToString());		
		}
		else{
			req_p->iserr = 1;
			Local<Value> err = onError.Exception();
			v2 = new String::Utf8Value(err->ToString());
		}

		req_p->result = new char[strlen(**v2)+1];
		strcpy(req_p->result,**v2);
		req_p->result[strlen(**v2)] = '\0';
		delete v2;
		context.Dispose();
}

void LibuvThread::afterWorkerCallback(uv_work_t *req, int status){//子线程执行完毕
 		HandleScope scope;
 		ThreadJob* req_p = (ThreadJob *) req->data;
 		Local<Value> argv[2];

 		if(req_p->iserr){
 			argv[0] = String::New(req_p->result);
 			argv[1] = Local<Value>::New(Null());		
 		}
 		else{
 			argv[0] = Local<Value>::New(Null());
	 		argv[1] = String::New(req_p->result);
 		}
 		req_p->callback->CallAsFunction(Object::New(), 2, argv); 
 		delete req_p;
}