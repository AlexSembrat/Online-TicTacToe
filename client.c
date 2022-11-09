#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

int main(){

  //request server IP and Port
  char IP[30];
  char porty[30];
  printf("Enter the IP of the server\n");
  scanf("%s",IP);
  printf("Enter the Port of the server\n");
  scanf("%s",porty);
  int port = atoi(porty);

  //socket setup
  int Socket;
	Socket = socket(AF_INET, SOCK_STREAM, 0);

	if(Socket==-1){
		printf("Socket Call Failed\n");
	}

  //server address setup
	struct sockaddr_in indirection_address;
	indirection_address.sin_family = AF_INET;
	indirection_address.sin_port = htons(port);
	indirection_address.sin_addr.s_addr = inet_addr(IP); //INADDR_ANY;

  //connect to server
	int status = connect(Socket, (struct sockaddr *) &indirection_address, sizeof(indirection_address));

  if(status==-1){
		printf("Connection Failed\n");
	}
  else{
    printf("Connection Initialized\n");
  }

  //Setup message variables
  char my_message[256];
  char server_message[256];
  int my_turn = 0;
  int original_turn = 0;
  char play_again[1];
  
  //ensure the space allocated for variables is empty
  memset(my_message,0,sizeof(my_message));
  memset(server_message,0,sizeof(server_message));

  printf("Waiting for other player to connect...\n");
  recv(Socket, &server_message, sizeof(server_message),0);
  printf("%s\n",server_message );
  
  if(strcmp(server_message,"Hi Player 1\nGame Start Player 1 Go First")==0){
	  my_turn = 0;
	  original_turn = 0;
  }
  else if(strcmp(server_message,"Hi Player 2\nGame Start Player 1 Go First")==0){
	  my_turn = 1;
	  original_turn = 1;
  }

  while(1){
   //receive board from server
   recv(Socket, &server_message, sizeof(server_message),0);
   printf("%s\n",server_message);
   
   //check for win/tie, else keep playing
   if(strstr(server_message,"Wins")!=NULL){
	   printf("Play Again? (Y/N)\n");
	   scanf("%s", play_again);
	   if(play_again[0] == 'Y'){
		   my_turn = original_turn;
	   }
	   else{
		   exit(1);
		}
   }
   else if(strstr(server_message,"Tie")!=NULL){
	   printf("Play Again? (Y/N)\n");
	   scanf("%s", play_again);
	   if(play_again[0] == 'Y'){
		   my_turn = original_turn;
	   }
	   else{
		   exit(1);
		}
   }
   else{
   
	   if(my_turn==0){
		  printf("Please enter move.\n");
		  scanf("%s", my_message);
		  my_message[strlen(my_message)] = '\0';
		  send(Socket, my_message, strlen(my_message), 0);
		  my_turn = 1;
	   }
	   else{
		   printf("Please wait...\n");
		   my_turn = 0;
	   }


		
   }
	//clear strings
		memset(my_message,0,sizeof(my_message));
		memset(server_message,0,sizeof(server_message));
   
  }


}
