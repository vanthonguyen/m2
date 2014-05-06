/*
 * =====================================================================================
 *
 *       Filename:  GaussianBlur.cpp
 *    Description:  
 *        Version:  1.0
 *        Created:  12/28/2013 09:19:32 PM
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

int main(int argc, char ** argv){
    cv::Mat img = cv::imread(argv[1], CV_LOAD_IMAGE_UNCHANGED); 
    int kernelSize = 3;
    if(argc > 2){
        kernelSize = atoi(argv[2]);
    }

    cv::GaussianBlur(img, img, cv::Size(kernelSize, kernelSize), 0, 0, cv::BORDER_DEFAULT);
    
    std::string newName("_" + std::to_string(kernelSize) + ".");
    std::string fileName(argv[1]);
    size_t lastPoint = fileName.find_last_of(".");
    if(lastPoint != std::string::npos){
        fileName.replace(lastPoint, 1, newName); 
    }

    std::cout<< fileName<<std::endl;
    cv::imwrite(fileName, img);
    return 0;
}
