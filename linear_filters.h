#pragma once

#include <opencv2/core/core.hpp>
#include <yarp/sig/Image.h>

void apply_and_display_filtered_images(const cv::Mat &image);
cv::Mat convert_yarp_to_opencv_image(yarp::sig::ImageOf<yarp::sig::PixelRgb> &yarpImage);
static cv::Mat colour_threshold(const cv::Mat &image);
cv::Mat read_image(const std::string &image_name);
static cv::Mat canny_threshold(const cv::Mat &image);