#ifndef URL_H

#define URL_H

class URL{
private:
	char* url;
	char* host;
	char* request;
	unsigned int port;
	int checkScheme(char* current_url){
		if (strncmp(current_url, "http://", 7) != 0){
			printf(" failed with invalid scheme\n");
			return 1;
		}
		else{
			return 0;
		}
	}
	int getHost(char* current_url){
		host = new char[MAX_HOST_LENGTH];
		char* minimum = NULL;
		if (strchr(current_url, ':') != NULL){
			minimum = strchr(current_url, ':');
		}
		if (strchr(current_url, '/') != NULL){
			if (minimum != NULL){
				minimum = min(minimum, strchr(current_url, '/'));
			}
			else{
				minimum = strchr(current_url, '/');
			}
		}
		if (strchr(current_url, '?') != NULL){
			if (minimum != NULL){
				minimum = min(minimum, strchr(current_url, '?'));
			}
			else{
				minimum = strchr(current_url, '?');
			}
		}
		if (strchr(current_url, '#') != NULL){
			if (minimum != NULL){
				minimum = min(minimum, strchr(current_url, '#'));
			}
			else{
				minimum = strchr(current_url, '#');
			}
		}
		if (minimum == NULL){
			if (strlen(current_url) > MAX_HOST_LENGTH){
				printf(" failed with invalid host\n");
				return 1;
			}
			strcpy(host, current_url);
		}
		else{
			if ((minimum - current_url) > MAX_HOST_LENGTH){
				printf(" failed with invalid host\n");
				return 1;
			}
			strncpy(host, current_url, minimum - current_url);
			host[minimum - current_url] = '\0';
		}
		return 0;
	}
	int parseOptionalParts(char* current_url){
		request = new char[MAX_URL_LENGTH];
		if (*current_url == ':'){
			char* minimum = strchr(current_url, '/');
			if (minimum != NULL){
				if (strchr(current_url, '?') != NULL){
					minimum = min(minimum, strchr(current_url, '?'));
				}
			}
			else{
				minimum = strchr(current_url, '?');
			}
			if (minimum != NULL){
				if (strchr(current_url, '#') != NULL){
					minimum = min(minimum, strchr(current_url, '#'));
				}
			}
			else{
				minimum = strchr(current_url, '#');
			}
			char temp;
			if (minimum != NULL){
				temp = *minimum;
				*minimum = '\0';
			}
			current_url++;
			if (*current_url == '0' || !isdigit(*current_url)){
				printf(" failed with invalid port\n");
				return 1;
			}
			port = atoi(current_url);
			*minimum = temp;
			current_url = minimum;
		}
		// port is loaded
		if (*current_url == '#'){
			*request = '/';
			*(request + 1) = '\0';
		}
		else{
			char* target = strchr(current_url, '#');
			if (target == NULL){
				if (*current_url == '/'){
					strcpy(request, current_url);
				}
				else{
					*request = '/';
					strcpy(request + 1, current_url);
				}
			}
			else{
				if (*current_url == '/'){
					strncpy(request, current_url, target - current_url);
					request[target - current_url] = '\0';
				}
				else{
					*request = '/';
					strncpy(request + 1, current_url, target - current_url);
					request[target - current_url + 1] = '\0';
				}
			}
		}
		return 0;
	}
public:
	URL(char* new_url){
		url = new char[strlen(new_url) + 1];
		strcpy(url, new_url);
		port = 80;
	}
	void printURL(){
		printf("URL: %s\n", url);
	}
	char* returnHost(){
		return host;
	}
	char* returnRequest(){
		return request;
	}
	unsigned int returnPort(){
		return port;
	}
	int parserURL(){
		printf("        Parsing URL...");
		if (checkScheme(url) != 0){
			return 1;
		}
		if (getHost(url + 7) != 0){
			return 1;
		}
		if (parseOptionalParts(url + 7 + strlen(host)) != 0){
			return 1;
		}
		printf(" host %s, port %d, request %s\n", host, port, request);
		return 0;
	}
};

#endif