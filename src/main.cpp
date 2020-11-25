#include <read_camera_stream.h>
#include <opencv2/highgui/highgui.hpp>

#include <yarp/sig/Image.h>
#include <linear_filters.h>
#include <facial_and_object_detection.h>
#include <yarp/os/LogStream.h>
#include <yarp/os/Network.h>
#include <yarp/os/BufferedPort.h>
#include <unistd.h>
using namespace yarp::sig;
using namespace yarp::os;

int main()
{
    Network network; //inits yarp ports
    BufferedPort<ImageOf<PixelRgb>> imagePort;
    imagePort.open("/cameraListener"); //connect to camera using
                                       //yarp connect /icubSim/cam/left /cameraListener
    yInfo() << "Press any key on the windows to close the program cleanly.";
    bool buttonPressed = false;
    while (!buttonPressed)
    {
        ImageOf<PixelRgb> yarpImage = read_port_until_image_received(imagePort);
        cv::Mat opencvImage = convert_yarp_to_opencv_image(yarpImage);
        apply_and_display_filtered_images(opencvImage);
        circle_detection(opencvImage);
        facial_detection(opencvImage);
        if (cv::waitKey(1) >= 0)
        {
            buttonPressed = true;
        }
    }
    imagePort.close();
    cv::destroyAllWindows();

    return 0;
}