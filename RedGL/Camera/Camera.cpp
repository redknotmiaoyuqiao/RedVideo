#include "Camera.hpp"

Camera::Camera()
{

}

Camera::~Camera()
{
    this->CloseCamera();
}

bool Camera::OpenCamera(char * id,int width,int height)
{
    this->width = width;
    this->height = height;

    fd = open(id,O_RDWR);
    if (!fd) {
        perror("Error opening device");
    }

    set_input();
    get_info();
    get_video_info();
    start_capturing();
}

void Camera::CloseCamera()
{
    close(fd);
}

void Camera::start_capturing()
{
    unsigned int i;
    enum v4l2_buf_type type;


    for (i = 0; i < n_buffers; ++i) {
        struct v4l2_buffer buf;

        memset(&buf, 0, sizeof(buf));

        buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory      = V4L2_MEMORY_MMAP;
        buf.index       = i;

        if (-1 == ioctl (fd, VIDIOC_QBUF, &buf)) {
            perror ("VIDIOC_QBUF");
            exit(EXIT_FAILURE);
        }
    }

    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if (-1 == ioctl (fd, VIDIOC_STREAMON, &type)) {
        perror ("VIDIOC_STREAMON");
        exit(EXIT_FAILURE);
    }

}


void Camera::set_input() {
    /*
    struct v4l2_fmtdesc fmts;
    int len = 0;
    memset(&fmts, 0, sizeof(fmts));
    fmts.index = 0;
    fmts.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    int ret;
    while ((ret = ioctl(fd, VIDIOC_ENUM_FMT, &fmts)) == 0) {
        printf("{ pixelformat = '%c%c%c%c', description = '%s' }\n",
            fmts.pixelformat & 0xFF, (fmts.pixelformat >> 8) & 0xFF,
            (fmts.pixelformat >> 16) & 0xFF, (fmts.pixelformat >> 24) & 0xFF,
            fmts.description);
    }
    */


    int index;
    index = 0;
    if (-1 == ioctl (fd, VIDIOC_S_INPUT, &index)) {
        perror ("VIDIOC_S_INPUT");
        exit (EXIT_FAILURE);
    }

    struct v4l2_capability cap;
    struct v4l2_cropcap cropcap;
    struct v4l2_crop crop;
    struct v4l2_format fmt;
    unsigned int min;

    if (-1 == ioctl (fd, VIDIOC_QUERYCAP, &cap)) {
        if (EINVAL == errno) {
            fprintf (stderr, "Device is no V4L2 device\n");
            exit (EXIT_FAILURE);
        } else {
            perror ("VIDIOC_QUERYCAP");
            exit(EXIT_FAILURE);
        }
    }

    if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
        fprintf (stderr, "Device is no video capture device\n");
        exit (EXIT_FAILURE);
    }


    if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
        fprintf (stderr, "Device does not support streaming i/o\n");
        exit (EXIT_FAILURE);
    }

    memset(&cropcap, 0, sizeof(cropcap));

    cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if (0 == ioctl (fd, VIDIOC_CROPCAP, &cropcap)) {
        crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        crop.c = cropcap.defrect; /* reset to default */

        if (-1 == ioctl (fd, VIDIOC_S_CROP, &crop)) {
            switch (errno) {
            case EINVAL:
                /* Cropping not supported. */
                break;
            default:
                /* Errors ignored. */
                break;
            }
        }
    } else {
        /* Errors ignored. */
    }

    memset(&fmt, 0, sizeof(fmt));

    fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width       = this->width;
    fmt.fmt.pix.height      = this->height;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;//V4L2_PIX_FMT_YUYV;//V4L2_PIX_FMT_YUV420
    fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;

    if (-1 == ioctl (fd, VIDIOC_S_FMT, &fmt)) {
        perror ("VIDIOC_S_FMT");
        exit(EXIT_FAILURE);
    }

    /* Note VIDIOC_S_FMT may change width and height. */

    /* Buggy driver paranoia. */
    min = fmt.fmt.pix.width * 2;
    if (fmt.fmt.pix.bytesperline < min)
        fmt.fmt.pix.bytesperline = min;

    min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;

    if (fmt.fmt.pix.sizeimage < min)
        fmt.fmt.pix.sizeimage = min;

    printf("%d %d\n", fmt.fmt.pix.width, fmt.fmt.pix.height);
    printf("%d\n",fmt.fmt.pix.sizeimage);

    // Init mmap
    struct v4l2_requestbuffers req;

    memset(&req, 0, sizeof(req));

    req.count               = 2;
    req.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory              = V4L2_MEMORY_MMAP;

    if (-1 == ioctl (fd, VIDIOC_REQBUFS, &req)) {
        if (EINVAL == errno) {
            fprintf (stderr, "Device does not support memory mapping\n");
            exit (EXIT_FAILURE);
        } else {
            perror ("VIDIOC_REQBUFS");
            exit(EXIT_FAILURE);
        }
    }

    if (req.count < 2) {
        fprintf (stderr, "Insufficient buffer memory on device\n");
        exit (EXIT_FAILURE);
    }

    buffers = (struct buffer*)calloc (req.count, sizeof (*buffers));

    if (!buffers) {
        fprintf (stderr, "Out of memory\n");
        exit (EXIT_FAILURE);
    }

    for (n_buffers = 0; n_buffers < req.count; ++n_buffers) {
        struct v4l2_buffer buf;

        memset(&buf, 0, sizeof(buf));

        buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory      = V4L2_MEMORY_MMAP;
        buf.index       = n_buffers;

        if (-1 == ioctl (fd, VIDIOC_QUERYBUF, &buf)) {
            perror ("VIDIOC_QUERYBUF");
            exit(EXIT_FAILURE);
        }

        buffers[n_buffers].length = buf.length;
        buffers[n_buffers].start =
            mmap (NULL /* start anywhere */,
                  buf.length,
                  PROT_READ | PROT_WRITE /* required */,
                  MAP_SHARED /* recommended */,
                  fd, buf.m.offset);

        if (MAP_FAILED == buffers[n_buffers].start) {
            perror ("mmap");
            exit(EXIT_FAILURE);
        }
    }
}

void Camera::get_info()
{
    struct v4l2_input input;
    int index;

    if (-1 == ioctl (fd, VIDIOC_G_INPUT, &index)) {
        perror ("VIDIOC_G_INPUT");
        exit (EXIT_FAILURE);
    }

    memset (&input, 0, sizeof (input));
    input.index = index;

    if (-1 == ioctl (fd, VIDIOC_ENUMINPUT, &input)) {
        perror ("VIDIOC_ENUMINPUT");
        exit (EXIT_FAILURE);
    }

    printf ("Current input: %s\n", input.name);
}

void Camera::get_video_info() {
    struct v4l2_input input;
    struct v4l2_fmtdesc formats;

    memset (&input, 0, sizeof (input));

    if (-1 == ioctl (fd, VIDIOC_G_INPUT, &input.index)) {
        perror ("VIDIOC_G_INPUT");
        exit (EXIT_FAILURE);
    }

    printf ("Current input %s supports:\n", input.name);

    memset (&formats, 0, sizeof (formats));
    formats.index = 0;
    formats.type  = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    while (0 == ioctl (fd, VIDIOC_ENUM_FMT, &formats)) {
        printf ("%s\n", formats.description);
        formats.index++;
    }

    if (errno != EINVAL || formats.index == 0) {
        perror ("VIDIOC_ENUMFMT");
        exit(EXIT_FAILURE);
    }
}

unsigned char* Camera::read_frame ()
{
    fd_set fds;
    struct timeval tv;
    int r;

    FD_ZERO (&fds);
    FD_SET (fd, &fds);

    /* Timeout. */
    tv.tv_sec = 2;
    tv.tv_usec = 0;

    r = select (fd + 1, &fds, NULL, NULL, &tv);

    if (-1 == r) {
        if (EINTR == errno)
            return NULL;
        perror ("select");
    }

    if (0 == r) {
        fprintf (stderr, "select timeout\n");
        return NULL;
    }

    unsigned int i;


    memset(&buf, 0, sizeof(buf));

    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;

    if (-1 == ioctl (fd, VIDIOC_DQBUF, &buf)) {
        switch (errno) {
        case EAGAIN:
            return 0;

        case EIO:

        default:
            perror ("VIDIOC_DQBUF");
            exit(EXIT_FAILURE);
        }
    }

    assert (buf.index < n_buffers);

    //printf ("%d %d: ", buf.index, buf.bytesused);

    if (-1 == ioctl (fd, VIDIOC_QBUF, &buf)) {
        perror ("VIDIOC_QBUF");
        exit(EXIT_FAILURE);
    }

    unsigned char* ptr = (unsigned char*)buffers[buf.index].start;

    return ptr;
}

