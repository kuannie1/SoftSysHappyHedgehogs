#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>
#include "structs.h"

regex_t regex;

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
    int len = end - start;
    char match[len];

    memcpy(match, raw_req + start, len);
    match[len] = '\0';

    printf("match from index %d to %d: \"%s\"\n", start, end, match);

    raw_req = raw_req + end;
    lineNum++;
  }
  free(pmatch);
  regfree(&regex);
  return r;
}

int main() {
  char * raw_req = "GET /api/testing HTTP/1.1\r\nCookie:chocolate chip\r\nAccept:*/*";
  Request *req = make_request(raw_req);
  return 0;
}
