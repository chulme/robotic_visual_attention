
#pragma once

#include <yarp/sig/Image.h>
#include <yarp/os/BufferedPort.h>

yarp::sig::ImageOf<yarp::sig::PixelRgb> *read_camera_stream();
static yarp::sig::ImageOf<yarp::sig::PixelRgb> *read_port_until_image_received(
    yarp::os::BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelRgb>> &imagePort);
