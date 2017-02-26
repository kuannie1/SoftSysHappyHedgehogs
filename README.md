## Web Server

### Description

We made library for a webserver in C that handles requests, responses for any
basic web app. We split this project into two main portions: structs to parse
the HTTP traffic, and a wrapper for talking to the socket. The idea is that a
user who wants to write a web application with our library only has to write a
single function. The function should take in the broken down HTTP request as a
struct and determine the response code and body of the response. Our library
handles processes such as converting between HTTP requests/responses and structs, 
and writing to sockets, and handling connections.

### Background
This is our first project for ENGR 3525: Software Systems. We wanted to work
with servers, so we chose to create a library that simplifies the process of
handling HTTP requests. By simplifying the creation of web applications, we
<!-- lower the barrier to entry into the field of computer science as a whole. -->
get to understand how servers communicate in a network and help others deploy
their ideas efficiently. 

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
	$ make ___ (instructions for compiling all the server files)

### Usage

### Implementation
The library has two main aspects: Communicating with the socket and sending data
through the HTTP protocol. We spent a long time figuring out how to architect
our system and modularize the code so all 4 of us could work on it.

In order to make this a library that a user could interface with, we had to be
able to insert the desired server logic in the standardized library methods. We
were initially imagining something where the user could call a function to start
the server, and then the server would somehow interrupt or prompt the user's
application when there was new data to process. However, after using pthreads,
we found out that it was possible to pass functions as parameters. Using that,
we were able to only require that the user's application pass in a function with
the right inputs and outputs when starting the server.

To handle the incoming data, we decided to read the incoming HTTP request strings 
character by character. We had to do this 

### Results
<!-- Add images, screenshots, and videos here -->

### License
Copyright 2017 Anne Ku, Samuel Myers, Serena Chen, and William Wong

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


<!-- ## Content we need to cover:

Big Idea/Abstract

The first thing someone should see when they land on your site is a quick and easily understandable explanation of what your project is all about.

Background

Provide context for your project by describing the broader space in which it is situated. This section will likely draw upon your annotated bibliography. You've already collected this knowledge and shown us you understand it, now frame it for an external audience.

Implementation

What specifically did you accomplish with this project? Within the context of the problem space, enumerate the potential options and explain why you chose what you did. Describe what makes it interesting/challenging, and how you overcame those challenges. Explain your implementation and design decisions with sufficient detail for a technical audience to understand it.

The library has two main aspects: Communicating with the socket and sending data
through the HTTP protocol. We spent a long time figuring out how to architect
our system and modularize the code so all 4 of us could work on it.

In order to make this a library that a user could interface with, we had to be
able to insert the desired server logic in the standardized library methods. We
were initially imagining something where the user could call a function to start
the server, and then the server would somehow interrupt or prompt the user's
application when there was new data to process. However, after using pthreads,
we found out that it was possible to pass functions as parameters. Using that,
we were able to only require that the user's application pass in a function with
the right inputs and outputs when starting the server.

Results

Provide evidence demonstrating that what you built works. Though the details will be different for each project, screenshots and video are likely helpful. Include graphs or other data if appropriate.


Having trouble with Pages? Check out our [documentation](https://help.github.com/categories/github-pages-basics/) or [contact support](https://github.com/contact) and we’ll help you sort it out.
 -->
