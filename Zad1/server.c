#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <signal.h>
#include <unistd.h>
#include <math.h>
#include "message.c"

int main ()
{
    int server_sockfd, client_sockfd;
    socklen_t server_len, client_len;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;

    server_sockfd = socket (AF_INET, SOCK_STREAM, 0);

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl (INADDR_ANY);
    server_address.sin_port = htons (9734);
    server_len = sizeof (server_address);
    bind (server_sockfd, (struct sockaddr *) &server_address, server_len);

    /*  Create a connection queue, ignore child exit details and wait for clients.  */

    listen (server_sockfd, 5);

    signal (SIGCHLD, SIG_IGN);

    while (1)
    {
        char ch[4];


        printf ("server waiting\n");

        /*  Accept connection.  */

        client_len = sizeof (client_address);
        client_sockfd = accept (server_sockfd,
                                (struct sockaddr *) &client_address,
                                &client_len);
        printf("Accepted connection\n");
        /*  Fork to create a process for this client and perform a test to see
            whether we're the parent or the child.  */


        if (fork () == 0)
        {

            /*  If we're the child, we can now read/write to the client on client_sockfd.
                The five second delay is just for this demonstration.  */
            printf("forked\n");

            message request;
            int message_len = get_message_size();
            byte * buffor = (byte*)malloc(message_len);


            while(1){
                int buffor_len = read (client_sockfd, buffor, message_len);
                if(buffor_len < 1) break;

                deserialize_message(&request, buffor);
                char * message_code = get_message_code(&request);
                printf("The message code: %s\n", message_code);

                if(strcmp(message_code, SQRT_REQUEST_CODE) == 0){
                    int id = get_rq_id(&request);
                    double number = get_number(&request);
                    double sqrt_number = sqrt(number);

                    message response;
                    byte code [4] = SQRT_RESPONSE_CODE;
                    write_message_code(&response, code);
                    write_rq_id(&response, get_rq_id(&request));
                    write_number(&response, sqrt_number);

                    byte * buffor = serialize_message(&response);
                    write (client_sockfd, buffor, get_message_size());
                    printf("Send sqrt response\n");
                }

                if(strcmp(message_code, TIME_REQUEST_CODE) == 0){
                    int id = get_rq_id(&request);

                    message response;
                    byte code [4] = TIME_RESPONSE_CODE;
                    write_message_code(&response, code);
                    write_rq_id(&response, get_rq_id(&request));
                    write_length(&response, 3);
                    write_time(&response, "abc", 3);

                    byte * buffor = serialize_message(&response);

                    write (client_sockfd, buffor, get_message_size());
                    free(buffor);
                    printf("Send time response\n");

                }
            }



            printf("Close connection\n");
            close (client_sockfd);
            exit (0);
        }

            /*  Otherwise, we must be the parent and our work for this client is finished.  */

        else
        {
            close (client_sockfd);
        }
    }
}