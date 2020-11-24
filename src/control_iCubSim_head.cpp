#include <yarp/os/all.h>
#include <yarp/sig/all.h>
#include <yarp/dev/all.h>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#include <vector>
#include <string>
#include <utils.h>
#include <control_iCubSim_head.h>
const double scale = 1.0;

using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::dev;

void toward_head(const std::vector<cv::Point> focus)
{
	Property options;
   options.put("device", "remote_controlboard");
   options.put("local", "/tutorial/motor/client");
   options.put("remote", "/icubSim/head");
   PolyDriver robotHead(options);
   	
   IPositionControl *pos;
   IVelocityControl *vel;
   IEncoders *enc;
   IControlMode *con;
   robotHead.view(pos);
   robotHead.view(vel);
   robotHead.view(enc);
   robotHead.view(con);
   	
   int jnts = 0;
    pos->getAxes(&jnts);
    Vector setpoints;
    setpoints.resize(jnts);
    for(int i = 0; i<=jnts; i++)
	con->setControlMode(i, VOCAB_CM_VELOCITY);
    vel->velocityMove(setpoints.data());
    
    for (cv::Point target : focus)
    {
    	double x = target.x;
        double y = target.y;
    
        double vx = x * 0.1;
        double vy = -y * 0.1;
    
        for (int i=0; i<jnts; i++) {
            setpoints[i] = 0;
        }
        
        setpoints[3] = vy;
        setpoints[4] = vx;
		
	vel->velocityMove(setpoints.data());
	yarp::os::Time::delay(1);
    }
}

