#include <iostream>
#include <vector>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif
extern bool GoogleTtsInit(const char* pipeline_path, const char* path_prefix);
extern bool GoogleTtsInstallVoice(const char* voice_name, const uint8_t* voice_bytes, int size);
extern bool GoogleTtsInitBuffered(const uint8_t* text_jspb, const char* speaker_name, int text_jspb_len);
extern size_t GoogleTtsGetFramesInAudioBuffer();
extern int GoogleTtsReadBuffered(float* audio_channel_buffer, size_t* frames_written);
extern void GoogleTtsFinalizeBuffered();
extern void GoogleTtsShutdown();
#ifdef __cplusplus
}
#endif

using namespace std;

int put_int(char *s, int i){
	int len = 1;
	while (i >= 0x80){
		*s = (i & 0x7F) | 0x80;
		++s, ++len;
		i = i >> 7;
	}
	*s = i;
	return len;
}

int main(int argc, char *argv[]) {
	int text_idx = 1;
	string path_prefix= "./en-us/"; // pipeline is extracted zvoice archive
	if(argc == 3) {
		path_prefix = argv[1];
		text_idx = 2;
	}
	string pipeline_path = path_prefix + "pipeline.pb";
	GoogleTtsInit(pipeline_path.c_str(), path_prefix.c_str());

	char* text = argv[text_idx];
	int tlen = strlen(text);
	const char footer[] = "\242\001\n\025\0\0\200?\035\0\0\200?";
	int flen = sizeof(footer) - 1;
	char header[16];
	int hlen = 0;

	int mlen  = tlen + put_int(header, tlen) + 1 + flen;
	int pblen = mlen + put_int(header, mlen) + 1; // + 1 = '\n'
	header[hlen++] = '\n';
	hlen += put_int(header + hlen, pblen);
	header[hlen++] = '\n';
	hlen += put_int(header + hlen, mlen);
	header[hlen++] = '\n';
	hlen += put_int(header + hlen, tlen);

	int text_jspb_len = hlen + tlen + flen;
	uint8_t *text_jspb = (uint8_t*)malloc(text_jspb_len * sizeof(uint8_t));
	memcpy(text_jspb, header, hlen);
	memcpy(text_jspb +hlen, text, tlen);
	memcpy(text_jspb +hlen +tlen, footer, flen);

	GoogleTtsInitBuffered(text_jspb, "hol", text_jspb_len);

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

