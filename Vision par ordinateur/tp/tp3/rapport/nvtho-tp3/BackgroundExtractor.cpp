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
#include <list>
#include <vector>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <math.h>
#include <thread>
#include <mutex>

#include "BackgroundExtractor.h"

std::mutex mt;

void insertionSort(uchar arr[], int length){
    int i, j ,tmp;                                                                     
    for (i = 1; i < length; i++)  {  
        j = i;  
        while (j > 0 && arr[j - 1] > arr[j])
        {
            tmp = arr[j];
            arr[j] = arr[j - 1];
            arr[j - 1] = tmp;
            j--;
        }
    }
}

//float *histogram;
void processHistory(std::list<cv::Mat> history, std::list<cv::Mat> &backgroundHistory, cv::Mat &background){
    std::lock_guard<std::mutex> guard(mt);
    int rows = history.front().size().height;
    int cols = history.front().size().width;
    int numberOfHistoryFrame = history.size();
    //background = history[0].clone();
    cv::Mat b = cv::Mat::zeros(rows, cols, CV_8U);
    for(int row = 0; row < rows; row++ ){
        for(int col = 0; col < cols; col++){
            uchar pixels[numberOfHistoryFrame];
            int k = 0;
            for ( std::list<cv::Mat>::iterator it = history.begin(); it != history.end(); ++it){
                pixels[k] = (*it).at<uchar>(row, col);
                k++;
            }
            insertionSort(pixels, numberOfHistoryFrame);
            b.at<uchar>(row, col) = pixels[numberOfHistoryFrame/2 + 1];       
        }
    }
    if(backgroundHistory.size() == maxNumberOfBackground){
        backgroundHistory.pop_front();
    }
    //only add the stable background
    if(numberOfHistoryFrame == maxNumberOfHistoryFrame){
        backgroundHistory.push_back(b);
    }
    //backgroundHistory.push_back(b);
    int numberOfBackground = backgroundHistory.size();
    if(numberOfBackground == 10){
        cv::Mat newBg = cv::Mat::zeros(rows, cols, CV_8U);
        for(int row = 0; row < rows; row++ ){
            for(int col = 0; col < cols; col++){
                uchar pixels[numberOfBackground];
                int k = 0;
                for ( std::list<cv::Mat>::const_iterator it = backgroundHistory.begin(), end = backgroundHistory.end(); it != end; ++it){
                    pixels[k] = (*it).at<uchar>(row, col);
                    k++;
                }
                insertionSort(pixels, numberOfBackground);
                newBg.at<uchar>(row, col) = pixels[numberOfBackground/2 + 1];       
            }
        }
        background = newBg.clone();
    }else{
        background = b.clone();
    }
}


BackgroundExtractor::BackgroundExtractor(){
}

cv::Mat BackgroundExtractor::getBackground(cv::Mat frame){
    // cv::cvtColor(frame, frame, CV_BGR2GRAY);
    //convert to grayscale
    if(numberOfHistoryFrame < maxNumberOfHistoryFrame ){
        //to grayscale
        //cv::Mat gray;
        history.push_back(frame);
        numberOfHistoryFrame++;
    }else{
        history.pop_front();
        history.push_back(frame);
    }
    if(numberOfHistoryFrame > minNumberOfHistoryFrame){
        std::thread t(processHistory, history, std::ref(backgroundHistory), std::ref(background)); 
        t.detach();
    }else{
        background = frame;
    }
    return background;
}

cv::Mat BackgroundExtractor::subtract(cv::Mat m1, cv::Mat m2){
    //check size, channels
    int rows = m1.size().height;
    int cols = m2.size().width;
    cv::Mat result = cv::Mat::zeros(m1.size(), CV_8U);
    for(int row = 0; row < rows; row++ ){
        for(int col = 0; col < cols; col++){
            uchar val = abs(m1.at<uchar> (row, col) - m2.at<uchar> (row, col));
            result.at<uchar> (row, col) = (val < threshold) ? 0 : 255;
            //result.at<uchar> (row, col) = val;
        }
    }
    return result;
}
