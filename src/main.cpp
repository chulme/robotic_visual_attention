#include <read_camera_stream.h>
#include <yarp/sig/Image.h>

using namespace yarp::sig;

int main()
{
    ImageOf<PixelRgb> *image = read_camera_stream();
    return 0;
}