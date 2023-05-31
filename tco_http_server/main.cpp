//#define _CRT_SECURE_NO_WARNINGS
//#define _WINSOCK_DEPRECATED_NO_WARNINGS
//
//#include <iostream>
//#pragma comment(lib, "Ws2_32.lib")
//#include <winsock2.h>
//#include <string.h>
//#include <time.h>
//#include <map>
//#include <string>
//#include <vector>
//
//const int MAX_SOCKETS = 60;
//constexpr int BUFF_MAX_SIZE = 2048;
//
//const int PORT = 8080;
//const int MAX_CLIENTS = 5;
//
//using namespace std;
//
//enum Status
//{
//	INIT,
//	PROCESSING,
//	FINISHED
//};
//
//enum HttpReq
//{
//	GET,
//	POST,
//	PUT,
//};
//struct SocketState
//{
//	SOCKET id;
//	Status recv;
//	Status send;
//	HttpReq sendVerb; // GET, POST, etc.
//	time_t activeStemp;
//	char buff[BUFF_MAX_SIZE];
//	int len;
//};
//
//map<string, string> parseQueryParameters(string request) {
//	map<string, string> result;
//	int start = request.find('?');
//	int end = request.find(' ', start);
//	string query = request.substr(start + 1, end - start - 1);
//	int pos = 0;
//	while (pos < query.length()) {
//		int eq = query.find('=', pos);
//		int amp = query.find('&', pos);
//		if (amp == -1) {
//			amp = query.length();
//		}
//		string key = query.substr(pos, eq - pos);
//		string value = query.substr(eq + 1, amp - eq - 1);
//		result[key] = value;
//		pos = amp + 1;
//	}
//	return result;
//}
//
//string getVerb(string request) {
//	int start = 0;
//	int end = request.find(' ', start);
//	return request.substr(start, end - start);
//}
//
//int main() {
//	WSADATA wsaData;
//	SOCKET masterSocket, newSocket, clientSockets[MAX_CLIENTS], sd;
//	struct sockaddr_in serverAddress;
//	int addrlen, activity, i, valread, sdMax;
//	char buffer[30000] = { 0 };
//
//	// Initialize Winsock
//	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
//		printf("Failed to initialize Winsock\n");
//		return 1;
//	}
//
//	// Create master socket
//	if ((masterSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
//		printf("Failed to create socket\n");
//		WSACleanup();
//		return 1;
//	}
//
//	// Set up server address
//	serverAddress.sin_family = AF_INET;
//	serverAddress.sin_addr.s_addr = INADDR_ANY;
//	serverAddress.sin_port = htons(PORT);
//
//	// Bind the socket
//	if (bind(masterSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
//		printf("Failed to bind socket\n");
//		closesocket(masterSocket);
//		WSACleanup();
//		return 1;
//	}
//
//	// Listen for incoming connections
//	if (listen(masterSocket, SOMAXCONN) == SOCKET_ERROR) {
//		printf("Failed to listen\n");
//		closesocket(masterSocket);
//		WSACleanup();
//		return 1;
//	}
//
//	// Initialize client sockets array
//	for (i = 0; i < MAX_CLIENTS; i++) {
//		clientSockets[i] = 0;
//	}
//
//	addrlen = sizeof(serverAddress);
//	std::cout << "Server waiting for new connections on IP: " << inet_ntoa(serverAddress.sin_addr) << ", PORT: " << PORT << std::endl;
//
//	while (1) {
//		// Clear the socket set
//		fd_set readfds;
//		FD_ZERO(&readfds);
//
//		// Add the master socket to the set
//		FD_SET(masterSocket, &readfds);
//		sdMax = masterSocket;
//
//		// Add client sockets to the set
//		for (i = 0; i < MAX_CLIENTS; i++) {
//			sd = clientSockets[i];
//			if (sd > 0) {
//				FD_SET(sd, &readfds);
//			}
//			if (sd > sdMax) {
//				sdMax = sd;
//			}
//		}
//
//		// Wait for activity on any socket
//		activity = select(sdMax + 1, &readfds, NULL, NULL, NULL);
//		if (activity == SOCKET_ERROR) {
//			printf("select error\n");
//			break;
//		}
//
//		// Handle new connection
//		if (FD_ISSET(masterSocket, &readfds)) {
//			if ((newSocket = accept(masterSocket, (struct sockaddr*)&serverAddress, (int*)&addrlen)) == INVALID_SOCKET) {
//				printf("Failed to accept\n");
//				break;
//			}
//
//			// Add new socket to the array of client sockets
//			for (i = 0; i < MAX_CLIENTS; i++) {
//				if (clientSockets[i] == 0) {
//					clientSockets[i] = newSocket;
//					std::cout << "New connection, socket fd is " << newSocket
//						<< ", IP is: " << inet_ntoa(serverAddress.sin_addr)
//						<< ", PORT is: " << ntohs(serverAddress.sin_port) << std::endl;
//					break;
//				}
//			}
//		}
//
//		// Handle client activity
//		for (i = 0; i < MAX_CLIENTS; i++) {
//			sd = clientSockets[i];
//			if (FD_ISSET(sd, &readfds)) {
//				// Check if it was for closing
//				if ((valread = recv(sd, buffer, sizeof(buffer), 0)) == 0) {
//					// Client disconnected
//					getpeername(sd, (struct sockaddr*)&serverAddress, (int*)&addrlen);
//					printf("Host disconnected, ip %s, port %d\n", inet_ntoa(serverAddress.sin_addr), ntohs(serverAddress.sin_port));
//
//					// Close the socket and mark as 0 in array for reuse
//					closesocket(sd);
//					clientSockets[i] = 0;
//				}
//				else {
//					// Handle client request
//					
//					string response = "";
//
//					//string header = 
//					
//					// Parse query parameters
//					map<string, string> queryParameters = parseQueryParameters(buffer);
//
//					string verb = getVerb(buffer);
//
//					if (verb == "GET")
//					{
//						// check if queryParameters contains the key "lang"
//						if (queryParameters.find("lang") != queryParameters.end())
//						{
//							// get the value associated with the key
//							string lang_val = queryParameters["lang"];
//
//							if (lang_val == "en")
//								// html in english
//								response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Hello, World!</h1></body></html>";
//							else if (lang_val == "fr")
//								// html in french
//								response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Bonjour, le monde!</h1></body></html>";
//							else if (lang_val == "he")
//								// html in hebrew
//								response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>שלום, עולם!</h1></body></html>";
//							else
//								// no such language
//								response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Language not supported!</h1></body></html>";
//						}
//					}
//
//
//					printf("Received data from client: %s\n", buffer);
//
//					// Send response back to the client
//					//const char* response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\nHello, World!";
//					send(sd, response.c_str(), response.length(), 0);
//				}
//			}
//		}
//	}
//
//	// Cleanup and close sockets
//	for (i = 0; i < MAX_CLIENTS; i++) {
//		sd = clientSockets[i];
//		if (sd > 0) {
//			closesocket(sd);
//		}
//	}
//	closesocket(masterSocket);
//	WSACleanup();
//
//	return 0;
//}

/////////////////////////////////////////////////////////////////////////////////////////////////

// working connections

//#define _CRT_SECURE_NO_WARNINGS
//#define _WINSOCK_DEPRECATED_NO_WARNINGS
//
//#include <iostream>
//#pragma comment(lib, "Ws2_32.lib")
//#include <winsock2.h>
//#include <string.h>
//#include <time.h>
//#include <map>
//#include <string>
//#include <vector>
//
//#define PORT 8080
//#define BUFFER_SIZE 1024
//#define INACTIVE_TIMEOUT 120 // 120 seconds
//#define BUFF_MAX_SIZE 1024 // the maximum size of the buffer
//
//enum Status { IDLE, READING, WRITING, FINISHED }; // used to indicate the status of a socket, whether it is idle, reading, writing or finished
//enum HttpReq { GET_, POST_, PUT_, DELETE_, HEAD_, TRACE_, OPTIONS_ , NOT_SET_YET_ , NOT_SUPPORTED_ }; // used to indicate the type of the HTTP request. We should support (at least) these 7 types of requests
//
// //we want to rewrite the code above so that each socket is stored in a SocketState struct:
//struct SocketState
//{
//	SOCKET id; // the socket's id = descriptor
//	Status status; // the status of the socket: IDLE, READING, WRITING, FINISHED
//	HttpReq reqType; // the type of the HTTP request: GET, POST, PUT, DELETE, HEAD, TRACE, OPTIONS
//	time_t lastAction; // the time of the last action performed on the socket (will be used to disconnect inactive sockets after INACTIVE_TIMEOUT seconds)
//	char buffer[BUFF_MAX_SIZE]; // the buffer used to read/write data to/from the socket
//	int bufferLen; // the length of the data stored in the buffer
//};
//
//int main()
//{
//	std::vector<SocketState> sockets; // vector of SocketState structs
//
//	// Initialize Winsock
//	WSADATA wsaData;
//	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData); // iResult = 0 if success, otherwise an error code
//	if (iResult != NO_ERROR) {
//		std::cout << "Error at WSAStartup(): " << iResult << std::endl;
//		return 1;
//	}
//
//	// Create a socket
//	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//	if (listenSocket == INVALID_SOCKET) {
//		std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
//		WSACleanup();
//		return 1;
//	}
//
//	// Bind the socket
//	sockaddr_in service;
//	service.sin_family = AF_INET;
//	service.sin_addr.s_addr = INADDR_ANY;
//	service.sin_port = htons(PORT);
//	if (bind(listenSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
//		std::cout << "bind() failed: " << WSAGetLastError() << std::endl;
//		closesocket(listenSocket);
//		return 1;
//	}
//
//	// Listen on the socket
//	if (listen(listenSocket, 1) == SOCKET_ERROR) {
//		std::cout << "Error listening on socket: " << WSAGetLastError() << std::endl;
//		closesocket(listenSocket);
//		return 1;
//	}
//
//	// Create an FD_SET for the master socket
//	fd_set master;
//	FD_ZERO(&master);
//	FD_SET(listenSocket, &master);
//
//	// Create an FD_SET for the temp socket
//	fd_set temp;
//
//	// Create a timeval struct for the timeout
//	timeval timeout;
//	timeout.tv_sec = 10;
//	timeout.tv_usec = 0;
//
//	// Main loop
//	while (true)
//	{
//		// Copy the master FD_SET to the temp FD_SET
//		temp = master;
//
//		// Call select() and wait 10 seconds for it to complete.
//		// If select() returns 0, no sockets were ready and a timeout occurred.
//		// If select() returns SOCKET_ERROR, an error occurred and we should close the server.
//		int n = select(0, &temp, NULL, NULL, &timeout);
//		if (n == SOCKET_ERROR) {
//			std::cout << "select() failed: " << WSAGetLastError() << std::endl;
//			break;
//		}
//		else if (n == 0) {
//			// Timeout occurred, no client ready
//			continue;
//		}
//
//		// Check for new connections
//		if (FD_ISSET(listenSocket, &temp)) {
//			// Accept a new connection
//			SOCKET clientSocket = accept(listenSocket, NULL, NULL);
//			if (clientSocket == INVALID_SOCKET) {
//				std::cout << "accept() failed: " << WSAGetLastError() << std::endl;
//				break;
//			}
//
//			// Add the new connection to the vector of sockets
//			SocketState newSocket;
//			newSocket.id = clientSocket;
//			newSocket.status = IDLE;
//			newSocket.reqType = NOT_SET_YET_;
//			newSocket.lastAction = time(NULL);
//			newSocket.bufferLen = 0;
//			sockets.push_back(newSocket);
//
//			// Add the new connection to the master FD_SET
//			FD_SET(clientSocket, &master);
//
//			std::cout << "New connection accepted with socket ID: " << clientSocket
//				<< " at " << ctime(&newSocket.lastAction)
//				<< " from " << inet_ntoa(service.sin_addr)
//				<< ":" << ntohs(service.sin_port) << std::endl;
//			}
//		else {
//			// Check for activity on any of the client sockets
//			for (int i = 0; i < sockets.size(); i++) {
//				SocketState& socket = sockets[i];
//				if (FD_ISSET(socket.id, &temp)) {
//					// Check if the socket was closed
//					char buffer[BUFFER_SIZE];
//					int bytesReceived = recv(socket.id, buffer, BUFFER_SIZE, 0);
//					if (bytesReceived <= 0) {
//						// The socket was closed
//						std::cout << "Socket " << socket.id << " closed." << std::endl;
//						closesocket(socket.id);
//						FD_CLR(socket.id, &master);
//						sockets.erase(sockets.begin() + i);
//						continue;
//					}
//
//					// The socket is active
//					std::cout << "Socket " << socket.id << " is active." << std::endl;
//
//					// make sure the buffer is 0-terminated
//					buffer[bytesReceived] = 0;
//
//					// Update the last action time
//					socket.lastAction = time(NULL);
//
//					std::cout << "Received " << bytesReceived << " bytes from socket " << socket.id << " (added '\\0' here)"<< std::endl;
//
//					std::cout << std::endl << "THE DATA RECEIVED IS: " << buffer << std::endl << std::endl;
//
//					// Process the data received from the client
//
//					// Send a response if necessary
//					// ...
//					// ...
//
//					// Clear the temp FD_SET for the next call to select()
//					FD_CLR(socket.id, &temp);
//					}
//				}
//			}
//
//		// Check for inactive sockets
//		time_t currentTime = time(NULL);
//
//		// Loop through all the sockets and check if any of them have timed out
//
//		for (int i = 0; i < sockets.size(); i++) {
//			SocketState& socket = sockets[i];
//			if (currentTime - socket.lastAction > INACTIVE_TIMEOUT) {
//				// The socket timed out
//				std::cout << "Socket " << socket.id << " timed out." << std::endl;
//				closesocket(socket.id);
//				FD_CLR(socket.id, &master);
//				sockets.erase(sockets.begin() + i);
//				continue;
//			}
//		}
//
//	}
//
//	// Close the socket
//	closesocket(listenSocket);
//
//	// Cleanup Winsock
//	WSACleanup();
//
//	return 0;
//}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// WITH file GET

//#define _CRT_SECURE_NO_WARNINGS
//#define _WINSOCK_DEPRECATED_NO_WARNINGS
//
//#include <iostream>
//#pragma comment(lib, "Ws2_32.lib")
//#include <winsock2.h>
//#include <string.h>
//#include <string>
//#include <time.h>
//#include <map>
//#include <string>
//#include <vector>
//#include <fstream>
//#include <sstream>
//
//#define PORT 8080
//#define BUFFER_SIZE 1024
//#define INACTIVE_TIMEOUT 120 // 120 seconds
//#define BUFF_MAX_SIZE 1024 // the maximum size of the buffer
//
//enum Status { IDLE, READING, WRITING, FINISHED };
//enum HttpReq { GET_, POST_, PUT_, DELETE_, HEAD_, TRACE_, OPTIONS_, NOT_SET_YET_, NOT_SUPPORTED_ };
//
//struct SocketState
//{
//	SOCKET id;
//	Status status;
//	HttpReq reqType;
//	time_t lastAction;
//	char buffer[BUFF_MAX_SIZE];
//	int bufferLen;
//};
//
//void SendFile(SOCKET socket, const std::string& path);
//int FindEndOfLine(const char* buffer, int bufferSize);
//HttpReq ParseRequestType(const std::string& requestLine);
//std::string ParsePath(const std::string& requestLine);
//
//std::string getContentType(const std::string& filePath) {
//	std::string extension = filePath.substr(filePath.find_last_of(".") + 1);
//	if (extension == "html") {
//		return "text/html";
//	}
//	else if (extension == "css") {
//		return "text/css";
//	}
//	else if (extension == "js") {
//		return "application/javascript";
//	}
//	else if (extension == "png") {
//		return "image/png";
//	}
//	else if (extension == "jpg" || extension == "jpeg") {
//		return "image/jpeg";
//	}
//	else if (extension == "gif") {
//		return "image/gif";
//	}
//	else if (extension == "txt") {
//		return "text/plain";
//	}
//	else {
//		return "application/octet-stream";
//	}
//}
//
//void sendResponse(SocketState& socket, const std::string& response) {
//	send(socket.id, response.c_str(), response.size(), 0);
//	socket.status = FINISHED;
//}
//
//std::string readFile(const std::string& filePath) {
//	std::ifstream file(filePath, std::ios::binary);
//	if (!file.is_open()) {
//		return "";
//	}
//
//	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
//	file.close();
//
//	return content;
//}
//
//void handleGETRequest(SocketState& socket) {
//	std::string request(socket.buffer, socket.bufferLen);
//	size_t start = request.find("GET ");
//	size_t end = request.find(" HTTP/");
//	if (start == std::string::npos || end == std::string::npos) {
//		sendResponse(socket, "HTTP/1.1 400 Bad Request\r\n\r\n");
//		return;
//	}
//
//	std::string filePath = request.substr(start + 4, end - start - 4);
//	if (filePath.empty() || filePath == "/") {
//		filePath = "/index.html";
//	}
//
//	std::string fullPath = "C://temp" + filePath;
//	std::string contentType = getContentType(fullPath);
//
//	std::string content = readFile(fullPath);
//	if (content.empty()) {
//		sendResponse(socket, "HTTP/1.1 404 Not Found\r\n\r\n");
//		return;
//	}
//
//	std::string response = "HTTP/1.1 200 OK\r\n";
//	response += "Content-Type: " + contentType + "\r\n";
//	response += "Content-Length: " + std::to_string(content.size()) + "\r\n";
//	response += "\r\n" + content;
//
//	sendResponse(socket, response);
//
//	/*std::string request(socket.buffer, socket.bufferLen);
//	size_t start = request.find("GET ");
//	size_t end = request.find(" HTTP/");
//	if (start == std::string::npos || end == std::string::npos) {
//		sendResponse(socket, "HTTP/1.1 400 Bad Request\r\n\r\n");
//		return;
//	}
//
//	std::string filePath = request.substr(start + 4, end - start - 4);
//	if (filePath.empty() || filePath == "/") {
//		filePath = "/index.html";
//	}
//
//	std::string fullPath = "C://temp" + filePath;
//	std::string contentType = getContentType(fullPath);
//
//	std::string content = readFile(fullPath);
//	if (content.empty()) {
//		sendResponse(socket, "HTTP/1.1 404 Not Found\r\n\r\n");
//		return;
//	}
//
//	std::string response = "HTTP/1.1 200 OK\r\n";
//	response += "Content-Type: " + contentType + "\r\n";
//	response += "Content-Length: " + std::to_string(content.size()) + "\r\n";
//	response += "\r\n" + content;
//
//	sendResponse(socket, response);*/
//}
//
//int main()
//{
//	std::vector<SocketState> sockets;
//	WSADATA wsaData;
//	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
//	if (iResult != NO_ERROR) {
//		std::cout << "Error at WSAStartup(): " << iResult << std::endl;
//		return 1;
//	}
//
//	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//	if (listenSocket == INVALID_SOCKET) {
//		std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
//		WSACleanup();
//		return 1;
//	}
//
//	sockaddr_in service;
//	service.sin_family = AF_INET;
//	service.sin_addr.s_addr = INADDR_ANY;
//	service.sin_port = htons(PORT);
//	if (bind(listenSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
//		std::cout << "bind() failed: " << WSAGetLastError() << std::endl;
//		closesocket(listenSocket);
//		return 1;
//	}
//
//	if (listen(listenSocket, 1) == SOCKET_ERROR) {
//		std::cout << "Error listening on socket: " << WSAGetLastError() << std::endl;
//		closesocket(listenSocket);
//		return 1;
//	}
//
//	fd_set master;
//	FD_ZERO(&master);
//	FD_SET(listenSocket, &master);
//	fd_set temp;
//	timeval timeout;
//	timeout.tv_sec = 10;
//	timeout.tv_usec = 0;
//
//	while (true)
//	{
//		temp = master;
//		int n = select(0, &temp, NULL, NULL, &timeout);
//		if (n == SOCKET_ERROR) {
//			std::cout << "select() failed: " << WSAGetLastError() << std::endl;
//			break;
//		}
//		else if (n == 0) {
//			continue;
//		}
//
//		if (FD_ISSET(listenSocket, &temp)) {
//			SOCKET clientSocket = accept(listenSocket, NULL, NULL);
//			if (clientSocket == INVALID_SOCKET) {
//				std::cout << "accept() failed: " << WSAGetLastError() << std::endl;
//				break;
//			}
//
//			SocketState newSocket;
//			newSocket.id = clientSocket;
//			newSocket.status = IDLE;
//			newSocket.reqType = NOT_SET_YET_;
//			newSocket.lastAction = time(NULL);
//			newSocket.bufferLen = 0;
//			sockets.push_back(newSocket);
//
//			FD_SET(clientSocket, &master);
//
//			std::cout << "New connection accepted with socket ID: " << clientSocket
//				<< " at " << ctime(&newSocket.lastAction)
//				<< " from " << inet_ntoa(service.sin_addr)
//				<< ":" << ntohs(service.sin_port) << std::endl;
//		}
//		else {
//			for (int i = 0; i < sockets.size(); i++) {
//				SocketState& socket = sockets[i];
//				if (FD_ISSET(socket.id, &temp)) {
//					char buffer[BUFFER_SIZE];
//					int bytesReceived = recv(socket.id, buffer, BUFFER_SIZE, 0);
//					if (bytesReceived <= 0) {
//						std::cout << "Socket " << socket.id << " closed." << std::endl;
//						closesocket(socket.id);
//						FD_CLR(socket.id, &master);
//						sockets.erase(sockets.begin() + i);
//						continue;
//					}
//
//					socket.lastAction = time(NULL);
//					std::cout << "Socket " << socket.id << " is active." << std::endl;
//
//					buffer[bytesReceived] = 0;
//					memcpy(socket.buffer + socket.bufferLen, buffer, bytesReceived);
//					socket.bufferLen += bytesReceived;
//
//					std::cout << "Received " << bytesReceived << " bytes from socket " << socket.id << " (added '\\0' here)" << std::endl;
//
//					std::cout << std::endl << "THE DATA RECEIVED IS: " << buffer << std::endl << std::endl;
//
//					if (socket.status == IDLE) {
//						std::string requestLine;
//						int requestLineEnd = FindEndOfLine(socket.buffer, socket.bufferLen);
//						if (requestLineEnd != -1) {
//							requestLine = std::string(socket.buffer, socket.buffer + requestLineEnd);
//							socket.bufferLen -= requestLineEnd;
//							memmove(socket.buffer, socket.buffer + requestLineEnd, socket.bufferLen);
//						}
//
//						if (!requestLine.empty()) {
//							socket.reqType = ParseRequestType(requestLine);
//							if (socket.reqType == GET_) {
//								std::string path = ParsePath(requestLine);
//								SendFile(socket.id, path);
//							}
//						}
//					}
//
//					FD_CLR(socket.id, &temp);
//				}
//			}
//		}
//
//		time_t currentTime = time(NULL);
//
//		for (int i = 0; i < sockets.size(); i++) {
//			SocketState& socket = sockets[i];
//			if (currentTime - socket.lastAction > INACTIVE_TIMEOUT) {
//				std::cout << "Socket " << socket.id << " timed out." << std::endl;
//				closesocket(socket.id);
//				FD_CLR(socket.id, &master);
//				sockets.erase(sockets.begin() + i);
//				continue;
//			}
//		}
//	}
//
//	closesocket(listenSocket);
//	WSACleanup();
//
//	return 0;
//}
//
//#include <filesystem>
//
//void SendFile(SOCKET socket, const std::string& path)
//{
//	std::string fullPath = "C:\\temp\\" + path;
//    std::cout << "Sending file: " << fullPath << std::endl;
//
//    std::ifstream file(fullPath, std::ios::binary);
//    if (!file.is_open()) {
//        std::cout << "Failed to open file: " << fullPath << std::endl;
//        return;
//    }
//
//    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
//    file.close();
//
//    std::string response = "HTTP/1.1 200 OK\r\n";
//    response += "Content-Length: " + std::to_string(content.size()) + "\r\n";
//    response += "Content-Type: text/html\r\n";
//    response += "\r\n" + content;
//
//    send(socket, response.c_str(), response.size(), 0);
//}
//
//int FindEndOfLine(const char* buffer, int bufferSize)
//{
//	for (int i = 0; i < bufferSize - 1; i++) {
//		if (buffer[i] == '\r' && buffer[i + 1] == '\n') {
//			return i + 2;
//		}
//	}
//	return -1;
//}
//
//HttpReq ParseRequestType(const std::string& requestLine)
//{
//	if (requestLine.substr(0, 3) == "GET") {
//		return GET_;
//	}
//	return NOT_SUPPORTED_;
//}
//
//std::string ParsePath(const std::string& requestLine)
//{
//	size_t start = requestLine.find(' ') + 1;
//	size_t end = requestLine.find(' ', start);
//	if (end != std::string::npos) {
//		// remove the first '/' character
//		start++;
//
//		std::cout << requestLine.substr(start, end - start);
//		return requestLine.substr(start, end - start);
//	}
//	return "";
//}