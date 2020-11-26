
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

void toward_head(const std::vector<cv::Point> focus, int jnts, Vector setpoints, IVelocityControl *vel)
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

        vel->velocityMove(setpoints.data());
        yarp::os::Time::delay(1);
    }
}
