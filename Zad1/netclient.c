/*  Make the necessary includes and set up the variables.  */

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "message.c"

static int id = 1;
int get_next_id(){
    return id++;
}

static int request_counter = 0;

void send_sqrt_request(int sockfd, double number){
    request_counter++;
    printf("Send sqrt request\n");
    message request;
    char code [4] = SQRT_REQUEST_CODE;
    write_message_code(&request, code);
    write_rq_id(&request, get_next_id());
    write_number(&request, number);
    byte * buffor = serialize_message(&request);

    write (sockfd, buffor, get_message_size());
    free(buffor);
}

void send_time_request(int sockfd){
    request_counter++;
    printf("Send time request\n");
    message request;
    char code [4] = TIME_REQUEST_CODE;
    write_message_code(&request, code);
    write_rq_id(&request, get_next_id());
    byte * buffor = serialize_message(&request);
    write (sockfd, buffor, get_message_size());
    free(buffor);
}

void read_and_handle_data(int sockfd){

    byte * buffor = (byte*) malloc(get_message_size());
    while(1){

        message response;
        printf("Reading... \n");
        int buffor_len = read (sockfd, buffor, get_message_size());
        printf("Buffor_len: %d\n", buffor_len);
        if(buffor_len < 1) break;
        deserialize_message(&response, buffor);
        char * message_code = get_message_code(&response);
        printf("The message code: %s \n", message_code);
        int id = get_rq_id(&response);
        printf("ID: %d \n", id);

        if(strcmp(message_code,  SQRT_RESPONSE_CODE) == 0){
            request_counter--;
            printf("The sqrt number: %f\n", get_number(&response));
        }

        else if(strcmp(message_code, TIME_RESPONSE_CODE) == 0) {
            request_counter--;
            int len = get_length(&response);
            char *msg = get_time(&response, len);
            printf("The time: %s\n", msg);
        }

        if(request_counter == 0) break;
    }

    free(buffor);
}

int main ()
{
    int sockfd;
    socklen_t len;
    struct sockaddr_in address;
    int result;


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
    send_time_request(sockfd);
    send_sqrt_request(sockfd, 5.2123);


    write (sockfd, NULL, 1);


    read_and_handle_data(sockfd);

    close (sockfd);

    exit (0);
}