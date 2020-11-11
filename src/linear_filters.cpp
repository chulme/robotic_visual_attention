#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <yarp/sig/Image.h>
#include <yarp/os/LogStream.h>

#include <linear_filters.h>

using namespace yarp::os;
using namespace yarp::sig;

void apply_and_display_filtered_images()
{
    cv::Mat image = read_image();
    cv::Mat clr_threshold = colour_threshold(image);

    cv::imshow("Original", image);
    cv::imshow("Colour Threshold", clr_threshold);
    cv::waitKey(0);
    cv::destroyAllWindows();
}

static void convert_image_to_cv_image()
{
}

//Used for debugging until converting image from yarp to OpenCV resolved.
static cv::Mat read_image()
{
    yInfo() << "Reading image";

    std::string image_path = "/home/user/Desktop/bulgaria.jpg";
    cv::Mat image = cv::imread(image_path, cv::IMREAD_COLOR);
    if (image.empty())
    {
        yInfo() << "Could not read image.";
    }

    return image;
}

static cv::Mat colour_threshold(cv::Mat image)
{
    cv::Mat mask;
    //Threshold is BGR, not RGB
    cv::inRange(image, cv::Scalar(0, 0, 100), cv::Scalar(80, 80, 225), mask);

    cv::Mat mask_rgb;
    cv::cvtColor(mask, mask_rgb, cv::COLOR_GRAY2RGB);

    return (image & mask_rgb);
}