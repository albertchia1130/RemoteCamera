

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

int ClientIndex = 0;
int clientSocket =0;
int servSockD;

typedef struct ClientAttr{
    struct ClientAttr* nextClient;
    int ClientID;

}Client_number;

int CreateNAttachClient(int clientSocket); //Create a thread for each client attached
int FreeNDetachClient(Client_number* Client); //Exit thread and free up resource when client exit
void BroadcastMessage(int user, char* Message); //broadcast message received from a client to all subscriber
static void* ClientMessageFunc(void *arg);

Client_number* HeadListofClient; //Starting point of client list;
  
int main(int argc, char const* argv[]) 
{ 
  
    // create server socket similar to what was done in 
    // client program 
    servSockD = socket(AF_INET, SOCK_STREAM, 0); 
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
    
    clientSocket = accept(servSockD, NULL, NULL);
    syslog(LOG_INFO, "New User");
    printf("A new connection\n");
    system("ffmpeg -f v4l2 -i /dev/video0   -vcodec libx264 -preset veryfast -tune zerolatency   -f rtsp rtsp://localhost:8554/my "); //Attach client Socket to the list

    return 0; 
}

