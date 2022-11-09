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
#include <sys/select.h>

void play_move(char board[], char move[], char shape);
int game_status(char board[]);

int main(){
  //set up a TCP server socket
  int server_socket;
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
  //Address Setup
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(4410);
	server_address.sin_addr.s_addr = INADDR_ANY;
  //Bind socket to address
	bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));
  //listen for connections
	listen(server_socket, 5);
  printf("Listening\n");

  //prepare a client Socket, accept player 1
  int client_socket;
  client_socket = accept(server_socket,NULL,NULL);
  printf("Client Soccet Accepted\n");
  
  //prepare a client Socket, accept player 2
  int client_socket2;
  client_socket2 = accept(server_socket,NULL,NULL);
  printf("Client2 Soccet Accepted\n");
  
  //message variables
  char client_message[256];
  char client_message2[256];
  char player1_move[256];
  char player2_move[256];
  int length = 0;
  int player = 0;
  char shape = 'x';
  int check = -1;
  
  //ensure the space allocated for variables is empty
  memset(client_message,0,sizeof(client_message));
  memset(client_message2,0,sizeof(client_message2));
  memset(player1_move,0,sizeof(player1_move));
  memset(player2_move,0,sizeof(player2_move)); 
 
  //prepare messages to players
  strcpy(client_message,"Hi Player 1\nGame Start Player 1 Go First");  
  client_message[strlen(client_message)] = '\0';
  strcpy(client_message2,"Hi Player 2\nGame Start Player 1 Go First");  
  client_message[strlen(client_message2)] = '\0';
  
  //Send messages to the players
  send(client_socket,client_message, sizeof(client_message),0);
  send(client_socket2,client_message2, sizeof(client_message),0);
  
  //The board
  char board[256];
  strcpy(board,"3  | | \n  -----\n2  | | \n  -----\n1  | | \n  a b c\0");

  while(1){
	//clear variables  
	memset(client_message,0,sizeof(client_message));
    memset(client_message2,0,sizeof(client_message2));
    memset(player1_move,0,sizeof(player1_move));
    memset(player2_move,0,sizeof(player2_move)); 
	
	check = game_status(board);
	
	//copy the board into the client messages
	strcpy(client_message,board);
	strcpy(client_message2,board);
	
	//see if game is over (win or tie)
	if(check==1){
		
		if(player==0){
			strcat(client_message, "\nGame Over, Player 2 Wins");
			strcat(client_message2, "\nGame Over, Player 2 Wins");
		}
		else{
			strcat(client_message, "\nGame Over, Player 1 Wins");
			strcat(client_message2, "\nGame Over, Player 1 Wins");
		}
		
		send(client_socket,client_message, sizeof(client_message),0);
		send(client_socket2,client_message2, sizeof(client_message),0);
		
		length = 0;
		player = 0;
		shape = 'x';
		check = -1;
		strcpy(board,"3  | | \n  -----\n2  | | \n  -----\n1  | | \n  a b c\0");
	}
	else if(check==0){
		strcat(client_message, "\nGame Over We Have A Tie");
		strcat(client_message2,"\nGame Over We Have A Tie");
		printf("%c\n", board[69]);
		
		send(client_socket,client_message, sizeof(client_message),0);
		send(client_socket2,client_message2, sizeof(client_message),0);
		
		length = 0;
		player = 0;
		shape = 'x';
		check = -1;
		strcpy(board,"3  | | \n  -----\n2  | | \n  -----\n1  | | \n  a b c\0");
	}
	else{
		//send the board to the players
		send(client_socket,client_message, sizeof(client_message),0);
		send(client_socket2,client_message2, sizeof(client_message),0);
		
		
		if(player == 0){
			length = recv(client_socket,&player1_move, sizeof(player1_move),0);
			player1_move[length] = '\0';
			play_move(board,player1_move,shape);
			player = 1;
			shape = 'o';
		}
		else{
			length = recv(client_socket2,&player2_move, sizeof(player2_move),0);
			player2_move[length] = '\0';
			play_move(board,player2_move,shape);
			player = 0;
			shape = 'x';
		}  
    }
  }


}

//edits the board when a move is played
void play_move(char board[], char move[], char shape){
	
	if(strcmp(move,"a1")==0){
		board[34] = shape;
	}
	else if(strcmp(move,"a2")==0){
		board[18] = shape;
	}
	else if(strcmp(move,"a3")==0){
		board[2] = shape;
	}
	else if(strcmp(move,"b1")==0){
		board[36] = shape;
	}
	else if(strcmp(move,"b2")==0){
		board[20] = shape;
	}
	else if(strcmp(move,"b3")==0){
		board[4] = shape;
	}
	else if(strcmp(move,"c1")==0){
		board[38] = shape;
	}
	else if(strcmp(move,"c2")==0){
		board[22] = shape;
	}
	else if(strcmp(move,"c3")==0){
		board[6] = shape;
	}
}

//returns winner, tie, or still playing
int game_status(char board[]){
	
	if (board[2] == board[4] && board[4] == board[6] && (board[6] == 'x' || board[6] == 'o'))
        return 1;
        
    else if (board[18] == board[20] && board[20] == board[22] && (board[22] == 'x' || board[22] == 'o'))
        return 1;
        
    else if (board[34] == board[36] && board[36] == board[38] && (board[38] == 'x' || board[38] == 'o'))
        return 1;
        
    else if (board[2] == board[18] && board[18] == board[34] && (board[34] == 'x' || board[34] == 'o'))
        return 1;
        
    else if (board[4] == board[20] && board[20] == board[36] && (board[36] == 'x' || board[36] == 'o'))
        return 1;
        
    else if (board[6] == board[22] && board[22] == board[38] && (board[38] == 'x' || board[38] == 'o'))
        return 1;
        
    else if (board[2] == board[20] && board[20] == board[38] && (board[38] == 'x' || board[38] == 'o'))
        return 1;
        
    else if (board[6] == board[20] && board[20] == board[34] && (board[34] == 'x' || board[34] == 'o'))
        return 1;
        
    else if (board[2] != ' ' && board[4] != ' ' && board[6] != ' ' && board[18] != ' ' && board[20] != ' ' && board[22] != ' ' && board[34] != ' ' && board[36] != ' ' && board[38] != ' ')
        return 0;
    else
        return  -1;
	
}
