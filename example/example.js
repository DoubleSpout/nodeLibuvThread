var thread = require('../');
var threadFunc = function(obj){
	return ++obj.count;
}
thread(threadFunc, {count:0}, function(err,count){
	if(err) return console.log(err);
	console.log(count);//this will print 1
})
