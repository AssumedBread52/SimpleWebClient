#ifndef SOCKET_H

#define SOCKET_H

class Socket{
private:
	SOCKET current_socket;
	sockaddr_in server;
	hostent* remote;
	DWORD time;
	int allocated_size;
	int current_position;
	char* buffer;
	int sendRequest(URL url){
		char* send_buffer = new char[4 + strlen(url.returnRequest()) + 17 + strlen(url.returnHost()) + 24];
		strcpy(send_buffer, "GET ");
		strncpy(send_buffer + 4, url.returnRequest(), strlen(url.returnRequest()));
		strcpy(send_buffer + 4 + strlen(url.returnRequest()), " HTTP/1.0\r\nHost: ");
		strncpy(send_buffer + 4 + strlen(url.returnRequest()) + 17, url.returnHost(), strlen(url.returnHost()));
		strcpy(send_buffer + 4 + strlen(url.returnRequest()) + 17 + strlen(url.returnHost()), "\r\nConnection: close\r\n\r\n\0");
		if (send(current_socket, send_buffer, strlen(send_buffer), 0) == SOCKET_ERROR){
			printf(" failed with %d\n", WSAGetLastError());
			closesocket(current_socket);
			WSACleanup();
			return 1;
		}
		return 0;
	}
public:
	Socket(){
		current_position = 0;
		allocated_size = 8192;
		buffer = new char[allocated_size];
	}
	sockaddr_in getServer(){
		return server;
	}
	int ExecuteDNS(URL url){
		time = GetTickCount();
		printf("        Doing DNS...");
		WSADATA wsa_data;
		WORD w_version_requested = MAKEWORD(2, 2);
		if (WSAStartup(w_version_requested, &wsa_data) != 0){
			printf(" failed with %d\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}
		current_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (current_socket == INVALID_SOCKET){
			printf(" failed with %d\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}
		DWORD IP = inet_addr(url.returnHost());
		if (IP == INADDR_NONE){
			if ((remote = gethostbyname(url.returnHost())) == NULL){
				printf(" failed with %d\n", WSAGetLastError());
				closesocket(current_socket);
				WSACleanup();
				return 1;
			}
			else{
				memcpy((char*)(&(server.sin_addr)), remote->h_addr, remote->h_length);
			}
		}
		else{
			server.sin_addr.S_un.S_addr = IP;
		}
		server.sin_family = AF_INET;
		server.sin_port = htons(url.returnPort());
		time = GetTickCount() - time;
		printf(" done in %d ms, found %s\n", time, inet_ntoa(server.sin_addr));
		return 0;
	}
	int Connect(){
		time = GetTickCount();
		printf("      * Connecting on page...");
		if (connect(current_socket, (sockaddr*)(&server), sizeof(sockaddr_in)) == SOCKET_ERROR){
			printf(" failed with %d\n", WSAGetLastError());
			closesocket(current_socket);
			WSACleanup();
			return 1;
		}
		time = GetTickCount() - time;
		printf(" done in %d ms\n", time);
		return 0;
	}
	int read(URL url){
		if (sendRequest(url)){
			return 1;
		}
		time = GetTickCount();
		printf("        Loading...");
		timeval timeout;
		timeout.tv_sec = 10;
		timeout.tv_usec = 0;
		fd_set fd;
		int return_value;
		while (true){
			FD_ZERO(&fd);
			FD_SET(current_socket, &fd);
			if ((return_value = select(0, &fd, NULL, NULL, &timeout)) > 0){
				// new data available; now read the next segment
				int bytes = recv(current_socket, buffer + current_position, allocated_size - current_position, 0);
				if (bytes == SOCKET_ERROR){
					printf(" failed with %d\n", WSAGetLastError());
					closesocket(current_socket);
					WSACleanup();
					break;
				}
				if (bytes == 0){
					buffer[current_position] = '\0';
					time = GetTickCount() - time;
					printf(" done in %d ms with %d\n", time, current_position);
					return 0;
				}
				current_position += bytes;
				if (allocated_size - current_position < 1){
					char* new_buffer = new char[allocated_size * 2];
					allocated_size *= 2;
					memcpy(new_buffer, buffer, current_position);
					buffer = new_buffer;
				}
			}
			else if (return_value == 0){
				printf(" failed with timeout\n");
				closesocket(current_socket);
				WSACleanup();
				break;
			}
			else{
				printf(" failed with %d\n", WSAGetLastError());
				closesocket(current_socket);
				WSACleanup();
				break;
			}
		}
		return 1;
	}
};

#endif