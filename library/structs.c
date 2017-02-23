#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include "structs.h"

Request * make_request(char * raw_req) {
  int reti;
  int j;
  int lineNum = 0;
  size_t nmatch = 2;
  regmatch_t* pmatch = malloc(sizeof(regex_t) * (regex.re_nsub + 1));
  Request *r = malloc(sizeof(Request));

  regcomp(&regex, "([^\r\n]+)", REG_EXTENDED);

  while (regexec(&regex, raw_req, regex.re_nsub + 1, pmatch, 0) == 0){
    int start = pmatch[0].rm_so;
    int end = pmatch[0].rm_eo;
    char * match[end - start];

    printf("match from index %d to %d: ", start, end);
    memcpy(match, raw_req + start, end - start);
    printf("%s\n", match);

    // Print the match char-by-char
    // for(j = pmatch[0].rm_so; j < pmatch[0].rm_eo; j++) {
    //  printf("%c", raw_req[j]);
    // }
    // r.startline = current_match;
    printf("\n");
    raw_req = raw_req + pmatch[0].rm_eo;
    lineNum++;
  }
  free(pmatch);
  regfree(&regex);
  return r;
}

int main() {
  // request req = {{POST, "/api/helloworld", "HTTP/1.1"}};
  // req.headers[0] = (messageheader) {"Cookie", "chocolate chip"};
  // req.headers[1] = (messageheader) {"Accept", "*/*"};

  // response res = {{"HTTP/1.1", {200, "OK"}}};
  // res.headers[0] = (messageheader) {"Content-Type","text/html"};
  // res.headers[1] = (messageheader) {"Server","dankwebsite.com"};
  // res.body = "<h1>Hello World!</h1>";


  char * raw_req = "GET /api/testing HTTP/1.1\r\nCookie:chocolate chip\r\nAccept:*/*";

  request *req = make_request(raw_req);

  return 0;
}
