/*
    C socket server example
    Alejandro Palomo Alepuz
*/
 
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h>
#include<stdlib.h>

void *connection_handler(void *);
struct sockaddr_in server , client;
char *stored_message;

int main(int argc , char *argv[])
{
	int socket_desc , client_sock , c , *new_sock;

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
	server.sin_port = htons( 8888 );
	
	//Bind
	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
	{
		//print the error message
		perror("bind failed. Error");
		return 1;
	}
	puts("bind done");
	
	//Listen
	listen(socket_desc , 3);
	
	//Accept and incoming connection
	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);
	while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
	{
		puts("Connection accepted");
		
		pthread_t sniffer_thread;
		new_sock = malloc(1);
		*new_sock = client_sock;
		
		if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
		{
			perror("could not create thread");
			return 1;
		}
		puts("Handler assigned");
	}
	
	if (client_sock < 0)
	{
		perror("accept failed");
		return 1;
	}
	
	return 0;
}

void *connection_handler(void *socket_desc)
{
    int sock = *(int*)socket_desc;
    int client_sock , c , read_size;
    char client_message[2000] = "";
    stored_message = (char *) malloc(2000);

    FILE * f = fopen("alexserver.log", "a+");
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen("message.txt", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    if ((read = getline(&line, &len, fp)) != -1) {
        strcpy(stored_message,line);
    }

    fclose(fp);
     
    //Receive a message from client
    while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 )
    {
        if(!strcmp(client_message,"t")){ //terminate server
            puts("Server terminated");
            fclose(f);
            free(socket_desc); 
            free(stored_message);
            exit(0);
        }else if(!strcmp(client_message,"r")){ //read the message
            time_t clk = time(NULL);
            fprintf(f, "Time: %s, Client IP: %s, Read\n", ctime(&clk),inet_ntoa(client.sin_addr));
            write(sock, stored_message , strlen(stored_message));
        }else{ //write the message
            memset (stored_message, '\0', 2000);
            time_t clk = time(NULL);
            fprintf(f, "Time: %s, Client IP: %s, Modify\n", ctime(&clk),inet_ntoa(client.sin_addr));

            write(sock, "Saved" , strlen("Saved"));
            memcpy ( stored_message, client_message, strlen(client_message));

            fp = fopen("message.txt", "w");
            fputs (stored_message,fp);
            fclose(fp);
        }
        memset (client_message, '\0', 2000);
    }
     
    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }

    free(socket_desc); 
    return 0;
}