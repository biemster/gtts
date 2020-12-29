#!/usr/bin/env python3
import sys
import ctypes
import tts_pb2 as Utterance # protoc tts.proto --python_out=.

c_float_p = ctypes.POINTER(ctypes.c_float)
c_int_p = ctypes.POINTER(ctypes.c_int)

class GTTS():
    def __init__(self, path='./en-us-x-multi/', pipeline='pipeline.pb'):
        self.ttslib = ctypes.CDLL('./libchrometts.so')
        self.ttslib.GoogleTtsInit(bytes((path + pipeline).encode('utf8')), bytes(path.encode('utf8')))

    def speak(self, utterance, speaker='hol'):
        u = Utterance.Utterance()
        u.a.b.text = utterance
        u.a.b.params.pitch = 1.
        u.a.b.params.speed = 1.
        text_jspb = u.SerializeToString()
        self.ttslib.GoogleTtsInitBuffered(text_jspb, speaker.encode('utf8'), len(text_jspb))

        audio_buffer = (ctypes.c_char * (self.ttslib.GoogleTtsGetFramesInAudioBuffer() *4))() # GoogleTtsReadBuffered uses floats, float is 4x bigger than char
        frames_in_buf = (ctypes.c_int * 1)()
        while self.ttslib.GoogleTtsReadBuffered(ctypes.cast(audio_buffer, c_float_p), ctypes.cast(frames_in_buf, c_int_p)) > 0:
            sys.stdout.buffer.write(bytes(audio_buffer[:]))


if __name__ == '__main__':
    client = GTTS()
    client.speak('Hello from Python')
