#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "date.h"

const char *days_of_week[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
const char *months[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug",
                           "Sep", "Oct", "Nov", "Dec" };
/*
 * An example of the preferred format is
 * Sun, 06 Nov 1994 08:49:37 GMT ; IMF-fixdate
*/
char *get_time_stamp()
{
    time_t current_time = time(NULL);
    struct tm *gmt = gmtime(&current_time);
    char *date_str = malloc(DATE_LEN * sizeof(char));
    sprintf(date_str, "%s, %02i %s %04i %02i:%02i:%02i GMT",
                                                    days_of_week[gmt->tm_wday],
                                                    gmt->tm_mday,
                                                    months[gmt->tm_mon],
                                                    ((gmt->tm_year) + 1900),
                                                    gmt->tm_hour,
                                                    gmt->tm_min,
                                                    gmt->tm_sec);
    return date_str;
}
