"use strict";

function setup(globalContext) {
	globalContext.FPS = 60;
	globalContext.timeFrame = 1000/globalContext.FPS;

	globalContext.arrayData =
		[
			0,0,3.0
		];

	globalContext.speed = 0.1;
	globalContext.move = function(startIndex, sign) {
		globalContext.arrayData[startIndex] += sign*globalContext.speed;
	};

	globalContext.processInput = function() {
		var input = process.natives.getInput();

		console.log(input.pointer);

		if (input.escape === true || input.closeWindow === true)
			globalContext["exit"] = true;

		if (input.left === true)
			globalContext.move(0,1);
		if (input.right === true)
			globalContext.move(0,-1)
		if (input.up === true)
			globalContext.move(1,-1);
		if (input.down === true)
			globalContext.move(1,1);
	}
}

try {
    module.exports = {
    	"setup": setup
    };
}
catch(e) {
    //used without module loader
}