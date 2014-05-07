/*
 * =====================================================================================
 *
 *       Filename:  LearningPca.cpp
 *    Description:  
 *        Version:  1.0
 *        Created:  12/31/2013 10:34:45 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  NGUYEN Van Tho (), thonv133@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <iostream>
#include "LearningPca.h"

LearningPca::LearningPca(int nComponent): nComponent(nComponent){

}

void LearningPca::train(const std::vector<cv::Mat> &images){
    pca.train(images, nComponent);
    trainData = pca.getTrainData();
}

int LearningPca::predict(const cv::Mat &image){
    return 0;
}
