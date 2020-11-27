
#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/ImplementPositionControl.h>
#include <yarp/dev/ImplementVelocityControl.h>
#include <yarp/dev/ImplementControlMode.h>
#include <yarp/dev/ImplementEncoders.h>
#include <yarp/sig/Vector.h>

#include <vector>
#include <control_iCubSim_head.h>
const double scale = 1.0;

using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::dev;

void toward_head(const std::vector<cv::Point> focus, int jnts, Vector setpoints, IPositionControl *pos)
{

    for (cv::Point target : focus)
    {
        double x = target.x;
        double y = target.y;
        x -= 320 / 2;
        y -= 240 / 2;
        double vx = x * 0.1;
        double vy = -y * 0.1;

        for (int i = 0; i < jnts; i++)
        {
            setpoints[i] = 0;
        }

        setpoints[3] = vy;
        setpoints[4] = vx;

        pos->positionMove(setpoints.data());
        yarp::os::Time::delay(1);
    }
}

void wave(Vector setpoints, IPositionControl *pos)
{
    setpoints[0] = -69;
    setpoints[1] = 61;
    setpoints[2] = 0;
    setpoints[3] = 49;
    setpoints[4] = 0;
    setpoints[5] = 0;
    setpoints[6] = 0;
    setpoints[7] = 59;
    setpoints[8] = 20;
    setpoints[9] = 20;
    setpoints[10] = 20;
    setpoints[11] = 10;
    setpoints[12] = 11;
    setpoints[13] = 10;
    setpoints[14] = 11;
    setpoints[15] = 11;
    pos->positionMove(setpoints.data());
    yarp::os::Time::delay(0.5);

    setpoints[3] = 82;

    pos->positionMove(setpoints.data());
    yarp::os::Time::delay(0.5);

    setpoints[3] = 28;
    pos->positionMove(setpoints.data());
    yarp::os::Time::delay(0.5);

    setpoints[0] = -25;
    setpoints[1] = 19;
    setpoints[2] = 0;
    setpoints[3] = 50;
    pos->positionMove(setpoints.data());

    yarp::os::Time::delay(0.01);
}

void salute(Vector setpoints, IPositionControl* pos)
{
    setpoints[0] = -94;
    setpoints[1] = 56;
    setpoints[2] = 60;
    setpoints[3] = 103;
    setpoints[4] = 0;
    setpoints[5] = 0;
    setpoints[6] = 0;
    setpoints[7] = 59;
    setpoints[8] = 20;
    setpoints[9] = 20;
    setpoints[10] = 20;
    setpoints[11] = 10;
    setpoints[12] = 11;
    setpoints[13] = 10;
    setpoints[14] = 11;
    setpoints[15] = 11;
    pos->positionMove(setpoints.data());
    yarp::os::Time::delay(2.0);

    setpoints[3] = 16;

    pos->positionMove(setpoints.data());
    
    yarp::os::Time::delay(2.0);

    setpoints[0] = -25;
    setpoints[1] = 19;
    setpoints[2] = 0;
    setpoints[3] = 50;
    pos->positionMove(setpoints.data());

    yarp::os::Time::delay(0.01);
}
