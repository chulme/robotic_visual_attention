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



void aruco_detection(const cv::Mat &image){


	aruco::MarkerDetector MDetector;
	//read in
	cv::Mat InImage = image;
	MDetector.setDictionary("ARUCO_MIP_36h12");
	
	for(auto m:MDetector.detect(InImage)){
		std::cout<<m<<std::endl;
		m.draw(InImage);	
	} 
	cv::imshow("hi",InImage);







}

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
	//cv::Mat opencvImage = read_image("marker.png");        
	cv::Mat opencvImage = convert_yarp_to_opencv_image(yarpImage);
        apply_and_display_filtered_images(opencvImage);
	//
        facial_detection(opencvImage);
	aruco_detection(opencvImage);

        if (cv::waitKey(1) >= 0)
        {
            buttonPressed = true;
        }
    }
    imagePort.close();
    cv::destroyAllWindows();

    return 0;
}
