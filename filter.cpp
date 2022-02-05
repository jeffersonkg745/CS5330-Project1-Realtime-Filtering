//
//  filter.cpp
//  imgDisplay
//
//  Created by Kaelyn Jefferson on 1/30/22.
//

//image manipulation functions go in this file


#include <stdio.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/mat.hpp>
#include "filter.h"
#include <iostream>


//alternate greyscale function where we take the average of RGB
int greyscale( cv::Mat &src, cv::Mat &dst) {
    
    //create the dst image with the same size as the src
    dst.create(src.size(), src.type());
    
    //find each pixel with 2 for loops
    for (int i = 0; i < src.rows; i++) {
        
        //pointer to the beginning of the ith row
        cv::Vec3b *ptr = src.ptr<cv::Vec3b>(i);
        
        for (int j = 0; j < src.cols; j++) {
            
            //first color channel (blue)
            int blue = ptr[j][0];
            int green = ptr[j][1];
            int red = ptr[j][2];
            
            int avgOfColors = (blue + green + red) / 3;
            
            //set the colors equal to their average
            ptr[j][0] = avgOfColors;
            ptr[j][1] = avgOfColors;
            ptr[j][2] = avgOfColors;
        }
    }
    
    return 0;
}

//blurs an image by using a 5x5 gaussian filter as separable 1x5 filters
int blur5x5( cv::Mat &src, cv::Mat &dst) {
    
    //create the intermediate and destination cv::Mat objects
    cv::Mat intermed;
    intermed.create(src.size(), src.type());
    dst.create(src.size(), src.type());
    
    //make our own initialized color channel
    cv::Vec3i result = {0, 0, 0};
    cv::Vec3i resultCol = {0, 0, 0};
    
    //applying 5x1 filter --> [1 2 4 2 1] horizontal
    for (int i = 2; i < src.rows-2; i++) {
        for (int j = 2; j < src.cols-2; j++){
            for (int c = 0; c < 3; c++) {
                result[c] = src.at<cv::Vec3b>(i-2, j-2)[c] + src.at<cv::Vec3b>(i-2, j-1)[c]*2
                + src.at<cv::Vec3b>(i-2, j)[c]*4 + src.at<cv::Vec3b>(i-2, j+1)[c]*2
                + src.at<cv::Vec3b>(i-2, j+2)[c];
                
                result[c] /= 10;
            }
            intermed.at<cv::Vec3b>(i,j) = result;
        }
    }

    //applying 1x5 filter --> [1 2 4 2 1] vertical
    for (int i = 2; i < intermed.rows-2; i++) {
        for (int j = 2; j < intermed.cols-2; j++){
            for (int c = 0; c < 3; c++) {
                result[c] = intermed.at<cv::Vec3b>(i-2, j-2)[c] + intermed.at<cv::Vec3b>(i-1, j-2)[c]*2
                + intermed.at<cv::Vec3b>(i, j-2)[c]*4 + intermed.at<cv::Vec3b>(i+1, j-2)[c]*2
                + intermed.at<cv::Vec3b>(i+2, j-2)[c];
                
                result[c] /= 10;
            }
            dst.at<cv::Vec3b>(i,j) = result;
        }
    }
    return 0;
}
     


int sobelX3x3( cv::Mat &src, cv::Mat &dst ){
    //sobel x should multiply [-1 0 1] by
    //[1
    // 2
    // 1]

    //create intermediate and dst objects with type CV_16SC3
    cv::Mat intermed;
    intermed.create(src.size(), CV_16SC3);
    dst.create(src.size(), CV_16SC3);
    
    //make our own initialized color channel
    //vec3s works for intermed and dst
    cv::Vec3s result= {0, 0, 0};

    //applying 3x1 filter
    for (int i = 1; i < src.rows-1; i++) {
        for (int j = 1; j < src.cols-1; j++){
            for (int c = 0; c < 3; c++) {
                result[c] = src.at<cv::Vec3b>(i-1, j-1)[c]*-1 + src.at<cv::Vec3b>(i-1, j)[c]*0 + src.at<cv::Vec3b>(i-1, j+1)[c];
            }
            intermed.at<cv::Vec3s>(i,j) = result;
        }
    }

    //applying 1x3 filter
    for (int i = 1; i < intermed.rows-1; i++) {
        for (int j = 1; j < intermed.cols-1; j++){
            for (int c = 0; c < 3; c++) {
                result[c] = intermed.at<cv::Vec3s>(i-1, j-1)[c] + intermed.at<cv::Vec3s>(i, j-1)[c]*2 + intermed.at<cv::Vec3s>(i+1, j-1)[c] ;
                result[c] /= 4;
            }
            dst.at<cv::Vec3s>(i,j) = result;
        }
    }
    return 0;
}
int sobelY3x3( cv::Mat &src, cv::Mat &dst ){
    //sobel y should multiply [1 2 1] by
    //[1
    // 0
    //-1]
    
    cv::Mat intermed;
    intermed.create(src.size(), CV_16SC3);
    dst.create(src.size(), CV_16SC3);
    
    //make our own initialized color channel
    cv::Vec3s result = {0, 0, 0};
 
    //applying 3x1 filter
    for (int i = 1; i < src.rows-1; i++) {
        for (int j = 1; j < src.cols-1; j++){
            for (int c = 0; c < 3; c++) {
                result[c] = src.at<cv::Vec3b>(i-1, j-1)[c] + src.at<cv::Vec3b>(i-1, j)[c]*2
                + src.at<cv::Vec3b>(i-1, j+1)[c];
            
                result[c] /= 4;
            }
            intermed.at<cv::Vec3s>(i,j) = result;
        }
    }

    //applying 1x3 filter
    for (int i = 1; i < intermed.rows-1; i++) {
        for (int j = 1; j < intermed.cols-1; j++){
            for (int c = 0; c < 3; c++) {
                result[c] = intermed.at<cv::Vec3s>(i-1, j-1)[c] + intermed.at<cv::Vec3s>(i, j-1)[c]*0
                + intermed.at<cv::Vec3s>(i+1, j-1)[c]*-1 ;
            }
            dst.at<cv::Vec3s>(i,j) = result;
        }
    }
    return 0;
}
 

int magnitude( cv::Mat &sx, cv::Mat &sy, cv::Mat &dst ){

    dst.create(sx.size(), sx.type());
    
    //sx and sy come in as vec3s
    cv::Vec3s result = {0, 0, 0};
    
    //only need to go through one image since sy should be the same
    for (int i = 0; i < sx.rows; i++) {
        for (int j = 0; j < sx.cols; j++){
            //go pixel by pixel for x and y
            for (int c = 0; c < 3; c++) {
                //find the euclidian distance with formula to find the new pixel values
                result[c] = sqrt(pow(sx.at<cv::Vec3s>(i, j)[c], 2) + pow(sy.at<cv::Vec3s>(i, j)[c], 2));
            }
            dst.at<cv::Vec3s>(i,j) = result;
        }
    }
    
    return 0;
}

int blurQuantize( cv::Mat &src, cv::Mat &dst, int levels ){
    
    cv::Mat intermed;
    intermed.create(src.size(), src.type());
    dst.create(src.size(), src.type());
    cv::Vec3i result = {0, 0, 0};
    
    //first blur the image
    blur5x5(src, intermed);
    
    //then quantize the image
    int buckets = 255 / levels;
    for (int i = 0; i < intermed.rows; i++) {
        for (int j = 0; j < intermed.cols; j++){
            for (int c = 0; c < 3; c++) {
                result[c] = intermed.at<cv::Vec3b>(i, j)[c] / buckets;
                result[c] = result[c] * buckets;
            }
            dst.at<cv::Vec3b>(i,j) = result;
        }
    }
    return 0;
}


int cartoon( cv::Mat &src, cv::Mat&dst, int levels, int magThreshold ) {

    //make additional copies of the pic to alter
    cv::Mat xPic;
    xPic.create(src.size(), CV_16SC3); //for x sobel fxn that returns vec3s
    cv::Mat yPic;
    yPic.create(src.size(), CV_16SC3); //for y sobel fxn that returns vec3s
    cv::Mat magImg;
    magImg.create(src.size(), CV_16SC3); //for the mag function that returns vec3s
    cv::Mat intermed;
    intermed.create(src.size(), src.type()); //result of blurquantize fxn is vec3b
    dst.create(src.size(), CV_16SC3); //dst will be vec3s here and convert in vidDisplay.c
    
    //calculate the gradient magnitude on src: send src to get sobel x and sobel y values; then send those to the magnitude fxn
    sobelX3x3(src, xPic);
    sobelY3x3(src, yPic);
    magnitude(xPic, yPic, magImg);
    
    //use the blur and quantize filters on src and save to intermed
    blurQuantize(src, intermed, levels);
    
    cv::Vec3s resultIntermed = {0, 0, 0};
    cv::Vec3s resultMagThresh = {0, 0, 0};
    
    //third loop to see what pixels are larger than the threshold and set
    //them to black
    for (int i = 0; i < intermed.rows; i++) {
        for (int j = 0; j < intermed.cols; j++){
            for (int c = 0; c < 3; c++) {
                //get all color channels for a pixel in intermed
                resultIntermed[c] = intermed.at<cv::Vec3b>(i, j)[c];
                
                //get all color channels for a pixel in magnitude to compare to magThreshold
                resultMagThresh[c] = magImg.at<cv::Vec3s>(i, j)[c];
            }
            
            //get the average of each color channel within the pixel and
            //compare it to the threshold
            int avgnum = resultMagThresh[0] + resultMagThresh[1] + resultMagThresh[2];
            avgnum /= 3;
            
            //if larger than threshold, set pixel to black
            if (avgnum > magThreshold) {
                resultIntermed = {0,0,0};
            }

            dst.at<cv::Vec3s>(i,j) = resultIntermed;
        }
    }
    
    return 0;
}
