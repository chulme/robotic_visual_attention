#include <read_camera_stream.h>
#include <opencv2/highgui/highgui.hpp>

#include <yarp/sig/Image.h>
#include <linear_filters.h>
#include <facial_and_object_detection.h>
#include <yarp/os/LogStream.h>

using namespace yarp::sig;

int main()
{
    ImageOf<PixelRgb> yarpImage = read_camera_stream();
    cv::Mat opencvImage = convert_yarp_to_opencv_image(yarpImage);
    apply_and_display_filtered_images(opencvImage);
    facial_detection(opencvImage);

    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}