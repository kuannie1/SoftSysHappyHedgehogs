#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>
#include "structs.h"

regex_t regex;

#define MAX_LINES 103
#define REQUEST_LINE_MAX_FIELDS 3
#define HEADER_MAX_FIELDS 2

void get_matches(char * pattern, char * target, char *** matches, size_t size) {
  *matches = malloc(size * sizeof(char *));
  regmatch_t* pmatch = malloc(sizeof(regex_t) * (regex.re_nsub + 1));
  int count = 0;

  regcomp(&regex, pattern, REG_EXTENDED);
  while (regexec(&regex, target, regex.re_nsub + 1, pmatch, 0) == 0){
    int start = pmatch[0].rm_so;
    int end = pmatch[0].rm_eo;
    int len = end - start;
    char match[len];

    memcpy(match, target + start, len);
    match[len] = '\0';

    (*matches)[count] = strdup(match);

    target = target + end;
    count++;
  }
  free(pmatch);
  regfree(&regex);
}

Request * make_request(char * raw_req) {
  Request *r = malloc(sizeof(Request));
  RequestLine *requestLine = malloc(sizeof(RequestLine));
  int i;
  char ** matches;

  get_matches("([^\r\n]+)", raw_req, &matches, MAX_LINES);

  for(i = 0; i < MAX_LINES; i++){
    printf("match in main: %s\n", matches[i]);
    if (matches[i] == NULL) break;
    char ** wordMatches;
    if (i == 0) {
      get_matches("([^ ]+)", matches[i], &wordMatches, REQUEST_LINE_MAX_FIELDS);
      requestLine->url = wordMatches[1];
      requestLine->http_ver = wordMatches[2];
    } else if (i > 0) {
      get_matches("([^:]+)", matches[i], &wordMatches, HEADER_MAX_FIELDS);
      MessageHeader *header = malloc(sizeof(MessageHeader));
      header->field_name = wordMatches[0];
      header->field_value = wordMatches[1];
      r->headers[i-1] = *header;
    }
    // TODO handle body
  }
  r->request_line = *requestLine;
  return r;
}

int main() {
  char * raw_req = "GET /api/testing HTTP/1.1\r\nCookie:chocolate chip\r\nAccept:*/*\r\n\r\nbodytesting";
  Request *req = make_request(raw_req);
  printf("FINAL URL: %s\n", req->request_line.url);
  printf("FINAL HEADER1: %s\n", req->headers[0].field_name);
  printf("FINAL HEADER2: %s\n", req->headers[1].field_name);
  return 0;
}
