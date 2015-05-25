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
	/*
	char filename[] = "tamu.html";
	HANDLE hFile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE){
		printf("HANDLE CreateFile(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwSharedMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile) failed with %d\n", GetLastError());
		return 0;
	}
	LARGE_INTEGER li;
	BOOL bRet = GetFileSizeEx(hFile, &li);
	if (bRet == 0){
		printf("BOOL GetFileSizeEx(HANDLE hFile, PLARGE_INTEGER lpFileSize) failed with %d\n", GetLastError());
		return 0;
	}
	int fileSize = (DWORD)li.QuadPart;
	DWORD bytesRead;
	char *fileBuf = new char[fileSize];
	bRet = ReadFile(hFile, fileBuf, fileSize, &bytesRead, NULL);
	if (bRet == 0 || bytesRead != fileSize){
		printf("BOOL ReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped) failed with %d\n", GetLastError());
		return 0;
	}
	CloseHandle(hFile);
	HTMLParserBase *parser = new HTMLParserBase;
	char baseURL[] = "http://www/tamu.edu";
	int nLinks;
	char *linkBuffer = parser->Parse(fileBuf, fileSize, baseURL, (int)strlen(baseURL), &nLinks);
	if (nLinks < 0){
		nLinks = 0;
	}
	printf("Found %d links:\n", nLinks);
	for (int i = 0; i < nLinks; i++){
		printf("%s\n", linkBuffer);
		linkBuffer += strlen(linkBuffer) + 1;
	}
	delete parser;
	delete[] fileBuf;
	*/
	system("PAUSE");
	return 0;
}