/*
 * =====================================================================================
 *
 *       Filename:  ClusteringPca.cpp
 *    Description:  
 *        Version:  1.0
 *        Created:  12/31/2013 10:59:45 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  NGUYEN Van Tho (), thonv133@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "ClassifyPca.h"

#define THRESHOLD 15

ClassifyPca::ClassifyPca(int nComponent): LearningPca(nComponent){

}

void ClassifyPca::train(const std::vector<cv::Mat> &images, const std::vector<int> &lbs){
    labels = std::vector<int>(lbs);
    LearningPca::train(images);
}

int ClassifyPca::predict(const cv::Mat &image){
    double minDistance = 1000000;
    int found = 1000000; 
    cv::Mat coeffs = pca.getProjectedData(image);
    for(unsigned int i = 0; i < trainData.rows; i++){
        double distance = cv::norm(coeffs, trainData.row(i)); 
        if(distance < minDistance){
            minDistance = distance;
            found = i;
        }
    }
debug(minDistance);
    if(minDistance < THRESHOLD){
        return labels[found];
    }
    return -1;
}
