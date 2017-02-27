#include "library/server.h"

#define QUEUE_SIZE 10
#define PORT 8080

Response *hello_world(Request *request)
{
    return build_response(200, "<body>Hello World!</body>");
}

int main()
{
    Application *app = create_application(PORT, QUEUE_SIZE);
    register_endpoint(app, "/", hello_world);
    start_server(app);
    return 0;
}
