#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <WinSock2.h>

#include "HTMLParserBase.h"
#include "URL.h"
#include "Socket.h"

#pragma comment(lib, "ws2_32.lib")

//URL structure:
//scheme://host[:port][/path][?query][#fragment]
int main(int arguement_count, char** arguement_values){
	if (arguement_count != 2){
		printf("Usage: SimpleWebClient.exe [valid http url]");
	}
	char* position = arguement_values[1];
	URL url = URL(position);
	url.printURL();
	if (url.parserURL() != 0){
		system("PAUSE");
		return 0;
	}
	// parse successful
	Socket sock = Socket();
	if (sock.ExecuteDNS(url) != 0){
		system("PAUSE");
		return 0;
	}
	// DNS execution successful
	if (sock.Connect() != 0){
		system("PAUSE");
		return 0;
	}
	// Connection successful
	if (sock.read(url) != 0){
		system("PAUSE");
		return 0;
	}
	// Read successful
	if (sock.VerifyHeader(position) != 0){
		system("PAUSE");
		return 0;
	}
	// Header Verified
	system("PAUSE");
	return 0;
}