/*
 * =====================================================================================
 *
 *       Filename:  Classify.cpp
 *
 *    Description:  implementation
 *        Version:  1.0
 *        Created:  12/09/2013 02:02:20 PM
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  NGUYEN Van Tho (), thonv133@gmail.com
 *   Organization:  IFI
 *
 * =====================================================================================
 */

#include "Classify.h"

int main(int argc, char ** argv){
    Classify Classify(argv[1]);  
}   

Classify::Classify(char *img){
    imageOriginal = cv::imread(img, CV_LOAD_IMAGE_UNCHANGED);
    cv::cvtColor(imageOriginal, imageGray, CV_BGR2GRAY);
    //cv::medianBlur(imageGray, imageGray, 5);
    cv::threshold(imageGray, imageGray, 220, 255, CV_THRESH_BINARY_INV);
    cv::medianBlur(imageGray, imageGray, 5);
    //cv::dilate(imageGray, imageGray, cv::Mat());
    xyCut.setImage(imageGray);
    std::vector<cv::Rect> blocks = xyCut.getBlocks();
    for(int i = 0; i < blocks.size(); i++){
    //    std::cout<< blocks[i].tl()<<std::endl;
        cv::rectangle(imageOriginal, blocks[i], cv::Scalar(33,33, 240), 3);
    }
    std::cout<<"No des blocks"<< blocks.size() <<std::endl;
    cv::namedWindow("test", CV_WINDOW_AUTOSIZE);
    cv::imshow("test", imageOriginal);
    cv::imshow("gray", imageGray);
    cv::imwrite("../output/" + std::string(img), imageOriginal );
    cv::waitKey();
while(cv::waitKey() != 27){                                                                                                                               
}
}
