#if defined(_MSC_VER)
	#include <windows.h>
#else
	#include <unistd.h>
	#define Sleep(x) usleep(x)
#endif

#include <engine/jx_wrapper.hpp>

namespace jx_wrapper {

	static int counterBeforeGC = 0;

	void init() {
		// Call JX_Initialize only once per app
		JX_Initialize("some_name", NULL);

		JX_InitializeNewEngine();
	}

	void defineExtension(char* name, void (*callback)(JXResult*, int)) {		
		JX_DefineExtension(name, callback);
	}

	double (*getTimeCallback)();

	void getTime(JXValue *results, int argc) {
		JX_SetDouble(&results[argc], getTimeCallback());
	}

	void setTimeCallback(double (*callback)()) {
		getTimeCallback = callback;
		JX_DefineExtension("getTime", getTime);
	}

	void start() {
		char *startScript =
			"\"use strict\";"
			"try {"
			"	var init = require('./resources/jxcore/init.js');"
			"	init.setup(global);"
			"}"
			"catch(e) {"
			"	console.log('Failure with ./resources/jxcore/init.js:\\n' + e);"
			"}";
		JX_DefineMainFile(startScript);
		JX_StartEngine();
	}

	int loopOnceIOEvents() {
		return JX_LoopOnce();
	}

	void approachGC(int counts) {
		if  (counterBeforeGC == 5000) {
			JX_ForceGC();
			counterBeforeGC = 0;
		}
		else {
			counterBeforeGC += counts;
		}
	}

	void terminate() {
		JX_StopEngine();
	}

}
