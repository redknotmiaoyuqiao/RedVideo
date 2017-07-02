#include <iostream>

extern "C"{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
}

using namespace std;

int main(int argc, char *argv[])
{
    cout << "Hello World!" << endl;

    av_register_all();
    avcodec_register_all();

    //avcodec_decode_video2();

    return 0;
}
