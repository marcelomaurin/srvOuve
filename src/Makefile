#HEADERS = program.h headers.h

CC=gcc
SPHINXFLAGS=`pkg-config --cflags --libs pocketsphinx sphinxbase`
SPHINXLIBS=-DMODELDIR=\"`pkg-config --variable=modeldir pocketsphinx`\"

TARGET=srvouve
SOURCE= ouve.c

default: all

all: clean compile install

clean:
	rm -f ./srvOuve
	rm -f /usr/local/bin/srvOuve


compile:
	$(CC) $(SOURCE) -g -o $(TARGET) $(SPHINXFLAGS) $(SPHINXLIBS)
	
	
install:
	cp  ./$(TARGET) /usr/bin/ 
	cp ./$(TARGET) ./../srvouve/usr/bin/
	
run:
	/usr/bin/$(TARGET)



