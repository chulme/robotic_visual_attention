#include <string>
#include <limits.h>
#include <yarp/os/Os.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <yarp/os/LogStream.h>

#include <utils.h>
std::string get_project_path()
{
    char buffer[PATH_MAX];
    yarp::os::getcwd(buffer, sizeof(buffer));
    std::string path = buffer;
    int id = path.find("/robotic_visual_attention/");
    path.erase(id);
    path += "/robotic_visual_attention/";
    return path;
}

//Used for debugging until converting image from yarp to OpenCV resolved.
cv::Mat read_image(const std::string &image_name)
{
    std::string path = get_project_path();
    path += "data/" + image_name;
    yInfo()
        << "Reading image from"
        << path;

    std::string image_path = path;
    cv::Mat image = cv::imread(image_path, cv::IMREAD_COLOR);
    if (image.empty())
    {
        yInfo() << "Could not read image.";
    }

    return image;
}