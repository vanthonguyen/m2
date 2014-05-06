/*
 * =====================================================================================
 *
 *       Filename:  Detector.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/12/2013 12:48:19 PM
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
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>

#include "Detector.h"


int main(int argc, char ** argv){
    Detector detector(argv[1]);
    detector.begin();
    return 0;
}

Detector::Detector(char *fileName): videoFile(fileName), capture(videoFile){
}

void Detector::begin(){
    while((char)cv::waitKey(30) != 'q' && capture.grab()){
        capture.retrieve(frame);
        cv::Mat gray;
        cv::cvtColor(frame, gray, CV_BGR2GRAY);                                                                                                                 
        if(background.size().width == 0){
            background = gray.clone();
        }
        int rnd = rand()%sampleRate;
        if(rnd == 0){
            background = backgroundSubtructor.getBackground(gray);
        }

        foreground = backgroundSubtructor.subtract(gray, background );
        //cv::threshold(foreground, foreground, 20, 255,  CV_THRESH_BINARY);
        cv::erode(foreground, foreground, cv::Mat());
        cv::dilate(foreground, foreground, cv::Mat());

        cv::imshow("Origine",frame);
        cv::imshow("Foreground",foreground);
        cv::imshow("Background",background);
    }
}
