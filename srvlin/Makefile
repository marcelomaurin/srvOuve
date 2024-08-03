#HEADERS = program.h headers.h

CC=gcc
SPHINXFLAGS=`pkg-config --cflags --libs pocketsphinx sphinxbase` 
INCLUDE= -I/usr/local/include/pocketsphinx \
		 -I/usr/local/include/sphinxbase 
		 
		 
SPHINXLIBS=-DMODELDIR=\"`pkg-config --variable=modeldir pocketsphinx`\" \
			-lasound
APP=srvOuve

default: all

all: clean compile install

clean:
	rm -f ./srvOuve
	rm -f /usr/local/bin/srvOuve


compile:
	$(CC)  ouve.cpp -g $(INCLUDE) $(SPHINXFLAGS) $(SPHINXLIBS) -o $(APP)

teste:	
	$(CC)  teste2.c -g $(INCLUDE) $(SPHINXFLAGS) $(SPHINXLIBS) -o teste2
install:
	cp  ./$(APP) /usr/local/bin/ 
	
run:
	/usr/local/bin/$(APP)

play:
	pocketsphinx_continuous -adcdev plughw:0,0 -kws_threshold 1e-20 -inmic yes

play2:
	pocketsphinx_continuous -adcdev hw:0,0 -samprate 48000 -nfft 2048 -hmm ./model/en-us/en-us  -inmic yes

