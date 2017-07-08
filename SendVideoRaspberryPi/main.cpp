#include <iostream>
#include <stdio.h>
#include "Camera/Camera.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    cout << "Hello World!" << endl;

    Camera * camera = new Camera();
    camera->OpenCamera("/dev/video0",800,480);

    while(1){
        unsigned char * yuv420data = camera->read_yuv420_frame();
        puts("--------------------\n");
        printf("%d\n",yuv420data[0]);
        printf("%d\n",yuv420data[1]);
        printf("%d\n",yuv420data[2]);
        puts("--------------------\n");
    }

    delete camera;

    return 0;
}
