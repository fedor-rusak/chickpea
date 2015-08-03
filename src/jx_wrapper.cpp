#if defined(_MSC_VER)
	#include <windows.h>
#else
	#include <unistd.h>
	#define Sleep(x) usleep(x)
#endif

#include <engine/jx_wrapper.hpp>

namespace jx {

	static int counterBeforeGC = 0;

	void init() {
		// Call JX_Initialize only once per app
		JX_Initialize("some_name", NULL);

		JX_InitializeNewEngine();
	}

	void defineExtension(char* name, void (*callback)(JXResult*, int)) {		
		JX_DefineExtension(name, callback);
	}

	void start() {
		char *startScript =
			"\"use strict\";"
			"try {"
			"	var init = require('./init.js');"
			"	init.setup(global);"
			"}"
			"catch(e) {"
			"	console.log('Failure with init.js:\\n' + e);"
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
