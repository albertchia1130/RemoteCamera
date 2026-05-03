
#include <arpa/inet.h>
#include <netinet/in.h> //structure for storing address information 
#include <stdio.h> 
#include <stdlib.h> 
#include <sys/socket.h> //for socket APIs 
#include <sys/types.h> 
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <syslog.h>


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT 8080
#define IP_ADDR "192.168.10.106"

static void*  ReadingThread();

int ConnectSocket;

int main() 
{
    
    pthread_t video_thread_id;
    int iResult;
    char userMSG[DEFAULT_BUFLEN];
    char serMsg[DEFAULT_BUFLEN];
    struct sockaddr_in servAddr;
    char ipaddress[DEFAULT_BUFLEN];


    printf("input ip of the server:");
    fgets(ipaddress, DEFAULT_BUFLEN, stdin);


    ConnectSocket = socket(AF_INET, SOCK_STREAM, 0);
    servAddr.sin_family = AF_INET;
    servAddr.sin_port= htons(DEFAULT_PORT); // use some unused port number
    servAddr.sin_addr.s_addr = inet_addr(IP_ADDR);

    // Connect to server.
    iResult = connect( ConnectSocket,(struct sockaddr*)&servAddr,
                  sizeof(servAddr));
    if (iResult == -1) {
        printf("Connection failed\n");
        return 0;
    }

    

    printf("Connection Succeded\n");
    iResult = recv(ConnectSocket, serMsg, sizeof(serMsg), 0);

    if(iResult != -1)
    {
        if(strcmp(serMsg,"VideoOK")== 0)
        {
            printf("prepare To receive \n");
            sleep(1);
            pthread_create(&video_thread_id, NULL, &ReadingThread,NULL);
            while(1)
            {
                fgets(userMSG, DEFAULT_BUFLEN, stdin);
                if(strcmp(userMSG,"exit")==0)
                {
                    send( ConnectSocket, "EXIT", DEFAULT_BUFLEN, 0 );
                    break;
                }
            }
        }
    }
    else{
        printf("Video not available/n");
    }

    return 0;
}

static void*  ReadingThread() 
{ 
    printf("video stream running");
    system("ffplay -i rtsp://192.168.10.106:8554/my");
    return 0;
}