#include <iostream>
#include <cstddef>

#ifdef __cplusplus
extern "C" {
#endif
extern void GoogleTtsSetLogger(void (*logger_func)(int severity, const char* message));
extern bool GoogleTtsInit();
extern bool GoogleTtsInstallVoice();
extern bool GoogleTtsInitBuffered();
extern int GoogleTtsReadBuffered();
extern void GoogleTtsFinalizeBuffered();
extern void GoogleTtsShutdown();
#ifdef __cplusplus
}
#endif

using namespace std;

void handleLibraryLogging(int severity, const char* msg);

int main(int argc, char *argv[]) {
	GoogleTtsSetLogger(handleLibraryLogging);

	
	return 0;
}

void handleLibraryLogging(int severity, const char* msg) {
	cout << severity << ": " << msg << endl;
}
