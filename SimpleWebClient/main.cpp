#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

#include "HTMLParserBase.h"

int main(int arguement_count, char** arguement_values){
	if (arguement_count != 2){
		printf("Usage: SimpleWebClient.exe [valid http url]");
	}
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
	delete fileBuf;
	system("PAUSE");
	return 0;
}