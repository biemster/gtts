#!/usr/bin/env python3
import sys
import ctypes
import tts_146_pb2 as Utterance # protoc tts_146.proto --python_out=.

c_float_p = ctypes.POINTER(ctypes.c_float)
c_int_p = ctypes.POINTER(ctypes.c_int)

class GTTS():
    def __init__(self, path='./en-us/', pipeline='pipeline'):
        self.ttslib = ctypes.CDLL('./libchrometts.so')
        self.ttslib.GoogleTtsInit(bytes((path + pipeline).encode('utf8')), bytes(path.encode('utf8')))

    def speak(self, utterance):
        u = Utterance.Utterance()
        u.a.b.text = utterance
        u.a.b.params.pitch = 1.
        u.a.b.params.speed = 1.
        text_jspb = u.SerializeToString()
        #text_jspb = bytes([0x0a, 0x15, 0x0a, 0x13, 0x0a, 0x04, 0x74, 0x65, 0x73, 0x74, 0xa2, 0x01, 0x0a, 0x15, 0x00, 0x00, 0x80, 0x3f, 0x1d, 0x00, 0x00, 0x80, 0x3f]) # test
        self.ttslib.GoogleTtsInitBuffered(text_jspb, len(text_jspb))

        audio_buffer = (ctypes.c_char * (self.ttslib.GoogleTtsGetFramesInAudioBuffer() *4))() # GoogleTtsReadBuffered uses floats, float is 4x bigger than char
        frames_in_buf = (ctypes.c_int * 1)()
        while self.ttslib.GoogleTtsReadBuffered(ctypes.cast(audio_buffer, c_float_p), ctypes.cast(frames_in_buf, c_int_p)) > 0:
            sys.stdout.buffer.write(bytes(audio_buffer[:]))


if __name__ == '__main__':
    client = GTTS()
    client.speak('Hello from Python')