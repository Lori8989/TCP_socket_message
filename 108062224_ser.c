#include <winsock2.h>
#include <stdio.h>
#include <string.h>

#define MAX_SIZE 2048
#define MY_ERROR(s) printf(s); system("PAUSE"); exit(1);

int main(int argc, char* argv[]) {
	SOCKET serverSocket, clientSocket; // create a socket
	struct sockaddr_in serverAddress, clientAddress; // sockaddr_in：IP4 格式使用 ,  sockaddr_in6：IP6 格式使用 , sockaddr：通用格式
	int clientAddressLen;
	int bytesRead;
	char buf[MAX_SIZE];
	char message[MAX_SIZE];
	int serverPort;
	message[0] = '\0';

	if (argc == 2) {
		serverPort = atoi(argv[1]);
	}
	// call WSAStartup first for Winsock
    WSADATA wsadata;
    if( WSAStartup(MAKEWORD(2,2),(LPWSADATA)&wsadata) != 0) { // ( version of winsock )
        MY_ERROR("Winsock Error\n");
	}

	// Create socket
	serverSocket = socket(PF_INET, SOCK_STREAM, 0); // (address , type , protocal(0表示不強制) )

	// Set the server information
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(serverPort); //converts a u_short from host to TCP/IP network byte order

	// Bind the socket
	if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
		MY_ERROR("Bind Error\n");
	}

	// Prepare to listen to the incoming clients
	if (listen(serverSocket, 3) < 0) {
		MY_ERROR("Listen Error\n");
	}

	while (1) {
		printf("Waiting...\n");

		// Accept a client
		clientAddressLen = sizeof(clientAddress);
		clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLen);
		printf("Client IP is : %s \n", inet_ntoa(clientAddress.sin_addr));

		char my_str[] = "Please press '1' for read or '2' for write.\n";
		send(clientSocket, my_str, strlen(my_str), 0);

		bytesRead = recv(clientSocket, buf, MAX_SIZE, 0);
		if(bytesRead != 1){
			char wrong_[] = "Wrong input!\n";
			send(clientSocket, wrong_, strlen(wrong_), 0);
		}
		else if(buf[0] == '1'){
			//read previous
			printf("Ask to read.\n");
			if(message[0] == '\0'){
				char empty_[] = "Now is empty!\n";
				send(clientSocket, empty_, strlen(empty_), 0);
			}
			else
			{
				char slice_message[MAX_SIZE];
				int i;
				for(i = 0; i < strlen(message);i++){
					if(message[i] == '\0')
						break;
					slice_message[i] = message[i];
				}
				slice_message[i] = '\0';
				send(clientSocket, slice_message, strlen(slice_message) + 1, 0);
			}
			
		}
		else if(buf[0] == '2'){
			//write new
			printf("Ask to write.\n");
			char write_[] = "Enter new message please.\n";
			send(clientSocket, write_, strlen(write_), 0);
			recv(clientSocket, message, MAX_SIZE, 0);//nw_message
			message[strlen(message)] = '\0';
		}
		else{
			char wrong_[] = "Wrong input!\n";
			send(clientSocket, wrong_, strlen(wrong_), 0);
		}
		//printf("Receive %d byte(s): %s\n", bytesRead, buf);
		closesocket(clientSocket);
	}

	return 0;
}
//gcc -o server .\TCP_echo_server_win.c -lwsock32
// Receive the data from the client, and send it back
		//bytesRead = recv(clientSocket, buf, MAX_SIZE, 0);
		//buf[bytesRead] = '\0';
		//send(clientSocket, buf, bytesRead, 0);