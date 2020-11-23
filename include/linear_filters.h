#pragma once

#include <opencv2/core/core.hpp>
#include <yarp/sig/Image.h>
#include <yarp/os/Network.h>
#include <yarp/os/BufferedPort.h>

void apply_and_display_filtered_images(const cv::Mat &image, yarp::sig::ImageOf<yarp::sig::PixelRgb> &colour_threshold_out, yarp::sig::ImageOf<yarp::sig::PixelRgb> &canny_threshold_out);
cv::Mat convert_yarp_to_opencv_image(yarp::sig::ImageOf<yarp::sig::PixelRgb> &yarpImage);
static yarp::sig::ImageOf<yarp::sig::PixelRgb> colour_threshold(const cv::Mat &image);
cv::Mat read_image(const std::string &image_name);
static yarp::sig::ImageOf<yarp::sig::PixelRgb> canny_threshold(const cv::Mat &image);