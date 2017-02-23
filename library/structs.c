#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include "structs.h"

int main() {
	Request req = {{POST, "/api/helloworld", "HTTP/1.1"}};
	req.headers[0] = (MessageHeader) {"Cookie", "chocolate chip"};
	req.headers[1] = (MessageHeader) {"Accept", "*/*"};

	Response res = {{"HTTP/1.1", {200, "OK"}}};
	res.headers[0] = (MessageHeader) {"Content-Type","text/html"};
	res.headers[1] = (MessageHeader) {"Server","dankwebsite.com"};
	res.body = "<h1>Hello World!</h1>";

	return 0;
}
