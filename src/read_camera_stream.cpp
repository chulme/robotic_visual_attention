#include <yarp/os/Bottle.h>
#include <yarp/os/BufferedPort.h>
#include <yarp/os/LogStream.h>
#include <yarp/os/Network.h>
#include <yarp/os/ConnectionReader.h>
#include <yarp/sig/Image.h>

using namespace yarp::os;
using namespace yarp::sig;

int main()
{

    Network network;
    BufferedPort<ImageOf<PixelRgb>> imagePort;
    imagePort.open("/cameraListener"); //connect to camera using
                                       //yarp connect /icubSim/cam/left /cameraListener
    while (1)
    {
        ImageOf<PixelRgb> *image = imagePort.read();
        if (image != NULL)
        {
            yInfo() << "we got an image of size " << image->width() << "X" << image->height();
            return 0;
        }
    }
    imagePort.close();
    return 0;
}