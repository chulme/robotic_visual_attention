#include <read_camera_stream.h>
#include <yarp/sig/Image.h>
#include <linear_filters.h>
#include <facial_and_object_detection.h>

using namespace yarp::sig;

int main()
{
    //ImageOf<PixelRgb> *image = read_camera_stream();
    apply_and_display_filtered_images();
    cv::Mat image = read_image("man.jpeg");
    facial_detection(image);

    return 0;
}