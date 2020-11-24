#include <read_camera_stream.h>
#include <opencv2/highgui/highgui.hpp>

#include <yarp/sig/Image.h>
#include <linear_filters.h>
#include <facial_and_object_detection.h>
#include <yarp/os/LogStream.h>
#include <yarp/os/Network.h>
#include <yarp/cv/Cv.h>

#include <yarp/os/BufferedPort.h>
#include <unistd.h>
using namespace yarp::sig;
using namespace yarp::os;

int main()
{
    Network network; //inits yarp ports

    BufferedPort<ImageOf<PixelRgb>> imagePort, colourPort, edgePort, facePort;
    imagePort.open("/cameraListener"); //yarp connect /icubSim/cam/left /cameraListener
    colourPort.open("/filters/colour");
    edgePort.open("/filters/edge");
    facePort.open("/faces");

    while (1)
    {
        // Read webcam
        ImageOf<PixelRgb> yarpImage = read_port_until_image_received(imagePort);
        cv::Mat opencvImage = convert_yarp_to_opencv_image(yarpImage);

        //Prepare ports
        ImageOf<PixelRgb> &clr = colourPort.prepare();
        ImageOf<PixelRgb> &edge = edgePort.prepare();
        ImageOf<PixelRgb> &faces = facePort.prepare();

        //Apply visual modules
        cv::Mat colour = colour_threshold(opencvImage, clr);
        cv::Mat canny = canny_threshold(opencvImage, edge);
        std::vector<cv::Point> faceCoords = facial_detection(opencvImage, faces);

        //Write to ports, so images can be viewed by yarpview
        colourPort.write();
        edgePort.write();
        facePort.write();
    }

    return 0;
}