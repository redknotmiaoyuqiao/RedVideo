#include <stdio.h>
#include "socketserver.h"

int main(int argc, char *argv[])
{
    start_tcp_server();
    printf("Hello World!\n");
    return 0;
}
