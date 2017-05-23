#include <stdint.h>
#include <stdio.h>

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
public:
    H264encode();
    ~H264encode();

    void h264_encoder_init(X264Encoder * encoder, int width, int height);
    int h264_compress_frame(X264Encoder * encoder, int type, uint8_t * in, uint8_t * out);
    void h264_encoder_uninit(X264Encoder * encoder);
};
