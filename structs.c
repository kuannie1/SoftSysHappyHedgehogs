#include <stdio.h>
#include <stdlib.h>
#include <regex.h>


typedef enum {
	HEAD, POST, GET, PUT, DELETE, OPTIONS, CONNECT
} request_type;

typedef struct messageheader {
	char * field_name;
	char * field_value; //is field_content
} messageheader;

typedef struct req_startline {
	request_type req_type;
	char * url;
	char * http_ver;
} req_startline;

typedef struct status {
	int code;
	char * reason_phrase;
} status;

typedef struct res_startline {
	char * http_ver;
	status status_code;
} res_startline;

typedef struct response {
	res_startline startline;
	messageheader headers[101];
	char * body;
} response;

typedef struct request {
	req_startline startline;
	messageheader headers[101];
	char * body;
} request;

int main() {
	request req = {{POST, "/api/helloworld", "HTTP/1.1"}};
	req.headers[0] = (messageheader) {"Cookie", "chocolate chip"};
	req.headers[1] = (messageheader) {"Accept", "*/*"};

	response res = {{"HTTP/1.1", {200, "OK"}}};
	res.headers[0] = (messageheader) {"Content-Type","text/html"};
	res.headers[1] = (messageheader) {"Server","dankwebsite.com"};
	res.body = "<h1>Hello World!</h1>";

	char * raw_req = "GET /api/testing tester HTTP/1.1\r\nCookie:chocolate chip\r\nAccept:*/*";
	// Parse Req
	regex_t regex;
	int reti;
	int i;
	int j;
	size_t nmatch = 2;
	// regmatch_t pmatch[2];
	regmatch_t* pmatch = malloc(sizeof(regex_t) * (regex.re_nsub + 1));

	regcomp(&regex, "\\(.*?\r\n\\)", 0);
	// regcomp(&regex, "\\w*:\\w*", 0);
	// regcomp(&regex, "\\(test\\)(?:ing)", 0);
	// reti = regexec(&regex, raw_req, nmatch, pmatch, 0);
	// printf("a matched substring \"%.*s\" is found at position %d to %d.\n",       
	//      pmatch[1].rm_eo - pmatch[1].rm_so, &raw_req[pmatch[1].rm_so],  
	//      pmatch[1].rm_so, pmatch[1].rm_eo - 1);

	if ((reti = regexec(&regex, raw_req, regex.re_nsub + 1, pmatch, 0))){
	        printf("no match\n");
	        /* error or no match */
	    } else {
	    	printf("match!\n");
	    	// printf("a matched substring \"%.*s\" is found at position %d to %d.\n",       
	    	//      pmatch[1].rm_eo - pmatch[1].rm_so, &raw_req[pmatch[1].rm_so],  
	    	//      pmatch[1].rm_so, pmatch[1].rm_eo - 1);
	        for(i = 0; i < regex.re_nsub; i++) {
	            printf("match %d from index %d to %d: ", i, pmatch[i].rm_so,
	                   pmatch[i].rm_eo);
	            for(j = pmatch[i].rm_so; j < pmatch[i].rm_eo; j++) {
	                printf("%c", raw_req[j]);
	            }
	            printf("\n");
	        }
	    }
	free(pmatch);
	regfree(&regex);
	return 0;
}