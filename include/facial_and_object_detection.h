#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <vector>

void facial_detection(const cv::Mat &image);
static void loadCascade(cv::CascadeClassifier &cascade);
static std::vector<cv::Rect> detectFaces(const cv::Mat &image, cv::CascadeClassifier &faceCascade);
static cv::Mat drawRectangeOnFaces(const std::vector<cv::Rect> faces, const cv::Mat &image);
cv::Mat circle_detection(const cv::Mat &src);