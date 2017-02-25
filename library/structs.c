#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <time.h>
#include "structs.h"
#include "codes.h"

#define HTTP_VERSION "HTTP/1.0"
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
    printf("%s\n", days_of_week[gmt->tm_wday]);
    printf("%s\n", months[gmt->tm_mon]);
    sprintf(date_str, "%s, %02i %s %04i %02i:%02i:%02i GMT", days_of_week[gmt->tm_wday],
                                                        gmt->tm_mday,
                                                        months[gmt->tm_mon],
                                                        ((gmt->tm_year)+1900),
                                                        gmt->tm_hour,
                                                        gmt->tm_min,
                                                        gmt->tm_sec);
    printf("%s\n", date_str);
    return date_str;
}

Response *build_response(int status_code, char *body)
{
    char reason_phrase[REASON_BUFFER_SIZE];
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

// int main()
// {
//  Request req = {{POST, "/api/helloworld", "HTTP/1.1"}};
//  req.headers[0] = (MessageHeader) {"Cookie", "chocolate chip"};
//  req.headers[1] = (MessageHeader) {"Accept", "*/*"};

//  Response res = {{"HTTP/1.1", {200, "OK"}}};
//  res.headers[0] = (MessageHeader) {"Content-Type","text/html"};
//  res.headers[1] = (MessageHeader) {"Server","dankwebsite.com"};
//  res.body = "<h1>Hello World!</h1>";

//  return 0;
// }
