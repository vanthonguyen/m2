/*
 * =====================================================================================
 *
 *       Filename:  Classifier.cpp
 *    Description:  
 *        Version:  1.0
 *        Created:  01/03/2014 09:56:23 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "Classifier.h"
#include <sys/time.h>

#define ATTEMPS 2
#define GAMMA 0.5 
#define C_VALUE 300.0

Classifier::Classifier(int size): dictionarySize(size){
    init();
}

Classifier::Classifier(int dictionarySize, char *vocabularyFile, char *svmFile)
:dictionarySize(dictionarySize){
    init();
    bowExtractor = new cv::BOWImgDescriptorExtractor(descriptorExtractor, descriptorMatcher);
    cv::FileStorage fs(vocabularyFile, cv::FileStorage::READ);
    fs["Vocabulary"]>>vocabulary;
    bowExtractor->setVocabulary(vocabulary);
    svm.load(svmFile);
}

void Classifier::train(const std::vector<cv::Mat> &images, const cv::Mat &lbs, double c, double gamma){
    //setup parammeters for svm
    params.svm_type    = CvSVM::C_SVC;
    params.kernel_type = CvSVM::RBF;
    //params.kernel_type = CvSVM::LINEAR;
    params.gamma = gamma;
    params.C = c;
    params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 1000, 1e-6);

    struct timeval tim;        
    gettimeofday(&tim, NULL);
    double t1=tim.tv_sec+(tim.tv_usec/1000000.0);
debug("Begin clustering ...");
    //building dictionary by using Kmeans
    buildDictionary(images);
    
    //dictionary is centroids 
    vocabulary = bowTrainer->cluster();
    gettimeofday(&tim, NULL);
    double t2=tim.tv_sec+(tim.tv_usec/1000000.0);
    bowExtractor->setVocabulary(vocabulary);

    //loop through train data's key point to compute historgram of words
    
    for (unsigned int i = 0; i < images.size(); i++){
        cv::Mat bowDescriptor;
        bowExtractor->compute(images[i], trainKeypoints[i], bowDescriptor);
        trainingData.push_back(bowDescriptor);
    }
    //train using SVM
    svm.train(trainingData, lbs, cv::Mat(), cv::Mat(), params);

std::cout<<"Clustered in " << t2 - t1 << "s"<<std::endl;
}

float Classifier::predict(const cv::Mat &image){
    std::vector<cv::KeyPoint> keypoint;
    featureDetector->detect(image, keypoint);
    cv::Mat bowDescriptorOfTestImage;
    bowExtractor->compute(image, keypoint, bowDescriptorOfTestImage);
    return svm.predict(bowDescriptorOfTestImage);
}

void Classifier::save(char *vocabularyFile, char *svmFile){
    cv::FileStorage fs(vocabularyFile, cv::FileStorage::WRITE);
    fs<<"Vocabulary"<<vocabulary;
    svm.save(svmFile);
}

void Classifier::buildDictionary(const std::vector<cv::Mat> &images){
    for (unsigned int i = 0; i < images.size(); i++){
        std::vector<cv::KeyPoint> keypoint;
        featureDetector->detect(images[i], keypoint);
        trainKeypoints.push_back(keypoint);
        cv::Mat features;
        descriptorExtractor->compute(images[i], keypoint, features); 
        // trainingData.push_back(features);
        bowTrainer->add(features);
    }
}

void Classifier::init(){
    descriptorExtractor = new cv::SurfDescriptorExtractor();
    descriptorMatcher = cv::DescriptorMatcher::create("FlannBased");
    featureDetector = new cv::SiftFeatureDetector();
debug("Dic");
debug(dictionarySize);
    bowTrainer = new cv::BOWKMeansTrainer(dictionarySize,
            cv::TermCriteria( CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 10, 0.01), 
            ATTEMPS, cv::KMEANS_PP_CENTERS);

    bowExtractor = new cv::BOWImgDescriptorExtractor(descriptorExtractor, descriptorMatcher);

    //setup labels
    labels = cv::Mat(0, 1, CV_32FC1);
}
