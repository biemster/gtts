# gtts
Google Chrome Text to Speech command line client

##### Intro:
This is a proof of concept how to write code against the libchrometts library found in Chrome OS and Android.
It's not a full application, but it will write out raw audio to stdout which can then be piped to for example ALSA or SoX.

##### Prepare:
Get a copy of googletts-\<version\>.tar.xz, 14.9.1 is the latest supported on this branch (it's the same as 14.7 and 14.8).
The version jumped to 25 on march 12th and changed to a protobuf with speaker params and will not work with this version of the client
Unpack at least one of the `zvoice` files (which are just zip files, `unzip` can handle them), and have the `path_prefix` variable in the code point at it.
Also symlink the proper `libchrometts_<platfrom>.so` from the tarball here where the code lives.

##### Compile:
```
make
```

##### Run:
```
./gtts "Hello from Google Text to Speech!" | aplay -r24000 -fFLOAT_LE -c1
```
or using python:
```
./gtts.py | aplay -r24000 -fFLOAT_LE -c1
```
