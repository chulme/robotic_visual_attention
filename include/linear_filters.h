#pragma once

#include <opencv2/core/core.hpp>
#include <yarp/sig/Image.h>
#include <yarp/os/Network.h>
#include <yarp/os/BufferedPort.h>

cv::Mat convert_yarp_to_opencv_image(yarp::sig::ImageOf<yarp::sig::PixelRgb> &yarpImage);
cv::Mat colour_threshold(const cv::Mat &image, yarp::sig::ImageOf<yarp::sig::PixelRgb> &out);
cv::Mat read_image(const std::string &image_name);
cv::Mat canny_threshold(const cv::Mat &image, yarp::sig::ImageOf<yarp::sig::PixelRgb> &out);