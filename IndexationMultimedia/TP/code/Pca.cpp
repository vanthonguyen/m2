/*
 * =====================================================================================
 *
 *       Filename:  Pca.cpp
 *    Description:  
 *        Version:  1.0
 *        Created:  12/30/2013 11:55:16 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  NGUYEN Van Tho (), thonv133@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "Pca.h"

Pca::Pca(){

}

Pca::Pca(const std::vector<cv::Mat> &images, int nComponent)
:nComponent(nComponent){
    train(images, nComponent);
}

void Pca::train(const std::vector<cv::Mat> &images, int nComponent){
    cv::Mat data = createRowsData(images);
    pca.operator()(data, cv::Mat(), CV_PCA_DATA_AS_ROW, nComponent);
    pca.project(data, trainData);
}

cv::Mat Pca::getProjectedData(const cv::Mat &image){
    cv::Mat imageRow = image.clone().reshape(1,1);
    imageRow.convertTo(imageRow, CV_32FC1, 1/255.0);
    cv::Mat coeffs;
    pca.project(imageRow, coeffs);
    return coeffs;
}

cv::Mat Pca::createRowsData(const std::vector<cv::Mat> &images){
    cv::Mat dst(static_cast<int>(images.size()), images[0].rows*images[0].cols, CV_32FC1);
    for(unsigned int i = 0; i < images.size(); i++)
    {
        cv::Mat image_row = images[i].clone().reshape(1,1);
        cv::Mat row_i = dst.row(i);
        image_row.convertTo(row_i, CV_32FC1, 1/255.0);
    }
    return dst;
}

cv::Mat Pca::getTrainData(){
    return trainData;
}

