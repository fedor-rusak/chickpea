#if defined(_MSC_VER)
	#include <windows.h>
#else
	#include <unistd.h>
	#define Sleep(x) usleep(x)
#endif

#include <iostream>

#include <engine/js_wrapper.hpp>

#define duk_register(ctx, jsName, functionName, argsCount)  \
	duk_push_c_function(ctx, functionName, argsCount);       \
	duk_put_global_string(ctx, jsName);

namespace js_wrapper {

	static duk_context *ctx;

	static int counterBeforeGC = 0;

	static duk_ret_t native_print(duk_context *ctx) {
		printf("%s\n", duk_to_string(ctx, 0));
		return 0;  /* no return value (= undefined) */
	}

	void init() {
		ctx = duk_create_heap_default();
		duk_register(ctx, "print", native_print, 1);
		duk_peval_string(ctx, "print('JS is working!');");
	}

	void defineExtension(char* name, void (*callback)(JXResult*, int)) {		
		//JX_DefineExtension(name, callback);
	}

	double (*getTimeCallback)();

	void getTime(JXValue *results, int argc) {
		//JX_SetDouble(&results[argc], getTimeCallback());
	}

	void setTimeCallback(double (*callback)()) {
		getTimeCallback = callback;
		//JX_DefineExtension("getTime", getTime);
	}

	void start() {
		char *startScript =
			"\"use strict\";"
			"try {"
			"	var init = require('./resources/js/init.js');"
			"	init.setup(global);"
			"}"
			"catch(e) {"
			"	console.log('Failure with ./resources/js/init.js:\\n' + e);"
			"}";
		//JX_DefineMainFile(startScript);
		//JX_StartEngine();
	}

	int loopOnceIOEvents() {
		return 0;//JX_LoopOnce();
	}

	void approachGC(int counts) {
		if  (counterBeforeGC == 5000) {
			//JX_ForceGC();
			counterBeforeGC = 0;
		}
		else {
			counterBeforeGC += counts;
		}
	}

	void terminate() {
		duk_destroy_heap(ctx);
	}

}