/*
 * =====================================================================================
 *
 *       Filename:  MatrixToImage.cpp
 *
 *    Description:  j
 *
 *        Version:  1.0
 *        Created:  11/07/2013 11:11:26 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  NGUYEN Van Tho (), thonv133@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <fstream>

#define SIZE 100
int main(int argc, char ** argv){
    int grayValue;
    char *inname = "images/finalResultCoil";
    std::ifstream infile(inname);
    if (!infile) {
        std::cout << "There was a problem opening file "
            << inname
            << " for reading."
            << std::endl;
        return 0;
    }
    
    //image 
    cv::Mat confusionMatrixImage(SIZE + 1, SIZE + 1, CV_8U);
    int i = 0;
    while (infile >> grayValue) {
        confusionMatrixImage.at<uchar>(i/SIZE, i % SIZE) = grayValue;
        i++;
    }
    cv::normalize(confusionMatrixImage, confusionMatrixImage, 0, 255, cv::NORM_MINMAX, CV_8U);
    cv::imshow("xxx", confusionMatrixImage);
    cv::waitKey();
    cv::imwrite("confusionMatrixCoil.jpg", confusionMatrixImage);
}
