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

## Components developed by other group members
### Marker Detection
The Aruco library has 3 detection modes in this implementation we used the DM_VIDEO_FAST which was best suited to this implementation as it looks at the currently found marker and similar sizes achieving the best search speeds. Live video wasn’t achieved due to the use of x2go but we were able to play a pre-recorded video using “yarpdev --device opencv_grapper –move /ce4/home/test.mp4 --framerate 20 –loop –name /test/video” (although this command frequently broke and only worked while using Microsoft Remote Desktop then X2GO inside the VM) and running the yarpdatagrabber to save the video then replaying it through the yarpdataplayer.
 
### Circle Detection
This  section aims to explain the circle detection implementation, found in; src/facial_and_object_detection.cpp.
This code uses the OpenCV “Hough circles” library to drive the circle detection 
Initially the image (video frame) is greyscaled and median blurred to reduce the visual noise and make detection easier. The Hough transform algorithm is then used on the new image to detect the circles. Once a circle is detected it is highlighted by drawing over the image, as can be seen below.

The centre of the circle is also stored, so that the iCub knows where to look when following the circles.
This implementation was chosen because its simplicity to use within the OpenCV library.
Unfortunately, due to poor performance caused by the VM, it was hard to get it to run at a reasonable framerate, so moving circles around quickly made the detection poor. Once the frame was still enough the detection worked, but struggled with certain surfaces, such as reflective ones

### Gestures Recognition
The code to allow the robot to wave when it recognises a face is also stored in control_iCubSim_head.cpp under the method void wave(). Also found within this file is the method void salute() which enables the robot to perform a salute when it sees a circle.
This method uses the same setpoints function as in the head control method and as found in lab 3. These methods also use IPositionControl to move the joints to a specific position. The positions for all joints for the first position in the gestures are first established and then, with sufficient time delays to allow each process to complete, only the necessary joint positions are changed. This allows the sim to ‘wave’ the right arm and salute the left.
The wave method has then been written into the main to execute whenever the facial recognition acknowledges a face. First checking if the variable ‘faceCoords’ returns a value, if true, the code executes. The same is done for the salute method, but it instead checks if ‘circleCoords’ returns a value and if true, executes the code.
