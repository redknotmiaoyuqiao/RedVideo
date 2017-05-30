#include <iostream>

#include <pthread.h>

using namespace std;

int main(int argc, char *argv[])
{
    cout << "Hello World!" << endl;

    int t = pthread_create();
    if(t == 0){
        cout << "Success" << endl;
    }

    return 0;
}
