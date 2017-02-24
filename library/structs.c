#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>
#include "structs.h"

regex_t regex;

void get_matches(char * pattern, char * target, char ** matches) {
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

    printf("match from index %d to %d: \"%s\"\n", start, end, match);
    // *matches[count] = match;
    strcpy(matches[count], match);

    target = target + end;
    count++;
  }
  free(pmatch);
  regfree(&regex);
}

Request * make_request(char * raw_req) {
  // int reti;
  // int j;
  // int lineNum = 0;
  // regmatch_t* pmatch = malloc(sizeof(regex_t) * (regex.re_nsub + 1));
  Request *r = malloc(sizeof(Request));

  char * matches[103];
  get_matches("([^\r\n]+)", raw_req, matches);
  printf("final match: %s\n", matches[0]);


  // regcomp(&regex, "([^\r\n]+)", REG_EXTENDED);

  // while (regexec(&regex, raw_req, regex.re_nsub + 1, pmatch, 0) == 0){
  //   int start = pmatch[0].rm_so;
  //   int end = pmatch[0].rm_eo;
  //   int len = end - start;
  //   char line[len];

  //   memcpy(line, raw_req + start, len);
  //   line[len] = '\0';

  //   printf("match from index %d to %d: \"%s\"\n", start, end, line);

  //   if (lineNum == 0) {
  //     //Store this line into RequestLine
  //     // Extract each 'word' separated by spaces
  //     regcomp(&regex, "([^ ]+)", REG_EXTENDED);
  //     while (regexec(&regex, line, regex.re_nsub + 1, pmatch, 0) == 0){
  //       int start = pmatch[0].rm_so;
  //       int end = pmatch[0].rm_eo;
  //       int len = end - start;
  //       char match[len];

  //       memcpy(match, raw_req + start, len);
  //       match[len] = '\0';

  //       printf("inner match from index %d to %d: \"%s\"\n", start, end, match);
  //       line = line + end;
  //     }
  //   }
  //   raw_req = raw_req + end;
  //   lineNum++;
  // }
  // free(pmatch);
  // regfree(&regex);
  return r;
}

int main() {
  char * raw_req = "GET /api/testing HTTP/1.1\r\nCookie:chocolate chip\r\nAccept:*/*";
  Request *req = make_request(raw_req);
  return 0;
}
