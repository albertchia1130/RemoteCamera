

#include <netinet/in.h> //structure for storing address information 
#include <stdio.h> 
#include <stdlib.h> 
#include <sys/socket.h> //for socket APIs 
#include <sys/types.h> 
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include <syslog.h>

#define messageSize 255


static void* ffmpeg_stream()
{
    system("ffmpeg -f v4l2 -i /dev/video0   -vcodec libx264 -preset veryfast -tune zerolatency   -f rtsp rtsp://localhost:8554/my "); //Attach client Socket to the list
}


  
int main(int argc, char const* argv[]) 
{ 
    pthread_t thread_id;  
    int iResult;
    char serMsg[messageSize];
    // create server socket similar to what was done in 
    // client program 
    int servSockD = socket(AF_INET, SOCK_STREAM, 0); 
    // define server address 
    struct sockaddr_in servAddr; 
  
    servAddr.sin_family = AF_INET; 
    servAddr.sin_port = htons(8080); 
  
    // bind socket to the specified IP and port 
    bind(servSockD, (struct sockaddr*)&servAddr, sizeof(servAddr)); 
  
    // listen for connections 
    printf("phase 1\n");
    listen(servSockD, SOMAXCONN); 
    printf("Entering Listen\n");
    
    int clientSocket = accept(servSockD, NULL, NULL);
    syslog(LOG_INFO, "New User");
    printf("A new connection\n");
    system("ffmpeg -f v4l2 -i /dev/video0   -vcodec libx264 -preset veryfast -tune zerolatency   -f rtsp rtsp://localhost:8554/my "); //Attach client Socket to the list
    pthread_create(&thread_id, NULL, ffmpeg_stream, NULL);
    send( clientSocket, "VideoOK", messageSize, 0 );
    while(1)
    {
         iResult = recv(servSockD, serMsg, sizeof(serMsg), 0);

        if(iResult != -1 && strcmp(serMsg,"Video_stop")== 0)
        {
            system("killall -INT ffmpeg");
            pthread_join(thread_id, NULL); // Wait for thread to finish
            printf("line finished\n");
            break;
        }
        
    }

    return 0; 
}

