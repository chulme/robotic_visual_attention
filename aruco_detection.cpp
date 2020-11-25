#include <read_camera_stream.h>
#include <opencv2/highgui/highgui.hpp>

#include <aruco/aruco.h>
#include <aruco/cvdrawingutils.h>
#include <aruco/markerdetector.h>
#include <opencv2/flann/flann.hpp>
#include <opencv2/core/core.hpp>



#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <yarp/sig/Image.h>
#include <yarp/sig/ImageDraw.h>
#include <yarp/sig/ImageFile.h>

#include <yarp/sig/Image.h>
#include <linear_filters.h>
#include <facial_and_object_detection.h>
#include <yarp/os/LogStream.h>
#include <yarp/os/Network.h>
#include <yarp/os/BufferedPort.h>
#include <unistd.h>
#include <iostream>
#include <utils.h>
#include <aruco_detection.h>

using namespace yarp::sig;
using namespace yarp::os;
using namespace std;
using namespace aruco;

//from x2go help email alternitive to yarp::cv::fromCvMat<PixelRgb>
yarp::sig::ImageOf<yarp::sig::PixelRgb> fromCvMat(::cv::Mat& cvImage)
{
    constexpr size_t align_8_bytes = 8;
    constexpr size_t align_4_bytes = 4;

    yarp::sig::ImageOf<yarp::sig::PixelRgb> outImg;
    // Checking cv::Mat::type() compatibility with the T PixelType
    assert(CV_8UC3 == cvImage.type());
    ::cv::cvtColor(cvImage, cvImage, CV_BGR2RGB);

    // Check the cv::Mat alignment
    if (cvImage.step % align_8_bytes == 0) {
        outImg.setQuantum(align_8_bytes);
    }
    else if (cvImage.step % align_4_bytes == 0) {
        outImg.setQuantum(align_4_bytes);
    }
    outImg.setExternal(cvImage.data, cvImage.cols, cvImage.rows);
    return outImg;
}

cv::Mat arucoDetection(const cv::Mat &image, ImageOf<PixelRgb> &output){


	aruco::MarkerDetector detector;
	//read in
	cv::Mat markerImg = image;
	detector.setDetectionMode(aruco::DM_VIDEO_FAST);
	detector.setDictionary("ARUCO_MIP_36h12");
	
	for(auto m:detector.detect(markerImg)){
		std::cout<<m<<std::endl;
		m.draw(markerImg);	
	} 
	//cv::imshow("Aruco_Detection",InImage);
	//output = yarp::cv::fromCvMat<PixelRgb>(markerImg);
	yarp::sig::ImageOf<yarp::sig::PixelRgb> out=fromCvMat(markerImg);
	output = out;
	//yarpReturnImage=yarp::cv::fromCvMat<yarp::sig::PixelRgb>(cvImage2);
	


return markerImg;

}
