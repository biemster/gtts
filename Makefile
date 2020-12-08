CC=g++
LDFLAGS=-L. -Wl,-rpath,. -lchrometts

gtts: gtts.c
	$(CC) -o $@ $< $(LDFLAGS)
