#include <yarp/os/BufferedPort.h>
#include <yarp/os/LogStream.h>
#include <yarp/os/Network.h>
#include <yarp/sig/Image.h>

#include <read_camera_stream.h>

using namespace yarp::os;
using namespace yarp::sig;

ImageOf<PixelRgb> *read_camera_stream()
{
    Network network; //inits yarp ports
    BufferedPort<ImageOf<PixelRgb>> imagePort;
    imagePort.open("/cameraListener"); //connect to camera using
                                       //yarp connect /icubSim/cam/left /cameraListener

    ImageOf<PixelRgb> *image = read_port_until_image_received(imagePort);
    imagePort.close();
    return image;
}

static ImageOf<PixelRgb> *read_port_until_image_received(BufferedPort<ImageOf<PixelRgb>> &imagePort)
{
    bool imageReceived = false;
    ImageOf<PixelRgb> *image;
    while (!imageReceived)
    {
        image = imagePort.read();
        if (image != nullptr)
        {
            yInfo() << "Image of size " << image->width() << "x" << image->height() << "received.";
            imageReceived = true;
        }
    }
    return image;
}