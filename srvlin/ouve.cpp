/*
Criado por Marcelo Maurin Martins
Data: 15/06/2021
*/
 
#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include <sys/time.h>  
#include <signal.h>  
#include <setjmp.h> 
#include <assert.h> 
#include <sys/select.h>

#include <pocketsphinx/pocketsphinx.h>
#include <sphinxbase/err.h>  
//generic live audio interface for recording and playback  
#include <sphinxbase/ad.h> 
#include <alsa/asoundlib.h>




#define PORTSRV 7092

ps_decoder_t *ps;
cmd_ln_t *config;
FILE *fh;
char const *hyp, *uttid;
int16 buf[512];
int rv;
int32 score;
snd_pcm_t *capture_handle;

ad_rec_t *ad;
int16 adbuf[2048];
uint8 utt_started, in_speech;
int32 k;
//char const *hyp;
int err;

int Receive_Voice();
static void sleep_msec(int32 ms);

void Ouvir(char * frase) {
	Receive_Voice();
	sprintf(frase,"Recognized: %s\n", hyp);
}

int Start_Sphinx(){
	//1, initialize: create a configuration object cmd_ln_t *  
    // cmd_ln_init function The first parameter is the last configuration we need to update, because this is the first time to create, so pass NULL;  
    // The second argument is an array defining the arguments. If you are using a standard set of arguments, you can get it by calling ps_args().  
    // The third parameter is a flag, which determines whether the interpretation of the parameter is strict, if it is TRUE, then encounter repeated or unknown parameters  
    //, will cause the explanation to fail;  
    //MODELDIR This macro specifies the path of the model, including the acoustic model, language model and dictionary files, which are passed in by the gcc command line.  
    //We get the modeldir variable from the PocketSphinx configuration via the pkg-config tool.  	
	config = cmd_ln_init(NULL, ps_args(), TRUE,
		         "-hmm", MODELDIR "./model/en-us/en-us",
		         "-lm", MODELDIR "./model/en-us/en-us.lm.bin",
	    		 "-dict", MODELDIR "./model/en-us/cmudict-en-us.dict",
		         NULL);
    if (config == NULL) {
	fprintf(stderr, "Failed to create config object, see log for details\n");
	return -1;
    }
	
	//2, initialize the decoder (language recognition is a decoding process, the popular one will be to decode what you said into the corresponding text string)  
    ps = ps_init(config);
    if (ps == NULL) {
	fprintf(stderr, "Failed to create recognizer, see log for details\n");
	return -1;
    }

	return 0;
	
}

/* Sleep for specified msec */
static void sleep_msec(int32 ms) {
#if (defined(_WIN32) && !defined(GNUWINCE)) || defined(_WIN32_WCE)
    Sleep(ms);
#else
    /* ------------------- Unix ------------------ */
    struct timeval tmo;

    tmo.tv_sec = 0;
    tmo.tv_usec = ms * 1000;

    select(0, NULL, NULL, NULL, &tmo);
#endif
}

/*
 * Main utterance processing loop:
 *     for (;;) {
 *        start utterance and wait for speech to process
 *        decoding till end-of-utterance silence will be detected
 *        print utterance result;
 *     }
 */
static void
recognize_from_microphone()
{
    ad_rec_t *ad;
    int16 adbuf[2048];
    uint8 utt_started, in_speech;
    int32 k;
    char const *hyp;

    //if ((ad = ad_open_dev(cmd_ln_str_r(config, "-adcdev"),
	if ((ad = ad_open_dev("hw:0,0",
                          (int) cmd_ln_float32_r(config,
                                                 "-samprate"))) == NULL)
        E_FATAL("Failed to open audio device\n");
    if (ad_start_rec(ad) < 0)
        E_FATAL("Failed to start recording\n");

    if (ps_start_utt(ps) < 0)
        E_FATAL("Failed to start utterance\n");
    utt_started = FALSE;
    E_INFO("Ready....\n");

    for (;;) {
        if ((k = ad_read(ad, adbuf, 2048)) < 0)
            E_FATAL("Failed to read audio\n");
        ps_process_raw(ps, adbuf, k, FALSE, FALSE);
        in_speech = ps_get_in_speech(ps);
        if (in_speech && !utt_started) {
            utt_started = TRUE;
            E_INFO("Listening...\n");
        }
        if (!in_speech && utt_started) {
            /* speech -> silence transition, time to start new utterance  */
            ps_end_utt(ps);
            hyp = ps_get_hyp(ps, NULL );
            if (hyp != NULL) {
                printf("%s\n", hyp);
                fflush(stdout);
            }

            if (ps_start_utt(ps) < 0)
                E_FATAL("Failed to start utterance\n");
            utt_started = FALSE;
            E_INFO("Ready....\n");
        }
        sleep_msec(100);
    }
    ad_close(ad);
}


int Receive_Voice(){
 
	recognize_from_microphone();
	return 0;

}

int Stop_Voice(){
    fclose(fh);
    ps_free(ps);
    cmd_ln_free_r(config);
	return 0;
}


int Audio_device(){
	int resultado = 0;
    

    /* if ((ad = ad_open_dev(cmd_ln_str_r(config, "-adcdev"),
                          (int) cmd_ln_float32_r(config,
                                                 "-samprate"))) == NULL)
												 */
												
	if ((err = snd_pcm_open (&capture_handle, "hw:2", SND_PCM_STREAM_CAPTURE, 0)) < 0) {
    fprintf (stderr, "cannot open audio device \n"   );
    exit (1);
	}												 
  /*
	if ((ad = ad_open_dev("hw:0",44100))==NULL){
        printf("Failed to open audio device\n");
		resultado = -1;
	}
	*/
    if (ad_start_rec(ad) < 0){
        printf("Failed to start recording\n");
		resultado = -2;
	}
    if (ps_start_utt(ps) < 0){
        printf("Failed to start utterance\n");
		resultado = -3;
	}
    utt_started = FALSE;
    printf("Ready....\n");
	return resultado;
		
}

int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c , read_size;
    struct sockaddr_in server , client;
    char client_message[2000];
	if(Audio_device()<0) { /*Inicializa audio device*/
		exit(-1);
	}
    if(Start_Sphinx()){ /*Inicializa Sphinx*/
		exit(-1);
	}
     
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1) {
        printf("Could not create socket");
		exit(-1);
    }
    puts("Socket created");
     

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( PORTSRV );
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        //return 1;
		exit(-1);
    }
    puts("bind done");
    int flgAtivo = 1;
    while(flgAtivo==1)     
    {
		//Listen
		listen(socket_desc , 3);
     
		//Accept and incoming connection
		puts("Waiting for incoming connections...");
		c = sizeof(struct sockaddr_in);
		//while(flgAtivo==1)
 
		//accept connection from an incoming client
		client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
		if (client_sock < 0)
		{
			perror("accept failed");
			return 1;
		}
		puts("Connection accepted");
    
		//Receive a message from client
		while( (read_size = recv(client_sock , client_message , 2000 , 0)) > 0 ) {
			memset(client_message,'\0',sizeof(client_message));
			Ouvir(client_message);
			printf("%s\n",client_message);
		}
     
		if(read_size == 0)
		{
			puts("Client disconnected");
			fflush(stdout);
		} else if(read_size == -1) {
			perror("recv failed");
		}
    }	 
	Stop_Voice();
    return 0;
}
