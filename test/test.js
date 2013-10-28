var should = require('should');
var thread = require('../');

//test1
try{
	thread(1)
}catch(e){
	should.equal(e, 'argument[0] must be a function');
}

//test2
try{
	thread(function(){},1)
}catch(e){
	should.equal(e, 'argument[1] must be an object');
}

//test3
var tf = function(){
	y;
}
thread(tf,{},function(err){
	should.equal(err, 'ReferenceError: y is not defined');
})

//test4
var tf = function(obj){
	return ++obj.count
}
thread(tf,{count:0},function(err,count){
	should.equal(count, '1');
})