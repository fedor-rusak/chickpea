#include "jx.h"

#include "duktape.h"

namespace jx_wrapper {

	void init();

	void setTimeCallback(double (*)());

	void defineExtension(char*, void (*)(JXResult*, int));

	void start();

	int loopOnceIOEvents();

	void approachGC(int);

	void terminate();
}