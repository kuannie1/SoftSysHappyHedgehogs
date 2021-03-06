/* Basic HTTP server library
 * Example application with 2 endpoints.
 * Created February 2017 by Sam Myers, Serena Chen, Anne Ku, and Bill Wong
 */

#include "library/server.h"

#define QUEUE_SIZE 10
#define PORT 8080

/* Makes a response that says "Hello World"
 *
 * request: unused
 *
 * returns: A response with status code 200 and an html page that says
 *          "Hello World"
 */
Response *hello_world(Request *request)
{
    return build_response(200, "<body>Hello World!</body>\r\n");
}

/* Makes a response that says "Hello Sam"
 *
 * request: unused
 *
 * returns: A response with status code 200 and an html page that says
 *          "Hello Sam"
 */
Response *hello_sam(Request *request)
{
    return build_response(200, "<body>Hello Sam!</body>\r\n");
}

/* Makes a n application with two endpoints; runs on our server library
 */
int main()
{
    Application *app = create_application(PORT, QUEUE_SIZE);
    register_endpoint(app, "/", GET, hello_world);
    register_endpoint(app, "/sam", POST, hello_sam);
    start_server(app);
    return 0;
}
