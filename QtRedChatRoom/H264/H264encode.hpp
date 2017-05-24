#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <x264.h>

typedef struct {
    x264_param_t * param;
    x264_t * handle;
    x264_picture_t * picture;
    x264_nal_t * nal;
} X264Encoder;

class H264encode
{
private:
    X264Encoder * encoder;
    int pts_time = 0;
public:
    H264encode(X264Encoder * encoder);
    ~H264encode();

    void h264EncoderInit(int width, int height);
    int h264CompressFrame(int type, uint8_t * in, uint8_t * out);
    void h264EncoderUninit();
};
