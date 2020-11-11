#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#include <vector>
#include <string>
#include <utils.h>
#include <facial_and_object_detection.h>
const double scale = 1.0;

void facial_detection(const cv::Mat &image)
{

    cv::CascadeClassifier faceCascade;
    loadCascade(faceCascade);
    std::vector<cv::Rect> faces = detectFaces(image, faceCascade);
    cv::Mat newImage = drawRectangeOnFaces(faces, image);
    
    cv::imshow("Facial Recognition", newImage);
    cv::waitKey(0);
    cv::destroyAllWindows();
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