#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <vector>
#include <string>
#include <facial_and_object_detection.h>

#include <utils.h>

using namespace cv;
using namespace std;

const double scale = 1.0;
#define HOUGH_GRADIENT 3
#define LINE_AA 16
void facial_detection(const cv::Mat &img)
{
    cv::Mat image = img;
    cv::CascadeClassifier faceCascade;
    loadCascade(faceCascade);
    std::vector<cv::Rect> faces = detectFaces(image, faceCascade);
    cv::Mat newImage = drawRectangeOnFaces(faces, image);

    cv::imshow("Facial Recognition", newImage);
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
    return image;
}

cv::Mat circle_detection(const cv::Mat &img)
{

    Mat src = img;
    Mat gray;
    cvtColor(src, gray, COLOR_BGR2GRAY);
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
        // circle center
        circle(src, center, 1, Scalar(0, 100, 100), 3, LINE_AA);
        // circle outline
        int radius = c[2];
        circle(src, center, radius, Scalar(255, 0, 255), 3, LINE_AA);
    }
    cv::imshow("Circles", src);

    return src;
}