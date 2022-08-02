/*
Criado por Marcelo Maurin Martins
Data: 15/06/2021
*/
 
#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include <pocketsphinx.h>



#define PORTSRV 8097

ps_decoder_t *ps;
cmd_ln_t *config;
FILE *fh;
char const *hyp, *uttid;
int16 buf[512];
int rv;
int32 score;

void Ouvir(char * frase) {
	Receive_Voice();
	sprintf(frase,"Recognized: %s\n", hyp);
}

int Start_Voice(){
	config = cmd_ln_init(NULL, ps_args(), TRUE,
		         "-hmm", MODELDIR "/en-us/en-us",
		         "-lm", MODELDIR "/en-us/en-us.lm.bin",
	    		 "-dict", MODELDIR "/en-us/cmudict-en-us.dict",
		         NULL);
    if (config == NULL) {
	fprintf(stderr, "Failed to create config object, see log for details\n");
	return -1;
    }

    ps = ps_init(config);
    if (ps == NULL) {
	fprintf(stderr, "Failed to create recognizer, see log for details\n");
	return -1;
    }

    fh = fopen("goforward.raw", "rb");
    if (fh == NULL) {
	fprintf(stderr, "Unable to open input file goforward.raw\n");
	return -1;
    }

    rv = ps_start_utt(ps);

	return 0;
	
}


int Receive_Voice(){
	while (!feof(fh)) {
		size_t nsamp;
		nsamp = fread(buf, 2, 512, fh);
		rv = ps_process_raw(ps, buf, nsamp, FALSE, FALSE);
    }
	rv = ps_end_utt(ps);
    hyp = ps_get_hyp(ps, &score);

	return 0;

}

int Stop_Voice(){


    fclose(fh);
    ps_free(ps);
    cmd_ln_free_r(config);
	return 0;
}

int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c , read_size;
    struct sockaddr_in server , client;
    char client_message[2000];

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
