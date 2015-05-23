#pragma once

#ifdef _WIN64

#ifdef _DEBUG

#pragma comment(lib, "HTMLParser_debug_x64.lib")

#else

#pragma comment(lib, "HTMLParser_release_x64.lib")

#endif

#else

#ifdef _DEBUG

#pragma comment(lib, "HTMLParser_debug_win32.lib")

#else

#pragma comment(lib, "HTMLParser_release_win32.lib")

#endif

#endif

#define MAX_HOST_LENGTH 256
#define MAX_URL_LENGTH 2048

class HTMLParserBase{
private:
	void *parser;
	void *buffer;
public:
	HTMLParserBase();
	~HTMLParserBase();
	char* Parse(char* html_code, int code_size, char* base_url, int url_length, int* link_count);
};