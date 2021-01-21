# robotic_visual_attention
## High-Level Architecture
The software is written as a single yarp module, that takes input from the /cameraListener port, which connects to the iCub’s left eye /icubSim/cam/left, then sends output images across various ports for debugging and demonstration of the functionality of the different aspects of the system. This has the benefit of a quicker initialisation process, only requiring building the software and connect the camera to the /cameraListener port to have the program function as intended. Furthermore, this approach follows a more traditional C++ project layout, with a singular main and functions split across relevant files. Data is passed between functions rather than ports, creating a more approachable software for those unfamiliar with yarp. 

## Linear Filters
### Colour Threshold
The software contained in color_threshold() method receives an OpenCV image and outputs the same image with a colour threshold filter applied, whereby it filters out everything outside of a range.
Using the OpenCV inRange function, we can accept a range of values of a given input using both a lower and upper threshold. In the context of colour thresholds, we use the OpenCV Scalar object to indicate the blue-green-red (BGR) colour values for each threshold. This outputs a grayscale image of pixels that fall within the threshold, so this is converted into an RGB format and the bitwise AND operation between the RGB-format mask and the original image is applied. This is so that the original colours of the image are shown instead of a grayscale image.

![](https://i.imgur.com/hAzN5Lp.png)

### Edge Detection
For our implemented we adopted a canny threshold filter for edge detection, this is because it is a widely used filter with a strong documentation as well as a low error rate. The software first blurs the input image to reduce noise, then the canny edge detection algorithm is applied.

![](https://i.imgur.com/N3BWceA.png)

## Facial Detection
This section aims to explain the software found in src/facial_and_object_detection.cpp.

The software uses a cascade classifier, which is a machine learning algorithm and so requires to be trained before use. The reasoning behind adopting this algorithm is that it’s a very popular implementation of facial detection using OpenCV – so has proven to be a robust algorithm with good documentation and accompanying free training data, meaning the team does not have to.
The facial_detection() method is the access point of our implementation, which first loads and trains the classifier with the training data, then faces are detected, with the input image for detection being scalable to improve performance. Following this, bounding boxes are drawn on the faces detected in the image and coordinates of faces calculated.

![](https://i.imgur.com/Ogl3cod.png)

