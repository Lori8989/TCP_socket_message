#include <winsock2.h>
#include <stdio.h>
#include <string.h>

#define MAX_SIZE 2048
#define ADDRESS_SIZE 20
#define MY_ERROR(s) printf(s); system("PAUSE"); exit(1);

int main(int argc,char *argv[]) {
	SOCKET serverSocket;
	struct sockaddr_in serverAddress;
	int bytesRead;
	char buf[MAX_SIZE];
	char serverAddressStr[ADDRESS_SIZE];
	int serverPort;

	if (argc == 3) {
		printf("connected.\n");
		strcpy(serverAddressStr, argv[1]);
		serverPort = atoi(argv[2]);
	}
	else
	{
		printf("argc:%d,",&argc);
		for( int i = 0; i < 300; i++)
			printf("%c",argv[i]);
	}
	

	// call WSAStartup first for Winsock
	WSADATA wsadata;
	if (WSAStartup(MAKEWORD(2,2),(LPWSADATA)&wsadata) != 0) {
		MY_ERROR("Winsock Error\n");
	}

	// Set the server information
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr(serverAddressStr); // transform to 32-bit unsigned integer
	serverAddress.sin_port = htons(serverPort); //converts a u_short from host to TCP/IP network byte order

	while (1) {
		//printf("\nEnter something:\n");
		// Create socket
		serverSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (connect(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
      		MY_ERROR("Connect Error\n");
		}
		printf("Server IP is : %s \n", inet_ntoa(serverAddress.sin_addr));

		recv(serverSocket,buf, MAX_SIZE, 0);//"press 1 or 2"
		for(int i = 0; i < MAX_SIZE;i++){
			if(buf[i] == '\n')
				break;
			printf("%c",buf[i]);
		}
		printf("\n");

		//client enter '1' or '2'
		char cmd[MAX_SIZE];
		scanf("%s", cmd);
		send(serverSocket, cmd, strlen(cmd), 0);

		if( cmd[0] == '2' && strlen(cmd) == 1){
			recv(serverSocket,buf, MAX_SIZE, 0);//"write"
			for(int i = 0; i < MAX_SIZE;i++){
				if(buf[i] == '\n')
					break;
				printf("%c",buf[i]);
			}
			printf("\n");
			char nw_messsage[MAX_SIZE];
			scanf("%s",nw_messsage);
			nw_messsage[strlen(nw_messsage)] = '\0';
			send(serverSocket, nw_messsage, strlen(nw_messsage)+1, 0);
			printf("New message is sent!\n");
		}
		else{
			recv(serverSocket,buf, MAX_SIZE, 0);//"wrong input","empty" or existing message
			
			for(int i = 0; i < MAX_SIZE;i++){
				if(buf[i] == '\0' || buf[i] == '\n')//end message,input or end empty_str
					break;
				printf("%c",buf[i]);
			}
			printf("\n");
		}
		closesocket(serverSocket);
	}

	return 0;
}


		// Send the data to the server, and receive it back
		//send(serverSocket, buf, strlen(buf), 0);
		//bytesRead = recv(serverSocket, buf, MAX_SIZE, 0);
		//buf[bytesRead] = '\0';
		//if (bytesRead > 0) {
		//	printf("Receive %d byte(s): %s\n", bytesRead, buf);
		//}