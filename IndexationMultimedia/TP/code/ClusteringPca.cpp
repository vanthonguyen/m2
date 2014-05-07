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

#include "ClusteringPca.h"

#define THRESHOLD 30 

#define ATTEMPS 100

ClusteringPca::ClusteringPca(int nComponent): LearningPca(nComponent){

}

void ClusteringPca::train(const std::vector<cv::Mat> &images, int k){
    LearningPca::train(images);
    //cv::Mat centers(k, 1, in.type())
    cv::kmeans(trainData, k, labels, cv::TermCriteria( CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 1000, 0.000001),
               ATTEMPS, cv::KMEANS_PP_CENTERS, centroids);
}

int ClusteringPca::predict(const cv::Mat &image){
    double minDistance = 1000000;
    int found = 1000000; 
    cv::Mat coeffs = pca.getProjectedData(image);
    for(int i = 0; i < trainData.rows; i++){
        double distance = cv::norm(coeffs, trainData.row(i)); 
        if(distance < minDistance){
            minDistance = distance;
            found = i;
        }
    }

    if(minDistance < THRESHOLD){
        return labels[found] + 3;
    }
    return 0;
}

