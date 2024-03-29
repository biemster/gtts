#!/usr/bin/env python3
import sys,os
import ctypes
import tts_pb2 as Utterance # protoc tts.proto --python_out=.
import speaker_pb2 as Speaker # protoc speaker.proto --python_out=.

c_float_p = ctypes.POINTER(ctypes.c_float)
c_int_p = ctypes.POINTER(ctypes.c_int)

class GTTS():
    def __init__(self, path=os.path.abspath(os.path.dirname(__file__)) + '/en-us-x-multi-r30/', pipeline='pipeline.pb'):
        self.ttslib = ctypes.CDLL(os.path.abspath(os.path.dirname(__file__)) + '/libchrometts.so')
        self.ttslib.GoogleTtsInit(bytes((path + pipeline).encode('utf8')), bytes(path.encode('utf8')))

    def speak(self, utterance, speaker=['hol','female']):
        u = Utterance.Utterance()
        u.a.b.text = utterance
        u.a.b.params.pitch = 1.
        u.a.b.params.speed = 1.
        text_jspb = u.SerializeToString()

        v = Speaker.Speaker()
        v.name = speaker[0]
        v.gender = speaker[1]
        speaker_jspb = v.SerializeToString()
        self.ttslib.GoogleTtsInitBuffered(text_jspb, speaker_jspb, len(text_jspb), len(speaker_jspb))

        audio_buffer = (ctypes.c_char * (self.ttslib.GoogleTtsGetFramesInAudioBuffer() *4))() # GoogleTtsReadBuffered uses floats, float is 4x bigger than char
        frames_in_buf = (ctypes.c_int * 1)()
        while self.ttslib.GoogleTtsReadBuffered(ctypes.cast(audio_buffer, c_float_p), ctypes.cast(frames_in_buf, c_int_p)) > 0:
            sys.stdout.buffer.write(bytes(audio_buffer[:]))


if __name__ == '__main__':
    client = GTTS()
    client.speak('Hello from Python')
    client.speak('Hello from Python', ['tpf','female'])
