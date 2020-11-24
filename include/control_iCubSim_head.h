#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <yarp/dev/IVelocityControl.h>
#include <yarp/sig/Vector.h>
#include <vector>

void toward_head(const std::vector<cv::Point> focus, int jnts, yarp::sig::Vector setpoints, yarp::dev::IVelocityControl *vel);
