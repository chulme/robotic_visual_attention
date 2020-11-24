#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

#include <yarp/cv/Cv.h>

#include <yarp/sig/Image.h>

#include <yarp/os/LogStream.h>
#include <yarp/os/Network.h>
#include <yarp/os/BufferedPort.h>

#include <linear_filters.h>

#include <utils.h>
#include <string>

using namespace yarp::os;
using namespace yarp::sig;

cv::Mat convert_yarp_to_opencv_image(ImageOf<PixelRgb> &yarpImage)
{
    return yarp::cv::toCvMat(yarpImage);
}

cv::Mat colour_threshold(const cv::Mat &image, ImageOf<PixelRgb> &out)
{
    cv::Mat mask;
    //Threshold is BGR, not RGB
    cv::inRange(image, cv::Scalar(100, 0, 0), cv::Scalar(255, 80, 80), mask);

    cv::Mat mask_rgb;
    cv::cvtColor(mask, mask_rgb, cv::COLOR_GRAY2RGB);
    cv::Mat colour_threshold = (image & mask_rgb);
    out = yarp::cv::fromCvMat<PixelRgb>(colour_threshold);

    return colour_threshold;
}

cv::Mat canny_threshold(const cv::Mat &image, ImageOf<PixelRgb> &out)
{
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_RGB2GRAY);

    cv::Mat blurred;
    cv::blur(gray, blurred, cv::Size(3, 3));
    cv::Mat edges;
    cv::Canny(blurred, edges, 100, 300, 3);
    cv::Mat dst;
    dst.setTo(cv::Scalar::all(0));
    image.copyTo(dst, edges);
    out = yarp::cv::fromCvMat<PixelRgb>(dst);
    return dst;
}