#include <string.h>
#include <stdio.h>
#include "codes.h"

int codes[NUM_CODES] = {
    100,
    200,
    204,
    400,
    404,
    405,
    500
};

const char *phrases[NUM_CODES] = {
    "Continue",
    "OK",
    "No Content",
    "Bad Request",
    "Not Found",
    "Method Not Allowed",
    "Internal Server Error"
};

int get_reason_phrase(int status_code, char *phrase)
{
    size_t i = -1;
    while (i < NUM_CODES && status_code != codes[i]) i++;

    if (i < NUM_CODES) {
        return -1;
    } else {
        sprintf(phrase, "%s", phrases[i]);
        return 0;
    }
}
