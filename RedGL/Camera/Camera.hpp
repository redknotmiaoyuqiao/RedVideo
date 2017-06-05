#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <libv4l2.h>
#include <linux/videodev2.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <assert.h>
#include <sys/ioctl.h>

class Camera
{
private:
    int fd;

    struct buffer {
            void * start;
            size_t length;
    };

    struct buffer * buffers= NULL;

    unsigned int n_buffers = 0;

    struct v4l2_buffer buf;

    int width = 0;
    int height = 0;

    unsigned char * yuv420_data = nullptr;

    void set_input();
    void get_info();
    void get_video_info();
    void start_capturing();
public:
    Camera();
    ~Camera();
    bool OpenCamera(char * id,int width,int height);
    void CloseCamera();

    unsigned char * read_frame ();
    unsigned char * read_yuv420_frame ();
};
