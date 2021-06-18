#HEADERS = program.h headers.h

CC=gcc
SPHINXFLAGS=`pkg-config --cflags --libs pocketsphinx sphinxbase`
SPHINXLIBS=-DMODELDIR=\"`pkg-config --variable=modeldir pocketsphinx`\"

default: all

all: clean compile install

clean:
	rm -f ./srvOuve
	rm -f /usr/local/bin/srvOuve


compile:
	$(CC)  ouve.c -g -o SRVOuve $(SPHINXFLAGS) $(SPHINXLIBS)
	
	
install:
	cp  ./SRVOuve /usr/local/bin/ 
	
run:
	/usr/local/bin/srvOuve



