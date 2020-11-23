#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

//#include <yarp/cv/Cv.h>
#include <yarp/sig/Image.h>
#include <yarp/os/LogStream.h>

#include <linear_filters.h>
#include <utils.h>
#include <string>

using namespace yarp::os;
using namespace yarp::sig;

//from opencvExampleNoYarp as a team member is using x2go

::cv::Mat toCvMat(yarp::sig::ImageOf<yarp::sig::PixelRgb>& yarpImage)
{
    ::cv::Mat outMat (yarpImage.height(), yarpImage.width(), CV_8UC3,
                      yarpImage.getRawImage(), yarpImage.getRowSize()); // RVO
    ::cv::cvtColor(outMat, outMat, CV_BGR2RGB);

    return outMat;
}


void apply_and_display_filtered_images(const cv::Mat &image)
{
   // cv::Mat image = read_image("house.jpg");
    cv::Mat clr_threshold = colour_threshold(image);
    cv::Mat edge_threshold = canny_threshold(image);

    cv::imshow("Colour Threshold", clr_threshold);
    cv::imshow("Canny Edge Detection", edge_threshold);
    cv::imshow("Original", image);
}

cv::Mat convert_yarp_to_opencv_image(ImageOf<PixelRgb> &yarpImage)
{
    return toCvMat(yarpImage);
	
}

static cv::Mat colour_threshold(const cv::Mat &image)
{
    cv::Mat mask;
    //Threshold is BGR, not RGB
    cv::inRange(image, cv::Scalar(100, 0, 0), cv::Scalar(255, 80, 80), mask);

    cv::Mat mask_rgb;
    cv::cvtColor(mask, mask_rgb, cv::COLOR_GRAY2RGB);

    return (image & mask_rgb);
}

static cv::Mat canny_threshold(const cv::Mat &image)
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
    return dst;
}
