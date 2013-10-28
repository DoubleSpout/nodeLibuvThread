nodeLibuvThread[![Build Status](https://travis-ci.org/DoubleSpout/nodeLibuvThread.png?branch=master)](https://travis-ci.org/DoubleSpout/nodeLibuvThread)
===============

node multi thread support using libuv.

## Installing the module

With [npm](http://npmjs.org/):

libuv_thread module is supported windows, linux, mac.

Make sure, node-gyp has installed.

     npm install libuv_thread

From source:

     git clone https://github.com/DoubleSpout/nodeLibuvThread.git
     cd libuv_thread
     node-gyp rebuild

To include the module in your project:

     var libuv_thread = require('libuv_thread');

##Simple Example:

    var thread = require('../');
    var threadFunc = function(obj){
    	return ++obj.count;
    }
    thread(threadFunc, {count:0}, function(err,count){
    	if(err) return console.log(err);
    	console.log(count);//this will print 1
    })

##Api:

    libuv_thread(func, obj, callback);

func: thread work function

obj: arguments for the thread funtion

callback:has two arguments,error and result,the result will be thread work return value




