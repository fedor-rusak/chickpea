#include "jx.h"

namespace jx {

	void init();

	void defineExtension(char*, void (*)(JXResult*, int));

	void start();

	int loopOnceIOEvents();

	void approachGC(int);

	void terminate();
}