
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include<unistd.h>
#define size

int main()
{   char *ip="127.0.0.1";
    int port=5619;
    int socket_desc;
    struct sockaddr_in server_addr; 
    struct sockaddr_in client_addr;
    char server_message[200];
    char client_message[200];
    int client_struct_length = sizeof(client_addr);
    int count=0;
    
    // Create UDP socket:
    socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(socket_desc < 0)
    {
     	perror("[-]Error while creating socket\a\n");
        return -1;
    }
    printf("[+]Socket created successfully\a\n");
    
    // Set port and IP:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);
    
    // Bind to the set port and IP:
    if(bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("[-]Couldn't bind to the port\a\n");
        return -1;
    }
    printf("[+] binding successfull\n");
    printf("[+]Listening for incoming messages... \a\n\n");
    printf("...type \"exit\" to quit.....\a\n.");
    
    while(1)
  {

    // Clean buffers:
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));
   
    // Receive client's message:
    if (recvfrom(socket_desc, client_message, sizeof(client_message), 0,
         (struct sockaddr*)&client_addr, &client_struct_length) < 0)
    {
        perror("[-]Couldn't receive\n");
        return -1;
    }

    // client ip and port information:
    if(count==0)
    {
    printf(".......Received message from IP: %s and port: %i...........\n",
           inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    count++;
    }
    printf("[+]client message: %s\n", client_message);
    
    // Respond to client:
    printf("[+]server message: ");
    fgets(server_message,200,stdin);

    if (sendto(socket_desc, server_message, strlen(server_message), 0,
         (struct sockaddr*)&client_addr, client_struct_length) < 0){
        perror("Can't send\n");
        return -1;
    }
 
    // server exit   
    if(strcmp(server_message, "exit\n") == 0)
    { 
        printf(".....server exited");
	goto close_label;

    }
  }

   
close_label:
{
  // Close the socket:
    close(socket_desc);
}    
    return 0;
}
