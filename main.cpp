#include <read_camera_stream.h>
#include <opencv2/highgui/highgui.hpp>

#include <aruco/aruco.h>
#include <aruco/cvdrawingutils.h>
#include <aruco/markerdetector.h>
#include <opencv2/flann/flann.hpp>
#include <opencv2/core/core.hpp>

#include <yarp/sig/Image.h>
#include <linear_filters.h>
#include <facial_and_object_detection.h>
#include <aruco_detection.h>
#include <yarp/os/LogStream.h>
#include <yarp/os/Network.h>
#include <yarp/os/BufferedPort.h>
#include <unistd.h>
#include <iostream>
#include <utils.h>
using namespace yarp::sig;
using namespace yarp::os;
using namespace std;
using namespace cv;
using namespace aruco;


int main()
{
    
    Network network; //inits yarp ports
    BufferedPort<ImageOf<PixelRgb>> imagePort,arucoPort;
    imagePort.open("/cameraListener"); //connect to camera using
                                       //yarp connect /icubSim/cam/left /cameraListener 
    arucoPort.open("/marker");
    yInfo() << "Press any key on the windows to close the program cleanly.";
    bool buttonPressed = false;

	
   
	
    while (!buttonPressed)
    {
	ImageOf<PixelRgb> &markerImg = arucoPort.prepare();

        ImageOf<PixelRgb> yarpImage = read_port_until_image_received(imagePort);
	//cv::Mat opencvImage = read_image("marker.png");        
	cv::Mat opencvImage = convert_yarp_to_opencv_image(yarpImage);

	cv::Mat arucoImg = arucoDetection(opencvImage, markerImg);

	arucoPort.write();
        if (cv::waitKey(1) >= 0)
        {
            buttonPressed = true;
        }
    }
    imagePort.close();
    cv::destroyAllWindows();

    return 0;
}
