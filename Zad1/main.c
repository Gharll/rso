#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "message.c"




int main()
{
    char a[] = "abc";
    char * b = a+2;

    printf("%s", b);
    /*
     message m;
     write_number(&m, 2.34);
     byte * buffor = serialize_message(&m);

     message m2;
     deserialize_message(&m2, buffor);

     printf("%f", get_number(&m2));

      byte a = 1;

     double number = 5.23;
     number /= 3;
     number *= 10000000000000;

     printf("Number: %f\n", number);
     printf("Size = %d\n",sizeof(byte));

     message m;



     char buffer[] = "jakis napis";
     char *p = buffer;
     p+=2;

     //buffer -= 2*sizeof(char);

     printf("%c", *p);

     printf("%d \n", sizeof(message));
     int my_size = 4 * sizeof(byte) +  sizeof(byte) * sizeof(int) + sizeof(byte) * sizeof(double);
     printf("%d \n", my_size);

     char napis[] = "jakis napis";
     char to_cpy[2] = "ko";

     memset(napis +2, to_cpy, sizeof(to_cpy));
     puts(napis);

      */
/*

    int a = 88;
    int *ptr = &a;
    double d1 = 55;

    byte rq_id[sizeof(int)];
    byte number[sizeof(double)];
    byte * message = (byte*) malloc(sizeof(int) + sizeof(double));
    byte *buffor = message;

    memcpy(rq_id, ptr, sizeof(int));
    memcpy(buffor, rq_id, sizeof(int));
    buffor += sizeof(int);

    memcpy(number, &d1, sizeof(double));
    memcpy(buffor, number, sizeof(double));
    buffor += sizeof(double);

    int b = -1;
    double d2 = -1;

    buffor -= sizeof(int) + sizeof(double);
    memcpy(&b, buffor, sizeof(int));

    buffor += sizeof(int);
    memcpy(&d2, buffor, sizeof(double));

    printf("%d\n", b);
    printf("%f\n", d2);


   /* int a = 5;
    byte s[sizeof(int)];
    int b;

    memcpy(s, &a, sizeof(int));
    memcpy(&b, s, sizeof(int));

    printf("%d", b); */

    return 0;
}
