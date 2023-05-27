#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
using namespace std;
#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h>
#include <string.h>
#include <time.h>

#define PORT 8080
#define MAX_CLIENTS 10

int main() {
	WSADATA wsaData;
	SOCKET masterSocket, newSocket, clientSockets[MAX_CLIENTS], sd;
	struct sockaddr_in serverAddress;
	int addrlen, activity, i, valread, sdMax;
	char buffer[30000] = { 0 };

	// Initialize Winsock
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("Failed to initialize Winsock\n");
		return 1;
	}

	// Create master socket
	if ((masterSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		printf("Failed to create socket\n");
		WSACleanup();
		return 1;
	}

	// Set up server address
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(PORT);

	// Bind the socket
	if (bind(masterSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
		printf("Failed to bind socket\n");
		closesocket(masterSocket);
		WSACleanup();
		return 1;
	}

	// Listen for incoming connections
	if (listen(masterSocket, SOMAXCONN) == SOCKET_ERROR) {
		printf("Failed to listen\n");
		closesocket(masterSocket);
		WSACleanup();
		return 1;
	}

	// Initialize client sockets array
	for (i = 0; i < MAX_CLIENTS; i++) {
		clientSockets[i] = 0;
	}

	addrlen = sizeof(serverAddress);
	printf("+++++++ Waiting for new connection ++++++++\n\n");

	while (1) {
		// Clear the socket set
		fd_set readfds;
		FD_ZERO(&readfds);

		// Add the master socket to the set
		FD_SET(masterSocket, &readfds);
		sdMax = masterSocket;

		// Add client sockets to the set
		for (i = 0; i < MAX_CLIENTS; i++) {
			sd = clientSockets[i];
			if (sd > 0) {
				FD_SET(sd, &readfds);
			}
			if (sd > sdMax) {
				sdMax = sd;
			}
		}

		// Wait for activity on any socket
		activity = select(sdMax + 1, &readfds, NULL, NULL, NULL);
		if (activity == SOCKET_ERROR) {
			printf("select error\n");
			break;
		}

		// Handle new connection
		if (FD_ISSET(masterSocket, &readfds)) {
			if ((newSocket = accept(masterSocket, (struct sockaddr*)&serverAddress, (int*)&addrlen)) == INVALID_SOCKET) {
				printf("Failed to accept\n");
				break;
			}

			// Add new socket to the array of client sockets
			for (i = 0; i < MAX_CLIENTS; i++) {
				if (clientSockets[i] == 0) {
					clientSockets[i] = newSocket;
					printf("New connection, socket fd is %d, IP is : %s, port : %d\n", newSocket,
						inet_ntoa(serverAddress.sin_addr), ntohs(serverAddress.sin_port));
					break;
				}
			}
		}

		// Handle client activity
		for (i = 0; i < MAX_CLIENTS; i++) {
			sd = clientSockets[i];
			if (FD_ISSET(sd, &readfds)) {
				// Check if it was for closing
				if ((valread = recv(sd, buffer, sizeof(buffer), 0)) == 0) {
					// Client disconnected
					getpeername(sd, (struct sockaddr*)&serverAddress, (int*)&addrlen);
					printf("Host disconnected, ip %s, port %d\n", inet_ntoa(serverAddress.sin_addr), ntohs(serverAddress.sin_port));

					// Close the socket and mark as 0 in array for reuse
					closesocket(sd);
					clientSockets[i] = 0;
				}
				else {
					// Handle client request
					// Parse the request and handle it accordingly
					// ...
					// Your code to handle the HTTP request goes here

					printf("Received data from client: %s\n", buffer);

					// Send response back to the client
					const char* response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\nHello, World!";
					send(sd, response, strlen(response), 0);
				}
			}
		}
	}

	// Cleanup and close sockets
	for (i = 0; i < MAX_CLIENTS; i++) {
		sd = clientSockets[i];
		if (sd > 0) {
			closesocket(sd);
		}
	}
	closesocket(masterSocket);
	WSACleanup();

	return 0;
}