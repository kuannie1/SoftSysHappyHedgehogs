# Web Server Library in C

### Description

We made library for a webserver in C that handles requests, responses for any
basic web app. We split this project into two main portions: structs to parse
the HTTP traffic, and a wrapper for talking to the socket. The idea is that a
user who wants to write a web application with our library only has to write a
single function. The function should take in the broken down HTTP request as a
struct and determine the response code and body of the response. Our library
handles processes such as converting between HTTP requests/responses and structs,
and writing to sockets, and handling connections.

We relied heavily on the HTTP specifications as well as using the examples for
socket communication from Head First C. We also looked at a HTTP server library
called [Tiny HTTPd](https://sourceforge.net/projects/tinyhttpd/)
to help us with starting our own server.

### Authors
[Anne Ku](https://github.com/kuannie1),
[Samuel Myers](https://github.com/sammyers),
[Serena Chen](https://github.com/poosomooso),
[William Wong](https://github.com/billmwong),

### Getting Started (Installation Instructions)
	$ git clone https://github.com/kuannie1/SoftSysHappyHedgehogs.git
	$ cd SoftSysHappyHedgehogs
	$ make

### Usage
To use this library, include `server.h` at the top of your file. 
Then call `Application *app = create_application(PORT, QUEUE_SIZE);`, where
`PORT` and `QUEUE_SIZE` are, respectively, the port number and the number
of connection to handle at once. Then, map functions to endpoints using the
`register_endpoint` function. To start the server, call `start_server()` 
with the Application you have just created.

### License
Copyright 2017 Anne Ku, Samuel Myers, Serena Chen, and William Wong

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
