#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

using namespace std;

typedef struct pth_arg
{
    int start;
    int end;
}pth_arg;

void * th_fun(void * arg){
    pth_arg p_arg = *((pth_arg * )arg);

    int sum = 1000;

    for(int i=p_arg.start;i<p_arg.end;i++){
        puts("wwwwwwwwww\n");
        //sum = sum + i;
    }

    //return (void *)sum;
    pthread_exit((void *)sum);
}

int main(int argc, char *argv[])
{
    pthread_t pth1;
    pthread_t pth2;

    int err;

    pth_arg w = {0,100};
    pth_arg * arg = &w;

    err = pthread_create(&pth1,NULL,th_fun,(void *)arg);
    if(err != 0){
        fprintf(stderr,"pthread_create :%s\n",strerror(err));
        exit(1);
    }

    int ret;
    pthread_join(pth1,(void **)&ret);

    printf("\nwwwwww%d\n",ret);

    cout << "Finish" << endl;

    return 0;
}
