#include <iostream>
#include <cstddef>
#include <vector>

#ifdef __cplusplus
extern "C" {
#endif
extern void GoogleTtsSetLogger(void (*logger_func)(int severity, const char* message));
extern bool GoogleTtsInit(const char* pipeline_path, const char* path_prefix);
extern bool GoogleTtsInstallVoice(const char* voice_name, const uint8_t* voice_bytes, int size);
extern bool GoogleTtsInitBuffered(const uint8_t* text_jspb, int text_jspb_len);
extern int GoogleTtsReadBuffered(float* audio_channel_buffer, size_t* frames_written);
extern void GoogleTtsFinalizeBuffered();
extern void GoogleTtsShutdown();
#ifdef __cplusplus
}
#endif

using namespace std;

void handleLibraryLogging(int severity, const char* msg);

int main(int argc, char *argv[]) {
	// GoogleTtsSetLogger(handleLibraryLogging);

	string path_prefix= "./en-us"; // pipeline is extracted zvoice archive
	string pipeline_path = path_prefix + "/pipeline";
	GoogleTtsInit(path_prefix.c_str(), pipeline_path.c_str());

	string text = "text: Hi There!";
	vector<uint8_t> text_jspb(text.begin(), text.end());
	GoogleTtsInitBuffered(&text_jspb[0], text_jspb.size());

	vector<float> audio_buffer;
	size_t frames_in_buf = 0;
	int status = 1;
	while(status > 0) {
		status = GoogleTtsReadBuffered(&audio_buffer[0], &frames_in_buf);
		audio_buffer.resize(frames_in_buf);
	}

	GoogleTtsFinalizeBuffered();
	GoogleTtsShutdown();


	return 0;
}

void handleLibraryLogging(int severity, const char* msg) {
	cout << severity << ": " << msg << endl;
}
