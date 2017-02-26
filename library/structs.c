#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>
#include <time.h>
#include "structs.h"
#include "codes.h"

regex_t regex;

#define HTTP_VERSION "HTTP/1.0"
#define MAX_LINES 103
#define REQUEST_LINE_MAX_FIELDS 3
#define HEADER_MAX_FIELDS 2
#define DATE_LEN 30

const char *days_of_week[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
const char *months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug",
                        "Sep", "Oct", "Nov", "Dec"};
/*
 * An example of the preferred format is
 * Sun, 06 Nov 1994 08:49:37 GMT ; IMF-fixdate
*/
char *get_time_stamp(){
    time_t current_time = time(NULL);
    struct tm *gmt = gmtime(&current_time);
    char *date_str = malloc(DATE_LEN*(sizeof(char)));
    sprintf(date_str, "%s, %02i %s %04i %02i:%02i:%02i GMT", days_of_week[gmt->tm_wday],
                                                        gmt->tm_mday,
                                                        months[gmt->tm_mon],
                                                        ((gmt->tm_year)+1900),
                                                        gmt->tm_hour,
                                                        gmt->tm_min,
                                                        gmt->tm_sec);
    return date_str;
}

void get_matches(char * pattern, char * target, char *** matches, size_t size)
{
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

Request * make_request(char * raw_req)
{
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

Response *build_response(int status_code, char *body)
{
    char *reason_phrase = malloc(REASON_BUFFER_SIZE*sizeof(char));
    get_reason_phrase(status_code, reason_phrase);

    Status *status = malloc(sizeof(Status));
    *status = (Status) { status_code, reason_phrase };

    StatusLine *status_line = malloc(sizeof(StatusLine));
    *status_line = (StatusLine) { HTTP_VERSION, status };

    MessageHeader date = {"Date", get_time_stamp()};

    Response *response = malloc(sizeof(Response));
    *response = (Response) { status_line, {}, 1, body };
    response->headers[0] = date;

    return response;
}

void clear_response(Response *response)
{
    free(response->status_line->status->reason_phrase);
    free(response->status_line->status);
    free(response->status_line);
    free(response);
}
