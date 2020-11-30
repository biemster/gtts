# gtts
Google Chrome Text to Speech command line client

##### Intro:
This is a proof of concept how to write code against the libchrometts library found in Chrome OS and Android.
It's not a full application, but it will write out a raw audio file of the input text.

##### Prepare:
Get a copy of googletts-<version>.tar.gz, 14.6 is the latest at the time of this writing. Google will give good results where to find it.
Unpack at least one of the `zvoice` files, and have the `path_prefix` variable in the code point at it.

##### Compile:
```
make
```

##### Run:
```
./gtts "Hello from Google Text to Speech!"
```

##### Play audio:
```
aplay -r22050 -fFLOAT_LE -c1 audio.raw
```
