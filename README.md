# serverclient
Example of server-client communication using tcp sockets and threads.

**File: alexserver.c**
This code is a simple server application that opens a socket to receive communication requests. For any client it creates a new thread. In the thread handler, the code will be waiting for a terminate command, a read command or a new information string.
In the first case, the application will end. In the second case, the server returns the information string that has been previously stored. In the third case, the server reads the new information and stores it to use it later.

The server will keep a log file including a line for each connection. The line includes timestamp, client IP and the world Modify or Read.
 
**File: alexclient.c**
The application will establish a communication channel with the
server of the previous section. This socket client offers a menu
to the user, asking for a ’t’, ‘w’ or ‘r’.
’t' means terminate the server application.
‘r’ means read the stored information.
If the user selects ‘w’, the application will ask for a new line,
that is the information the user wants to send to the server to be
stored.
