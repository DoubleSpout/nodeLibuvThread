var express = require('express');
var thread = require('../');
var app = express();
var th_func = function(obj){
	var n = obj.n;
	var fibo =function fibo (n) { //在子线程中定义fibo函数
        return n > 1 ? fibo(n - 1) + fibo(n - 2) : 1;
    }
    var r = fibo(n);
    return r.toString();
}
app.get('/', function(req, res){
  var n = ~~req.query.n || 1;
  thread(th_func, {n:n}, function(err,result){
  //创建一个js线程,传入工作函数,buffer参数以及回调函数
        if(err) return res.end(err);//当线程创建失败
        res.end(result.toString());//响应线程执行计算的结果
    })
});
app.listen(8124);
console.log('listen on 8124');