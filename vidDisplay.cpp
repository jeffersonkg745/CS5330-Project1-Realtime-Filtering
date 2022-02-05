//
//  main.cpp
//  imgDisplay
//
//  Created by Kaelyn Jefferson on 1/30/22.
//

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "filter.cpp"
#include "filter.h"

#include <iostream>

using namespace cv;

int main(int argc, const char * argv[]) {
    
    cv::VideoCapture *capdev;
    cv::Mat dst;
    
    //open video
    capdev = new cv::VideoCapture(0);
    if (!capdev->isOpened()) {
        printf("Unable to open video for you\n");
        return(-1);
    }
    
    //get the properties of the image
    cv::Size refS((int) capdev->get(cv::CAP_PROP_FRAME_WIDTH),
                  (int) capdev ->get(cv::CAP_PROP_FRAME_HEIGHT));
    printf("expected size %d %d\n", refS.width, refS.height);
    
    cv::namedWindow("Video", 1);
    cv::Mat frame;
    cv::Mat sobxdst;
    cv::Mat sobydst;
    int k = 0;
    
    for (;;) {
        //get a new frame from the camera and treat it as a stream
        *capdev >> frame;
        
        //sets the current frame in greyscale using openCV function
        if (k == 1) {
            cvtColor(frame, dst, COLOR_RGBA2GRAY, 0);
            frame = dst;
        }
        //sets the current frame to alternate greyscale using fxn defined in filter.c
        else if (k ==2) {
            int i = greyscale(frame, dst);
    
            // when i = 0, the function was a success
            if (i == 0) {
                frame = dst;
            }
        }
        //sets the current frame to blurred filter using fxn defined in filter.c
        else if (k == 3) {
            blur5x5(frame, dst);
            frame = dst;
        }
        //sets the current frame to a 3x3 sobel x filter as defined in filter.c
        else if (k == 4) {
            sobelX3x3(frame, dst);
            cv::convertScaleAbs( dst, frame );
        }
        //sets the current frame to a 3x3 sobel y filter as defined in filter.c
        else if (k == 5) {
            sobelY3x3(frame, dst);
            cv::convertScaleAbs( dst, frame );
        }
        //sets the current frame to the Scharr gradient filter (Q10)
        else if (k == 6) {
            cv::Scharr(frame, dst, -1, 0, 1);
            frame = dst;
        }
        //sets the current frame to the Laplacian gradient filter (Q10)
        else if (k == 7) {
            cv::cvtColor(frame, dst, cv::COLOR_RGB2GRAY, 0);
            cv::Laplacian(frame, dst, CV_8U, 3, 1, 0, cv::BORDER_DEFAULT);
            frame = dst;
        }
        //sets the current frame to the Threshold filter (Q10)
        else if (k == 8) {
            cv::threshold(frame, dst, 100, 200, THRESH_BINARY);
            frame = dst;
        }
        //sets the current image to a gradient magnitude image
        else if (k == 9) {
            sobelX3x3(frame, sobxdst);
            sobelY3x3(frame, sobydst);
            magnitude(sobxdst, sobydst, dst);
            cv::convertScaleAbs( dst, frame );
        }
        //sets the current image to a blurred and quantized image
        else if (k == 10) {
            blurQuantize(frame, dst, 15);
            frame = dst;
        }
        //sets the current image to a cartoonized image
        else if (k == 11) {
            cartoon(frame, dst, 15, 10);
            cv::convertScaleAbs( dst, frame );
        }
        
        //quits if the frame is empty
        if (frame.empty()) {
            printf("frame is empty\n");
            break;
        }
        
        //see if there is a waiting keystroke
        char key = cv::waitKey(10);
        
        
        //listens for key of user
        if(key == 'q') {
            //quits the program (q2)
            break;
        } else if ( key == 's'){
            //save an image with that frame (q2)
            imwrite("/Users/kaelynjefferson/Desktop/sampleImage.jpg", frame);
        } else if (key == 'g') {
            //displays a grayscale image with opencv functions (q3)
            k = 1;
        } else if (key == 'h') {
            //displays an alternate grayscale image with defined function in filter.c (q4)
            k = 2;
        } else if (key == 'b') {
            //displays a blurred image by using a 5x5 gaussian filter as separable 1x5 filters (q5)
            k = 3;
        } else if (key == 'x') {
            //displays a 3x3 sobel x filter as separable 1x3 filters (q6)
            k = 4;
        } else if (key == 'y') {
            //displays a 3x3 sobel y filter as separable 1x3 filters (q6)
            k = 5;
        } else if (key == '1') {
            //displays a scharr gradient filter (q10 - 1 of 3 images)
            k = 6;
        }else if (key == '2') {
            //displays a laplacian gradient filter (q10 - 2 of 3 images)
            k = 7;
        }else if (key == '3') {
            //displays a threshold filter (q10 - 3 of 3 images)
            k = 8;
        }else if (key == 'm') {
            //displays a gradient magnitude image (q7)
            k = 9;
        }
        else if (key == 'l') {
            //displays a blurred and quantized image (q8)
            k = 10;
        }
        else if (key == 'c') {
            //displays a cartoonized image(q9)
            k = 11;
        }
        
        cv::imshow("Video", frame);
        
    }
    
    delete capdev;
    return 0;
}
