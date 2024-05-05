#include "state.h"

#ifdef _WIN32
#	include "run_win32.h"
#else
#	error "Unsupported platform."
#endif

int main() {
	State state(4096, 64.0, 20.0);
	return run(state);
}