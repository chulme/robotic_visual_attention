#include <read_camera_stream.h>
#include <opencv2/highgui/highgui.hpp>
#include <aruco/aruco.h>
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


void arucos (){

	//print(aruco::Dictionary::DICT_TYPES);
/*
	cv::Mat markerImage;
	aruco::Dictionary dictionary = aruco::getPredefinedDictionary("ARUCO_MIP_36h12"​);

	// 
	for(int i  =0; i <50; i++){
		aruco::drawMarker(dictionary, i, 500, markerImage, 1);
		ostringstream convert;
		string image = "4x4Marker_";
		convert << image << i << ".jpeg";
		imwrite(convert.str(),markerImage);
	}
	*/
}

int main()
{
    arucos();
    Network network; //inits yarp ports
    BufferedPort<ImageOf<PixelRgb>> imagePort;
    imagePort.open("/cameraListener"); //connect to camera using
                                       //yarp connect /icubSim/cam/left /cameraListener
    yInfo() << "Press any key on the windows to close the program cleanly.";
    bool buttonPressed = false;
    while (!buttonPressed)
    {
        //ImageOf<PixelRgb> yarpImage = read_port_until_image_received(imagePort);
	cv::Mat opencvImage = read_image("marker_66.jpg");        
	//cv::Mat opencvImage = convert_yarp_to_opencv_image(yarpImage);
        apply_and_display_filtered_images(opencvImage);
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
