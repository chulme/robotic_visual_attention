#include <read_camera_stream.h>
#include <opencv2/highgui/highgui.hpp>

#include <yarp/sig/Image.h>
#include <linear_filters.h>
#include <facial_and_object_detection.h>
#include <yarp/os/LogStream.h>
#include <yarp/os/Network.h>
#include <yarp/cv/Cv.h>
#include <control_iCubSim_head.h>
#include <yarp/os/BufferedPort.h>
#include <unistd.h>

#include <yarp/dev/all.h>

using namespace yarp::sig;
using namespace yarp::os;
using namespace yarp::dev;

int main()
{
    Network yarp; // set up yarp

    Property options;
    options.put("device", "remote_controlboard");
    options.put("local", "/tutorial/motor/client");
    options.put("remote", "/icubSim/head");
    PolyDriver robotHead(options);
    if (!robotHead.isValid())
    {
        printf("Cannot connect to robot head\n");
        return 1;
    }
    IPositionControl *pos;
    IVelocityControl *vel;
    IEncoders *enc;
    IControlMode *con;
    robotHead.view(pos);
    robotHead.view(vel);
    robotHead.view(enc);
    robotHead.view(con);
    if (pos == NULL || vel == NULL || enc == NULL || con == NULL)
    {
        printf("Cannot get interface to robot head\n");
        robotHead.close();
        return 1;
    }

    int jnts = 0;
    pos->getAxes(&jnts);
    Vector setpoints;
    setpoints.resize(jnts);
    for (int i = 0; i <= jnts; i++)
        con->setControlMode(i, VOCAB_CM_VELOCITY);
    vel->velocityMove(setpoints.data());

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
        yInfo() << "hi";

        //Prepare ports
        ImageOf<PixelRgb> &clr = colourPort.prepare();
        ImageOf<PixelRgb> &edge = edgePort.prepare();
        ImageOf<PixelRgb> &faces = facePort.prepare();

        //Apply visual modules
        cv::Mat colour = colour_threshold(opencvImage, clr);
        cv::Mat canny = canny_threshold(opencvImage, edge);
        std::vector<cv::Point> faceCoords = facial_detection(opencvImage, faces);
        for (auto faceCoord : faceCoords)
        {
            yInfo() << faceCoord.x << ", " << faceCoord.y;
        }
        toward_head(faceCoords, jnts, setpoints, vel);
        //Write to ports, so images can be viewed by yarpview
        colourPort.write();
        edgePort.write();
        facePort.write();
    }

    return 0;
}