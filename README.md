## Web Server

### Description

We made library for a webserver in C that handles requests, responses for a basic web app.

### Authors
[Anne Ku](https://github.com/kuannie1),
[Samuel Myers](https://github.com/sammyers),
[Serena Chen](https://github.com/poosomooso),
[William Wong](https://github.com/billmwong),

### Getting Started (Installation Instructions)
	$ git clone https://github.com/kuannie1/SoftSysHappyHedgehogs.git
	$ make ___
### Usage

### License

## Content we need to cover:

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
