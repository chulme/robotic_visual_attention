#pragma once
#include <read_camera_stream.h>
#include <opencv2/highgui/highgui.hpp>

#include <aruco/aruco.h>
#include <aruco/cvdrawingutils.h>
#include <aruco/markerdetector.h>
#include <opencv2/flann/flann.hpp>
#include <opencv2/core/core.hpp>

#include <yarp/sig/Image.h>
#include <linear_filters.h>
#include <facial_and_object_detection.h>

#include <yarp/sig/Image.h>


#include <yarp/os/LogStream.h>
#include <yarp/os/Network.h>
#include <yarp/os/BufferedPort.h>
#include <unistd.h>
#include <iostream>
#include <utils.h>

cv::Mat arucoDetection(const cv::Mat &image, yarp::sig::ImageOf<yarp::sig::PixelRgb> &output);
