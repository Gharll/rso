#include <stdlib.h>
#include <string.h>

#define DATA_MAX_SIZE 128
#define SQRT_REQUEST_CODE "0001"
#define SQRT_RESPONSE_CODE "1001"
#define TIME_REQUEST_CODE "0002"
#define TIME_RESPONSE_CODE "1002"

typedef unsigned char byte;
typedef struct message{
    byte message_code[4 * sizeof(char)];
    byte rq_id[sizeof(int)];
    byte data[DATA_MAX_SIZE];
}message;

void write_message_code(message * self, char * code){
    memcpy(self->message_code, code, 4 * sizeof(char));
}

void write_rq_id(message * self, int rq_id){
    memcpy(self->rq_id, &rq_id, sizeof(int));
}

void write_number(message * self, double number){
    memcpy(self->data, &number, sizeof(double));
}

void write_length(message * self, int length){
    memcpy(self->data, &length, sizeof(int));
}

void write_time(message * self, char * time, int length){
    byte * time_wsk = self->data + sizeof(int);
    memcpy(time_wsk, time, length);
}

byte * serialize_message(message * self){
    byte * serialized_message = (byte*) malloc(get_message_size());
    byte * buffor = serialized_message;
    int len;

    len = sizeof(self->message_code);
    memcpy(buffor, self->message_code, len);
    buffor += len;

    len = sizeof(self->rq_id);
    memcpy(buffor, self->rq_id, len);
    buffor += len;

    len = DATA_MAX_SIZE;
    memcpy(buffor, self->data, len);
    buffor += DATA_MAX_SIZE;

    return serialized_message;
}


void deserialize_message(message * self, byte * buffor){
    int len;
    byte * ptr = buffor;

    len = sizeof(self->message_code);
    memcpy(self->message_code, ptr, len);
    ptr += len;

    len = sizeof(self->rq_id);
    memcpy(self->rq_id, ptr, len);
    ptr += len;

    len = sizeof(self->data);
    memcpy(self->data, ptr, len);
    ptr += len;
}

char * get_message_code(message * self){
    char * code = malloc(sizeof(self->message_code) + 1);
    memcpy(code, self->message_code, sizeof(self->message_code));
    code[4] = '\0';
    return code;
}

int get_rq_id(message * self){
    int rq_id = -1;
    memcpy(&rq_id, self->rq_id, sizeof(int));
    return rq_id;
}

double get_number(message * self){
    double number = -1;
    memcpy(&number, self->data, sizeof(double));
    return number;
}

int get_length(message * self){
    int length = -1;
    memcpy(&length, self->data, sizeof(int));
    return length;
}

char * get_time(message * self, int length){
    char * time_message = malloc(sizeof(char) * length) ;
    byte * time_wsk = self->data + sizeof(int);
    memcpy(time_message, time_wsk, length);
    return time_message;
}

int get_base_message_size(){
    return 4*sizeof(char) + sizeof(int);
}

int get_message_size(){
    return get_base_message_size() + DATA_MAX_SIZE;
}



