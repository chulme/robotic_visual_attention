#pragma once

#include <opencv2/core/core.hpp>

void apply_and_display_filtered_images();

static cv::Mat colour_threshold(const cv::Mat &image);
cv::Mat read_image(const std::string &image_name);
static cv::Mat canny_threshold(const cv::Mat &image);