#ifndef SOCKET_H

#define SOCKET_H

class Socket{
private:
	SOCKET current_socket;
	sockaddr_in server;
	hostent* remote;
	DWORD time;
public:
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
			return 1;
		}
		time = GetTickCount() - time;
		printf(" done in %d ms\n", time);
		return 0;
	}
};

#endif