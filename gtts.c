#include <iostream>
#include <fstream>
#include <cstddef>
#include <vector>

#ifdef __cplusplus
extern "C" {
#endif
extern void GoogleTtsSetLogger(void (*logger_func)(int severity, const char* message));
extern bool GoogleTtsInit(const char* pipeline_path, const char* path_prefix);
extern bool GoogleTtsInstallVoice(const char* voice_name, const uint8_t* voice_bytes, int size);
extern bool GoogleTtsInitBuffered(const uint8_t* text_jspb, int text_jspb_len);
extern size_t GoogleTtsGetFramesInAudioBuffer();
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

	string path_prefix= "./en-us/"; // pipeline is extracted zvoice archive
	string pipeline_path = path_prefix + "pipeline";
	GoogleTtsInit(pipeline_path.c_str(), path_prefix.c_str());

	string text = "Hi there!";
	char tlen = text.size();
	const char *footer = "\242\001\n\025\0\0\200?\035\0\0\200?";
	string jspb = to_string(tlen) + text + footer;
	char jspb_len = jspb.size();
	jspb = string("\n") + (char)(jspb_len +2) + "\n" + jspb_len + jspb;
	// vector<uint8_t> text_jspb(jspb.begin(), jspb.end());
	vector<char> text_jspb = { '\n', '\026', '\n', '\024', '\n', '\005', 'h', 'e', 'l', 'l', 'o', '\242', '\001', '\n', '\025', '\0', '\0', '\200', '?', '\035', '\0', '\0', '\200', '?' };
	GoogleTtsInitBuffered((uint8_t*)(&text_jspb[0]), text_jspb.size());

	ofstream audio_file;
	audio_file.open("audio.raw", ios::out | ios::binary);
	vector<float> audio_buffer;
	audio_buffer.resize(GoogleTtsGetFramesInAudioBuffer());
	size_t frames_in_buf = 0;
	int status = 1;
	while(status > 0) {
		status = GoogleTtsReadBuffered(&audio_buffer[0], &frames_in_buf);
		audio_buffer.resize(frames_in_buf);
		audio_file.write(reinterpret_cast<char*>(&audio_buffer[0]), audio_buffer.size()*sizeof(float));
	}
	audio_file.close();

	GoogleTtsFinalizeBuffered();
	GoogleTtsShutdown();


	return 0;
}

void handleLibraryLogging(int severity, const char* msg) {
	cout << severity << ": " << msg << endl;
}
