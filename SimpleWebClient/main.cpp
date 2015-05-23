#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <Windows.h>

#include "HTMLParserBase.h"

// URL structure:
// http://host[:port][/path][?query][#fragment]
int checkScheme(char* url){
	if (strncmp(url, "http://", 7) != 0){
		printf(" failed with invalid scheme\n");
		return 1;
	}
	else{
		return 0;
	}
}

// URL structure:
// host[:port][/path][?query][#fragment]
int getHost(char* url, char* host){
	char* minimum = NULL;
	if (strchr(url, ':') != NULL){
		minimum = strchr(url, ':');
	}
	if (strchr(url, '/') != NULL){
		if (minimum != NULL){
			minimum = min(minimum, strchr(url, '/'));
		}
		else{
			minimum = strchr(url, '/');
		}
	}
	if (strchr(url, '?') != NULL){
		if (minimum != NULL){
			minimum = min(minimum, strchr(url, '?'));
		}
		else{
			minimum = strchr(url, '?');
		}
	}
	if (strchr(url, '#') != NULL){
		if (minimum != NULL){
			minimum = min(minimum, strchr(url, '#'));
		}
		else{
			minimum = strchr(url, '#');
		}
	}
	if (minimum == NULL){
		if (strlen(url) > MAX_HOST_LENGTH){
			printf(" failed with invalid host\n");
			return 1;
		}
		strcpy(host, url);
	}
	else{
		if ((minimum - url) > MAX_HOST_LENGTH){
			printf(" failed with invalid host\n");
			return 1;
		}
		strncpy(host, url, minimum - url);
		host[minimum - url] = '\0';
	}
	return 0;
}

// URL structure:
// [:port][/path][?query][#fragment]
int parseOptionalParts(char* url, unsigned int* port, char* request){
	if (*url == ':'){
		char* minimum=strchr(url, '/');
		if (minimum != NULL){
			if (strchr(url, '?') != NULL){
				minimum = min(minimum, strchr(url, '?'));
			}
		}
		else{
			minimum = strchr(url, '?');
		}
		if (minimum != NULL){
			if (strchr(url, '#') != NULL){
				minimum = min(minimum, strchr(url, '#'));
			}
		}
		else{
			minimum = strchr(url, '#');
		}
		char temp;
		if (minimum != NULL){
			temp = *minimum;
			*minimum = '\0';
		}
		url++;
		if (*url == '0' || !isdigit(*url)){
			printf(" failed with invalid port\n");
			return 1;
		}
		*port = atoi(url);
		*minimum = temp;
		url = minimum;
	}
	// port is loaded
	if (*url == '#'){
		*request = '/';
		*(request + 1) = '\0';
	}
	else{
		char* target = strchr(url, '#');
		if (target == NULL){
			if (*url == '/'){
				strcpy(request, url);
			}
			else{
				*request = '/';
				strcpy(request + 1, url);
			}
		}
		else{
			if (*url == '/'){
				strncpy(request, url, target - url);
				request[target - url] = '\0';
			}
			else{
				*request = '/';
				strncpy(request + 1, url, target - url);
				request[target - url + 1] = '\0';
			}
		}
	}
	return 0;
}

int ParseURL(char* url, char* host, unsigned int* port, char* request){
	char* index = new char[strlen(url) + 1];
	strcpy(index, url);
	index[strlen(url)] = '\0';
	if (checkScheme(index) != 0){
		return 1;
	}
	index += 7;
	if (getHost(index, host) != 0){
		return 1;
	}
	*port = 80;
	index += strlen(host);
	if (parseOptionalParts(index, port, request) != 0){
		return 1;
	}
	printf(" host %s, port %d, request %s\n", host, port, request);
	return 0;
}

//URL structure:
//scheme://host[:port][/path][?query][#fragment]
int main(int arguement_count, char** arguement_values){
	if (arguement_count != 2){
		printf("Usage: SimpleWebClient.exe [valid http url]");
	}
	char* position = arguement_values[1];
	printf("URL: %s\n", position);
	printf("        Parsing URL...");
	char* host = new char[MAX_HOST_LENGTH];
	char* request = new char[MAX_URL_LENGTH];
	unsigned int port;
	if (ParseURL(position, host, &port, request) != 0){
		system("PAUSE");
		return 0;
	}
	// parse successful
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
	system("PAUSE");
	return 0;
}