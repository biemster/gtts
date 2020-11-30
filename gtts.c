#include <iostream>
#include <vector>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif
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

int main(int argc, char *argv[]) {
	string path_prefix= "./en-us/"; // pipeline is extracted zvoice archive
	string pipeline_path = path_prefix + "pipeline";
	GoogleTtsInit(pipeline_path.c_str(), path_prefix.c_str());

	char* text = argv[1];
	char tlen = strlen(text);
	const char *footer = "\242\001\n\025\0\0\200?\035\0\0\200?";
	int flen = 15;
	char mlen = tlen +flen;
	char pblen = mlen +2;
	char header[] = {'\n', pblen, '\n', mlen, '\n', tlen};
	int hlen = 6;
	int text_jspb_len = (hlen -2) + tlen + flen; // this is a mess
	uint8_t *text_jspb = (uint8_t*)malloc((text_jspb_len +2)*sizeof(uint8_t));
	memcpy(text_jspb, header, 6);
	memcpy(text_jspb +6, text, tlen);
	memcpy(text_jspb +6 +tlen, footer, flen);
	GoogleTtsInitBuffered(text_jspb, text_jspb_len);

	vector<float> audio_buffer;
	audio_buffer.resize(GoogleTtsGetFramesInAudioBuffer());
	size_t frames_in_buf = 0;
	int status = 1;
	while(status > 0) {
		status = GoogleTtsReadBuffered(&audio_buffer[0], &frames_in_buf);
		audio_buffer.resize(frames_in_buf);
		cout.write(reinterpret_cast<char*>(&audio_buffer[0]), audio_buffer.size()*sizeof(float));
	}

	GoogleTtsFinalizeBuffered();
	GoogleTtsShutdown();
	delete text_jspb;


	return 0;
}
