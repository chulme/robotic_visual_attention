#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#include <yarp/sig/Image.h>
#include <yarp/cv/Cv.h>
#include <vector>
#include <string>
#include <facial_and_object_detection.h>

#include <utils.h>

using namespace cv;
using namespace std;

const double scale = 1.0;
const int HOUGH_GRADIENT = 3;
const int LINE_AA = 16;

std::vector<cv::Point> facial_detection(const cv::Mat &in, yarp::sig::ImageOf<yarp::sig::PixelRgb> &out)
{
    cv::Mat image = in.clone(); //OpenCV can override const, so clone to be safe and ensure no reference between old and new.
    cv::CascadeClassifier faceCascade;
    loadCascade(faceCascade);
    std::vector<cv::Rect> faces = detectFaces(image, faceCascade);
    cv::Mat imageWithBoundingBoxes = drawRectangeOnFaces(faces, image);
    out = yarp::cv::fromCvMat<yarp::sig::PixelRgb>(imageWithBoundingBoxes);
    return getFaceCoords(faces);
}

static void loadCascade(cv::CascadeClassifier &cascade)
{
    std::string path = get_project_path();
    cascade.load(path + "data/haarcascade_frontalface_default.xml");
}

static std::vector<cv::Rect> detectFaces(const cv::Mat &image, cv::CascadeClassifier &faceCascade)
{
    cv::Mat gray; //, smallImg;
    cv::cvtColor(image, gray, cv::COLOR_RGB2GRAY);
    cv::resize(gray, gray, cv::Size(gray.size().width / scale, gray.size().height / scale));

    std::vector<cv::Rect> faces;
    faceCascade.detectMultiScale(gray, faces, 1.1, 3, 0, cv::Size(30, 30));
    return faces;
}

static cv::Mat drawRectangeOnFaces(const std::vector<cv::Rect> faces, const cv::Mat &image)
{
    cv::Mat newImage = image;
    cv::Scalar colour = cv::Scalar(255, 0, 0);
    for (cv::Rect area : faces)
    {
        rectangle(newImage,
                  cv::Point(cvRound(area.x * scale), (cvRound(area.y * scale))),
                  cv::Point(cvRound((area.x + area.width - 1) * scale), cvRound((area.y + area.height - 1) * scale)),
                  colour);
    }
    return newImage;
}

std::vector<cv::Point> circle_detection(const cv::Mat &img, yarp::sig::ImageOf<yarp::sig::PixelRgb> &out)
{

    cv::Mat src = img.clone(); //OpenCV can override const, so clone to be safe and ensure no reference between old and new.
    Mat gray;
    std::vector<cv::Point> circleCoords;
    cvtColor(src, gray, COLOR_RGB2GRAY);
    medianBlur(gray, gray, 5);
    vector<Vec3f> circles;
    HoughCircles(gray, circles, HOUGH_GRADIENT, 1,
                 gray.rows / 16, // change this value to detect circles with different distances to each other
                 100, 30, 5, 30  // change the last two parameters
                                 // (min_radius & max_radius) to detect larger circles

    );
    for (size_t i = 0; i < circles.size(); i++)
    {

        Vec3i c = circles[i];
        Point center = Point(c[0], c[1]);
        circleCoords.push_back(center);
        // circle center
        circle(src, center, 1, Scalar(0, 100, 100), 3, LINE_AA);
        // circle outline
        int radius = c[2];
        circle(src, center, radius, Scalar(255, 0, 255), 3, LINE_AA);
    }

    out = yarp::cv::fromCvMat<yarp::sig::PixelRgb>(src);
    return circleCoords;
}
// static std::vector<cv::Point> getFaceCoords(const std::vector<cv::Rect> faces)
// {
//     std::vector<cv::Point> faceCoords;
//     for (cv::Rect face : faces)
//     {
//         faceCoords.push_back({face.x, face.y});
//     }
//     return faceCoords;
// }

static std::vector<cv::Point> getFaceCoords(const std::vector<cv::Rect> faces)
{
    std::vector<cv::Point> faceCoords;
    for (cv::Rect face : faces)
    {
        faceCoords.push_back(cv::Point(((2 * face.x + face.width - 1) / 2 * scale),
                                       ((2 * face.y + face.height - 1) / 2 * scale)));
    }
    return faceCoords;
}
