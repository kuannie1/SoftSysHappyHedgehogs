#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include "structs.h"
#include "codes.h"

Response *build_response(int status_code, char *body)
{
	Response *response = malloc(sizeof(Response));
	char reason_phrase[REASON_BUFFER_SIZE];
	get_reason_phrase(status_code, reason_phrase);
	const Status status = {status_code, reason_phrase};
	const StatusLine status_line = {"HTTP/1.1", &status};
	response->status_line = &status_line;
	response->body = body;
	return response;
}

// int main()
// {
// 	Request req = {{POST, "/api/helloworld", "HTTP/1.1"}};
// 	req.headers[0] = (MessageHeader) {"Cookie", "chocolate chip"};
// 	req.headers[1] = (MessageHeader) {"Accept", "*/*"};

// 	Response res = {{"HTTP/1.1", {200, "OK"}}};
// 	res.headers[0] = (MessageHeader) {"Content-Type","text/html"};
// 	res.headers[1] = (MessageHeader) {"Server","dankwebsite.com"};
// 	res.body = "<h1>Hello World!</h1>";

// 	return 0;
// }
