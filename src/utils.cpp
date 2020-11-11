#include <string>
#include <limits.h>
#include <yarp/os/Os.h>

std::string get_project_path()
{
    char buffer[PATH_MAX];
    yarp::os::getcwd(buffer, sizeof(buffer));
    std::string path = buffer;
    int id = path.find("/robotic_visual_attention/");
    path.erase(id);
    path += "/robotic_visual_attention/";
    return path;
}