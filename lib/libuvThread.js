var libuvThreadCC = require('../build/Release/libuvThread.node').libuvThreadCC;

module.exports = function(work, arg, cb){
	if('function' !== typeof work) throw('argument[0] must be a function');
	if('object' !== typeof arg) throw('argument[1] must be an object');
	cb = cb || function(){};

	arg = JSON.stringify(arg);
	work = '('+work.toString()+')('+arg+')';
	libuvThreadCC(work,cb);
}
