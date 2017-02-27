/* Basic HTTP server library
 * Method to map status codes to their reason phrase.
 * Created February 2017 by Sam Myers, Serena Chen, Anne Ku, and Bill Wong
 */

#include <string.h>
#include <stdio.h>
#include "codes.h"

/* List of status codes our library supports; maps to the list of reason phrases
 */
const int codes[NUM_CODES] = {
    100,
    200,
    204,
    400,
    404,
    405,
    500
};

/* List of reason phrases our library supports; maps to the list of status codes
 */
const char *phrases[NUM_CODES] = {
    "Continue",
    "OK",
    "No Content",
    "Bad Request",
    "Not Found",
    "Method Not Allowed",
    "Internal Server Error"
};

/* Given a status code, get the reason phrase mapped to it
 *
 * status_code: the code to use as a key
 * phrase: string to pack the reason phrase into
 *
 * returns: 0 if successful, else -1
 */
int get_reason_phrase(int status_code, char *phrase)
{
    size_t i = 0;
    while (i < NUM_CODES && status_code != codes[i]) i++;

    if (i >= NUM_CODES) {
        return -1;
    } else {
        sprintf(phrase, "%s", phrases[i]);
        return 0;
    }
}
