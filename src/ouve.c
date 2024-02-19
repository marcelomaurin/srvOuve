/*
Criado por Marcelo Maurin Martins
Data: 15/06/2021
*/
 
#include<stdio.h>
#include <unistd.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include <pocketsphinx.h>

#include <sphinxbase/ad.h>
#include <sphinxbase/err.h>


#define PORTSRV 8097

ps_decoder_t *ps;
cmd_ln_t *config;
FILE *fh;
//char const *hyp, *uttid;
char const *hyp;                   // pointer to "hypothesis" (best guess at the decoded result)
char const *decoded_speech;
uint8 utt_started, in_speech;      // flags for tracking active speech - has speech started? - is speech currently happening?
int32 k;                           // holds the number of frames in the audio buffer


const char * recognize_from_microphone();
int16 adbuf[4096];                 // buffer array to hold audio data

int16 buf[512];
int rv;
int32 score;
ad_rec_t *ad;                      // create audio recording structure - for use with ALSA functions


void Ouvir(char * frase) {
	//hyp = Receive_Voice();
	
	
	printf("Iniciou Ouvir\n");
	decoded_speech = recognize_from_microphone();
	printf("Hipo: %s\n", hyp);
	printf("Recognized: %s\n",decoded_speech);
	sprintf(frase,"%s\n", hyp);
	printf("Finalizou Ouvir\n");
}


int Start_Voice(){
	
	config = cmd_ln_init(NULL, ps_args(), TRUE,
		         "-hmm", MODELDIR "./pt_BR",
		         "-lm", MODELDIR "./cmusphinx-pt-br-5.2/cmusphinx-pt_BR-pt_BR.lm.bin",
	    		 "-dict", MODELDIR "./cmusphinx-pt-br-5.2/cmudict-pt_BR.dict",
			 NULL);
    if (config == NULL) {
	fprintf(stderr, "Failed to create config object, see log for details\n");
	return -1;
    }

    ps = ps_init(config);
    ad = ad_open_dev("sysdefault", (int) cmd_ln_float32_r(config, "-samprate")); // open default microphone at default samplerate

    if (ps == NULL) {
	fprintf(stderr, "Failed to create recognizer, see log for details\n");
	return -1;
    }
	/*
    fh = fopen("goforward.raw", "rb");
    if (fh == NULL) {
	fprintf(stderr, "Unable to open input file goforward.raw\n");
	return -1;
    }

    rv = ps_start_utt(ps);

	//return 0;
	*/
	/*
   config = cmd_ln_init(NULL, ps_args(), TRUE,                   // Load the configuration structure - ps_args() passes the default values
			 "-hmm", "/usr/local/share/pocketsphinx/model/en-us/en-us",  // path to the standard english language model
			 "-lm", "custom.lm",                                         // custom language model (file must be present)
			 "-dict", "custom.dic",                                      // custom dictionary (file must be present)
			 "-logfn", "/dev/null",                                      // suppress log info from being sent to screen
			  NULL);

   ps = ps_init(config);                                                        // initialize the pocketsphinx decoder
   */
   ad = ad_open_dev("sysdefault", (int) cmd_ln_float32_r(config, "-samprate")); // open default microphone at default samplerate
   return 0;
}


int Receive_Voice(){
    while (!feof(fh)) 
    {
		size_t nsamp;
		nsamp = fread(buf, 2, 512, fh);
		rv = ps_process_raw(ps, buf, nsamp, FALSE, FALSE);
    }
    rv = ps_end_utt(ps);
    hyp = ps_get_hyp(ps, &score);

	return 0;

}

const char * recognize_from_microphone()
{

	ad_start_rec(ad);                                // start recording
	ps_start_utt(ps);                                // mark the start of the utterance
	utt_started = FALSE;                             // clear the utt_started flag
	printf("Reconhecimento de microfone\n");
	while(1) {                                       
	    k = ad_read(ad, adbuf, 4096);                // capture the number of frames in the audio buffer
            ps_process_raw(ps, adbuf, k, FALSE, FALSE);  // send the audio buffer to the pocketsphinx decoder
	    in_speech = ps_get_in_speech(ps);            // test to see if speech is being detected

            if (in_speech && !utt_started) {             // if speech has started and utt_started flag is false                           
	        utt_started = TRUE;                      // then set the flag
	    }

	    if (!in_speech && utt_started) {             // if speech has ended and the utt_started flag is true
                ps_end_utt(ps);                          // then mark the end of the utterance
                ad_stop_rec(ad);                         // stop recording
                hyp = ps_get_hyp(ps, NULL );             // query pocketsphinx for "hypothesis" of decoded statement
                return hyp;                              // the function returns the hypothesis
                break;                                   // exit the while loop and return to main
            }
       }
}



int Stop_Voice()
{
    printf("Parando captura\n");	
    ad_close(ad); 
    //close(fh);
    //ps_free(ps);
    //cmd_ln_free_r(config);
    return 0;
}

void setparametros(int argc, char *argv[]) {
    int opt;
    while ((opt = getopt(argc, argv, "vhc:")) != -1) {
        switch (opt) {
            case 'v':
                printf("Opção -v selecionada\n");
                break;
            case 'h':
                printf("Opção -h selecionada\n");

                break;
            case 'c':
                printf("Opção -c selecionada com o valor: %s\n", optarg);
                break;
            default: /* '?' */
                fprintf(stderr, "Uso: %s [-v] [-h] [-c valor]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }
}

int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c , read_size;
    struct sockaddr_in server , client;
    char client_message[2000];
    // Chamar a função setparametros
    setparametros(argc, argv);

    Start_Voice();
     
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
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
        return 1;
    }
    puts("bind done");
    int flgAtivo = 1;
    printf("Finalizando Start\n");
    while(flgAtivo==1)     
    {
        printf("Flgativo\n");
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
    
		while(1)
		{
			memset(client_message,'\0',sizeof(client_message));
			Ouvir(client_message);
			send(client_sock,client_message,strlen(client_message),0);
			/*
			//Receive a message from client
			while( (read_size = recv(client_sock , client_message , 2000 , 0)) > 0 ) {
				memset(client_message,'\0',sizeof(client_message));			
				printf("%s\n",client_message);
			}
			*/
			if(read_size == 0)
			{
				puts("Client disconnected");
				fflush(stdout);
			} else if(read_size == -1) {
				perror("recv failed");
			}
		}
    }	 
    Stop_Voice();
    return 0;
}
