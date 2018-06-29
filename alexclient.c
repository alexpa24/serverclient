/*
    C client example using sockets
    Alejandro Palomo Alepuz
*/
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
 
int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
    char message[1000] , server_reply[2000];
    char message2[1000] , server_reply2[2000];
     
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 );
 
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
     
    puts("Connected\n");
     
    //keep communicating with server
    while(1)
    {
        puts("Enter r to read, w to modify the message or t to terminate the server");
        scanf("%s" , message);

        if(!strcmp(message,"s")){
            close(sock);
            return 0;
        }else if(!strcmp(message,"t")){
            if( send(sock , message , strlen(message) , 0) < 0)
            {
                puts("Send failed");
                return 1;
            }
            puts("Server terminated");
            exit(0);
        }else if(!strcmp(message,"w")){
            puts("Enter the message");
            scanf(" %[^\t\n]s",message2);
            puts(message2);
            //Send data to the server
            if( send(sock , message2 , strlen(message2) , 0) < 0)
            {
                puts("Send failed");
                return 1;
            }
            
            //Receive a reply from the server
            if( recv(sock , server_reply2 , 2000 , 0) < 0)
            {
                puts("recv failed");
                break;
            }
            
            puts("Server reply:");
            puts(server_reply2);
        }else{
            //Send data to the server
            if( send(sock , message , strlen(message) , 0) < 0)
            {
                puts("Send failed");
                return 1;
            }
            
            //Receive a reply from the server
            if( recv(sock , server_reply , 2000 , 0) < 0)
            {
                puts("recv failed");
                break;
            }
            
            puts("Server reply:");
            puts(server_reply);
            memset (server_reply, '\0', 2000);
        }
    }
     
    close(sock);
    return 0;
}