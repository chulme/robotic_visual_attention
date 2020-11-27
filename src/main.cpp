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

//Yarp ports
Network network;
BufferedPort<ImageOf<PixelRgb>> imagePort, colourPort, edgePort, facePort, circlePort;

//Head
Property options;
PolyDriver robotHead;
IPositionControl *pos;
IVelocityControl *vel;
IEncoders *enc;
IControlMode *con;
int jnts = 0;
Vector setpoints;

//Right Arm
Property armOptions;
PolyDriver robotRightArm;
IPositionControl *aPos;
IVelocityControl *aVel;
IEncoders *aEnc;
IControlMode *aCon;
int armJnts = 0;
Vector setArmPoints;

//Left Arm
Property leftArmOptions;
PolyDriver robotLeftArm;
IPositionControl *bPos;
IVelocityControl *bVel;
IEncoders *bEnc;
IControlMode *bCon;
int leftArmJnts = 0;
Vector setLeftArmPoints;

std::vector<cv::Point> defaultCoord;

void init_head_joints();
void init_right_arm_joints();
void init_left_arm_joints();
void init_ports();
void tracking_state_machine(std::vector<cv::Point> faceCoords, std::vector<cv::Point> circleCoords);

int main()
{
    init_head_joints();
    init_left_arm_joints();
    init_right_arm_joints();
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
        ImageOf<PixelRgb> &circles = circlePort.prepare();

        //Apply visual modules
        cv::Mat colour = colour_threshold(opencvImage, clr);
        cv::Mat canny = canny_threshold(opencvImage, edge);
        std::vector<cv::Point> faceCoords = facial_detection(opencvImage, faces);
        std::vector<cv::Point> circleCoords = circle_detection(opencvImage, circles);

        tracking_state_machine(faceCoords, circleCoords);

        //Write to ports, so images can be viewed by yarpview
        colourPort.write();
        edgePort.write();
        facePort.write();
        circlePort.write();
    }

    return 0;
}

void tracking_state_machine(std::vector<cv::Point> faceCoords, std::vector<cv::Point> circleCoords)
{
    if (faceCoords.size() > 0)
    {
        yInfo() << "Face detected, moving to" << faceCoords.front().x << "," << faceCoords.front().y << "and waving.";
        wave(setArmPoints, aPos);
        toward_head(faceCoords, jnts, setpoints, pos);
    }
    else if (circleCoords.size() > 0)
    {
        yInfo() << "Circle detected, moving to" << circleCoords.front().x << "," << circleCoords.front().y;
        salute(setLeftArmPoints, bPos);
        toward_head(circleCoords, jnts, setpoints, pos);
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
    options.put("local", "/head");
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
}

void init_right_arm_joints()
{
    armOptions.put("device", "remote_controlboard");
    armOptions.put("local", "/rightArm");
    armOptions.put("remote", "/icubSim/right_arm");
    robotRightArm.open(armOptions);
    if (!robotRightArm.isValid())
    {
        printf("Cannot connect to robot arm\n");
    }
    robotRightArm.view(aPos);
    robotRightArm.view(aVel);
    robotRightArm.view(aEnc);
    robotRightArm.view(aCon);
    if (aPos == NULL || aVel == NULL || aEnc == NULL || aCon == NULL)
    {
        printf("Cannot get interface to robot arm\n");
        robotRightArm.close();
    }
    aPos->getAxes(&armJnts);
    setArmPoints.resize(armJnts);
    for (int i = 0; i <= armJnts; i++)
        aCon->setControlMode(i, VOCAB_CM_POSITION);
    aPos->positionMove(setArmPoints.data());
}

void init_left_arm_joints()
{
    leftArmOptions.put("device", "remote_controlboard");
    leftArmOptions.put("local", "/leftArm");
    leftArmOptions.put("remote", "/icubSim/left_arm");
    robotLeftArm.open(leftArmOptions);
    if (!robotLeftArm.isValid())
    {
        printf("Cannot connect to robot left arm\n");
    }
    robotLeftArm.view(bPos);
    robotLeftArm.view(bVel);
    robotLeftArm.view(bEnc);
    robotLeftArm.view(bCon);
    if (bPos == NULL || bVel == NULL || bEnc == NULL || bCon == NULL)
    {
        printf("Cannot get interface to robot left arm\n");
        robotLeftArm.close();
    }
    bPos->getAxes(&leftArmJnts);
    setLeftArmPoints.resize(leftArmJnts);
    for (int i = 0; i <= leftArmJnts; i++)
        bCon->setControlMode(i, VOCAB_CM_POSITION);
    bPos->positionMove(setLeftArmPoints.data());
}

void init_ports()
{
    imagePort.open("/cameraListener"); //yarp connect /icubSim/cam/left /cameraListener
    colourPort.open("/filters/colour");
    edgePort.open("/filters/edge");
    facePort.open("/faces");
    circlePort.open("/circles");
}
