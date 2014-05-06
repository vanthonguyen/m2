/*
 * =====================================================================================
 *
 *       Filename:  Classifier.h
 *    Description:  
 *        Version:  1.0
 *        Created:  01/03/2014 10:06:38 AM
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <vector>
#include <iostream>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/ml/ml.hpp"


#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#define debug(str)\
    std::cout<<str<<std::endl;

class Classifier{
    public:
        Classifier(int dictionarySize);
        Classifier(int dictionarySize, char *vocabularyFile, char *svmFile);
        //train(int dictionarySize);
        void train(const std::vector<cv::Mat> &images, const cv::Mat &lbs, double c = 300.0, double gamma = 0.5); 
        float predict(const cv::Mat &image);
        void save(char *vocabularyFile, char *svmFile);
    private:
        void buildDictionary(const std::vector<cv::Mat> &images);
        void init();

        int dictionarySize;
        cv::Mat labels;
        cv::Mat trainingData;
        cv::Ptr<cv::BOWKMeansTrainer> bowTrainer;
        cv::Ptr<cv::BOWImgDescriptorExtractor> bowExtractor;


        cv::Ptr<cv::DescriptorExtractor> descriptorExtractor;
        cv::Ptr<cv::DescriptorMatcher> descriptorMatcher;
        
        cv::Ptr<cv::FeatureDetector> featureDetector;

        //store keypoint to avoid recompute
        std::vector<std::vector<cv::KeyPoint> > trainKeypoints;
        cv::Mat vocabulary;
        //svm and it's params
        cv::SVM svm; 
        CvSVMParams params;
};
#endif
