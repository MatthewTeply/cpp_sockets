#include <iostream>
#include <windows.h>
#include <stdio.h>

#include "packet.h";

#pragma comment(lib, "ws2_32.lib")

using namespace std;

SOCKET sock; //My main socket
SOCKET sock2[200]; //Socket from clients
SOCKADDR_IN i_sock2; //Info about sock2
SOCKADDR_IN i_sock; //Info about main sock
WSADATA Data; //Socket version control
int clients = 0;

int startServer(int Port) {
	
	int err;

	WSAStartup(MAKEWORD(2,2), &Data); //Initializing socket, Choosing version, &Data = Save socket version
	sock = socket(AF_INET, SOCK_STREAM, 0); //AF_INET = Addres family (IPv4), 0 = Choosing protocol (0 = NULL)

	if(sock == INVALID_SOCKET) {
	
		Sleep(4000);
		exit(0);
		return 0; 
	}

	i_sock.sin_family = AF_INET; //Address family specification, AF_INET = IPv4 
	i_sock.sin_addr.s_addr = htonl(INADDR_ANY); //Start the server at my IP
	i_sock.sin_port = htons(Port); //Specify our server's port

	err = bind(sock, (LPSOCKADDR)&i_sock, sizeof(i_sock)); 

	if(err != 0)
		return 0;

	err = listen(sock, 2); //2 = maximum clients supported

	if(err == SOCKET_ERROR)
		return 0;

	while(true) {
	
		for(int i = 0; i < 4; i++) {
		
			if(clients < 4) {
			
				int so2len = sizeof(i_sock2); //Size of informations about the client (IP address, etc.)
				sock2[clients] = accept(sock, (sockaddr *)&i_sock2, &so2len); //Accept incoming connection, with client information stored in '(sockaddr *)&i_sock2' and '&so2len'

				if(sock2[clients] == INVALID_SOCKET)
					return 0;

				cout << "Client has joined the server (IP : " << i_sock2.sin_addr.s_addr << " )" << endl;
				clients++;
				continue;
			}

			else
				break;
		}
	}

	return 1;
}

int send(char *Buf, int len, int client) { //int len = size of data that will be sent to the Buf, int client = client ID

	int slen = send(sock2[client], Buf, len, 0); //'sock2[client]' = accepted client ID, Buf = data sent, len = size of sent data, 0 = specifying call (ignore for now)

	if(slen < 0) { //If the size of the data is zero, meaning there is no data, data won't be sent
	
		cout << "Cannot send data!" << endl;
		return 1;
	}

	return slen; //Return the length of the sent data, in bytes
}

int recieve(char *Buf, int len, int client) { //Buf = data, len = data length, client = client ID

	int slen = recv(sock2[client], Buf, len, 0); //Length of recieved data

	if(len < 0) {
	
		cout << "Cannot send data!" << endl;
		return 1;
	}

	return slen; //Return length of the recieved data
}

int endSocket() {

	closesocket(sock);
	WSACleanup();
	return 1;
}

int main() {

	startServer(3000);

	//Send to client with ID 1
	//MyPacket packet;
	//send((char *)&packet, sizeof(packet), 1);
	
	//Recieve from client with ID 1
	//recieve((char *)&packet, sizeof(packet), 1);

	//Send to all clients
	/*for(int i = 0; i < 4; i++) {
	
		MyPacket packet;
		send((char *)&packet, sizeof(packet), i);
	}*/

	//Receive from all clients
	/*for(int i = 0; i < 4; i++) {
	
		MyPacket packet;
		recieve((char *)&packet, sizeof(packet), i);
	}*/

	cout << "Still working!" << endl;
	
	getchar();
	getchar();

	return 0;
}