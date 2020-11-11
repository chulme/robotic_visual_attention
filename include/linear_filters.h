#pragma once

#include <opencv2/opencv.hpp>

void apply_and_display_filtered_images();

static cv::Mat colour_threshold(cv::Mat image);
static cv::Mat read_image();
static cv::Mat canny_threshold(cv::Mat image);