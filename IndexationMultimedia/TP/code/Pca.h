#ifndef PCA_H
#define PCA_H

#include <vector>
#include <iostream>

#include "opencv2/highgui/highgui.hpp"                                
#include "opencv2/imgproc/imgproc.hpp"

#define debug(str)\
    std::cout<<str<<std::endl;

class Pca{
    public:
        Pca();
        Pca(const std::vector<cv::Mat> &images, int nComponent = 0);
        void train(const std::vector<cv::Mat> &images, int nComponent = 0);
        cv::Mat getProjectedData(const cv::Mat &image);
        cv::Mat getTrainData();
    private:
        cv::PCA pca;
        //Private methode
        cv::Mat createRowsData(const std::vector<cv::Mat> &images); 
        //cv::Mat createRowData(const cv::Mat &image);
        //Private data
        cv::Mat trainData;
        int nComponent;
};
#endif
