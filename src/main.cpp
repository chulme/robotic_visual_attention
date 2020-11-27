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

#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/ImplementPositionControl.h>
#include <yarp/dev/ImplementVelocityControl.h>
#include <yarp/dev/ImplementControlMode.h>
#include <yarp/dev/ImplementEncoders.h>

using namespace yarp::sig;
using namespace yarp::os;
using namespace yarp::dev;

Network network;
BufferedPort<ImageOf<PixelRgb>> imagePort, colourPort, edgePort, facePort;
Property options;
PolyDriver robotHead;
IPositionControl *pos;
IVelocityControl *vel;
IEncoders *enc;
IControlMode *con;
int jnts = 0;
Vector setpoints;
std::vector<cv::Point> defaultCoord;

void init_head_joints();
void init_ports();

int main()
{
    init_head_joints();
    init_ports();
    defaultCoord.push_back(cv::Point(160, 120));

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

        tracking_state_machine(faceCoords);

        //Write to ports, so images can be viewed by yarpview
        colourPort.write();
        edgePort.write();
        facePort.write();
    }

    return 0;
}

void tracking_state_machine(std::vector<cv::Point> faceCoords)
{
    if (faceCoords.size() > 0)
    {
        yInfo() << "Face detected, moving to track.";
        toward_head(faceCoords, jnts, setpoints, pos);
    }
    else
    {
        toward_head(defaultCoord, jnts, setpoints, pos);
        yInfo() << "Nothing detected, moving to rest position.";
    }
}

void init_head_joints()
{
    options.put("device", "remote_controlboard");
    options.put("local", "/tutorial/motor/client");
    options.put("remote", "/icubSim/head");
    robotHead.open(options);
    if (!robotHead.isValid())
    {
        printf("Cannot connect to robot head\n");
        return;
    }
    robotHead.view(pos);
    robotHead.view(vel);
    robotHead.view(enc);
    robotHead.view(con);
    if (pos == NULL || vel == NULL || enc == NULL || con == NULL)
    {
        printf("Cannot get interface to robot head\n");
        robotHead.close();
        return;
    }

    pos->getAxes(&jnts);
    setpoints.resize(jnts);
    for (int i = 0; i <= jnts; i++)
        con->setControlMode(i, VOCAB_CM_POSITION);
    pos->positionMove(setpoints.data());
    vel->velocityMove(setpoints.data());
}

void init_ports()
{
    imagePort.open("/cameraListener"); //yarp connect /icubSim/cam/left /cameraListener
    colourPort.open("/filters/colour");
    edgePort.open("/filters/edge");
    facePort.open("/faces");
}
