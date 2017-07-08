#include <iostream>

#include "Camera/Camera.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    cout << "Hello World!" << endl;

    Camera * camera = new Camera();
    camera->OpenCamera("/dev/video0",800,480);

    while(1){
        char * yuv420data = camera->read_yuv420_frame();
        puts("--------------------");
        println("%d",yuv420data[0]);
        println("%d",yuv420data[1]);
        println("%d",yuv420data[2]);
        puts("--------------------");
    }

    delete camera;

    return 0;
}
