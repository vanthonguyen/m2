/*
 * =====================================================================================
 *
 *       Filename:  SkinDetector.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/14/2013 12:46:02 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  NGUYEN Van Tho (), thonv133@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */


#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>

#include "Learning.h"
#include "SkinDetector.h"
#include "def.h"

//float *histogram;
const double THRESHOLD = 0.3;

int main(int argc, char ** argv){
    //devoir utiliser parametre
    if(argc < 3){
        printf("Parammeter is not enough, use ./detector scale image_name ...");
        return 1;
    }
    char *ptr;
    int scale = strtol(argv[1], &ptr, 10);
    SkinDetector detector(scale, argv[2]);
    detector.detect();
//    detector.save();
    return 0;
}

SkinDetector::SkinDetector(int s, char * file): learning(s){
    scale = s;
    imageName = file;
    cv::Mat img = cv::imread(file, CV_LOAD_IMAGE_UNCHANGED);
    if(!img.data){
        printf("SkinDetector, No data: %s\n", file );
    }
    cv::cvtColor(img, image, CV_BGR2Lab);
    histogramSkin = learning.getHistogramSkin(); 
    histogramNonSkin = learning.getHistogramNonSkin(); 
    getPosibilityOfSkin();
    getPosibilityOfNonSkin();
}

void SkinDetector::detect(){
    for (int i = 0; i < image.size().width; i++){
        for (int j = 0; j < image.size().height; j++){
            cv::Vec3b labPixel = image.at<cv::Vec3b>(j, i);
            //calculer index
            int a = (int)labPixel.val[1]*scale/MAX_VALUE;
            int b = (int)labPixel.val[2]*scale / MAX_VALUE;
            int index = b*scale + a;
            double ps = histogramSkin.at<double>(b, a); 
            double pns = histogramNonSkin.at<double>(b, a); 
            double possibility = ps*possibilityOfSkin / (ps*possibilityOfSkin + pns*possibilityOfNonSkin);
            if(possibility < THRESHOLD){
                labPixel.val[1] = 128;
                labPixel.val[2] = 128;
                labPixel.val[0] = 0;
                image.at<cv::Vec3b>(j, i) = labPixel;
            }
        }
    }
    cv::cvtColor(image, image, CV_Lab2BGR); 
    cv::imshow("image", image);
    cv::waitKey();
}
int SkinDetector::getPosibilityOfSkin(){
//    if(possibilityOfSkin == 0.0){
        for (int i = 0; i < histogramSkin.size().width; i++){
            for (int j = 0; j < histogramSkin.size().height; j++){
//std::cout<<histogramSkin.at<double>(j,i)<<" ";
                possibilityOfSkin += histogramSkin.at<double>(j,i);
            }
        }
 //   }
    return possibilityOfSkin;
}
int SkinDetector::getPosibilityOfNonSkin(){
//    if(possibilityOfNonSkin == 0.0){
        for (int i = 0; i < histogramNonSkin.size().width; i++){
            for (int j = 0; j < histogramNonSkin.size().height; j++){
//std::cout<<histogramNonSkin.at<double>(j,i)<<" ";
                possibilityOfNonSkin += histogramNonSkin.at<double>(j,i);
            }
        }
//    }
    return possibilityOfNonSkin;
}
void SkinDetector::save(){
    cv::Mat newImage;
    cv::cvtColor(image, newImage, CV_Lab2BGR); 
    imwrite( "output.jpg", newImage);
}
