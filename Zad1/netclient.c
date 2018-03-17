/*  Make the necessary includes and set up the variables.  */

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "message.c"

int main ()
{
    int sockfd;
    socklen_t len;
    struct sockaddr_in address;
    int result;
    message m;

    /*  Create a socket for the client.  */
    sockfd = socket (AF_INET, SOCK_STREAM, 0);

    /*  Name the socket, as agreed with the server.  */
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr ("127.0.0.1");
    address.sin_port = htons (9734);
    len = sizeof (address);

    /*  Now connect our socket to the server's socket.  */
    result = connect (sockfd, (struct sockaddr *) &address, len);

    if (result == -1)
    {
        perror ("oops: netclient");
        exit (1);
    }

    /*  We can now read/write via sockfd.  */
    byte code [] = {'0','0','0','1'};
    write_message_code(&m, code);
    write_rq_id(&m, 100);
    write_number(&m, 2.35);

    byte * buffor = serialize_message(&m);

    write (sockfd, buffor, get_message_size(&m));

    read (sockfd, &buffor, get_message_size(&m));

    message response;
    byte * buffor2 = (byte*) malloc(sizeof(message));
    deserialize_message(&response, buffor2);

    printf ("char from server = %f\n", get_number(&response));
    close (sockfd);
    exit (0);
}