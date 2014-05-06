/*
 * =====================================================================================
 *
 *       Filename:  GenerateTrainingData.c
 *    Description:  
 *        Version:  1.0
 *        Created:  12/20/2013 09:18:59 AM
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  NGUYEN Van Tho (), thonv133@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "LearningSVM.h"

#define NORM 16.0

LearningSVM::LearningSVM(double cValue, double g, char *trainingDescriptorsFile, char *trainingLables)
    :c(cValue), gamma(g){
    descriptors = IO::getDescriptors(trainingDescriptorsFile);
    //testDescriptors = IO::getDescriptors(testDescriptorsFile);
    labels = IO::getLabels(trainingLables);
}

void LearningSVM::train(){

    // Set up SVM's parameters
    CvSVMParams params;
    params.svm_type    = CvSVM::C_SVC;
    params.kernel_type = CvSVM::RBF;
    //params.kernel_type = CvSVM::LINEAR;
    params.gamma = gamma;
    params.C = c;
    params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 1000, 1e-6);
    
    //params.term_crit = cvTermCriteria( CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 1000, FLT_EPSILON );

    float trainingData[NUMBER_OF_EXAMPLE][DIM];
    
    for(int row = 0; row < NUMBER_OF_EXAMPLE; row++){
        for(int col = 0; col < DIM; col++){
            trainingData[row][col] = descriptors[row][col];
        }
    }

    float arrayLabels[NUMBER_OF_EXAMPLE];
    std::copy(labels.begin(), labels.begin() + NUMBER_OF_EXAMPLE, arrayLabels);
    //Labels of classes
    cv::Mat labelsMat(NUMBER_OF_EXAMPLE, 1, CV_32FC1, arrayLabels);
    //cv::Mat trainingDataMat( numberOfExample, 1, CV_32FC(DIM));
    cv::Mat trainingDataMat(NUMBER_OF_EXAMPLE, DIM, CV_32FC1, trainingData);
    svm.train(trainingDataMat, labelsMat, cv::Mat(), cv::Mat(), params);
}

std::vector<float> LearningSVM::classifyAll(std::vector<std::vector<int> > descriptors){
    cv::Mat samples[1000];    
    for(int i = 0; i < 1000; i++){
        float chs[DIM];
        std::copy(descriptors[i].begin(), descriptors[i].end(), chs);
        samples[i] = cv::Mat(1, DIM, CV_32FC1, chs);
    }
    cv::Mat res(1000, 1, CV_32FC1);
    std::vector<float> results;
    for(int i = 0; i < 1000; i++){
        results.push_back(res.at<float>(i));
    }
    return results;
}
int LearningSVM::classify(std::vector<int> descriptor){
    float chs[DIM];
    std::copy(descriptor.begin(), descriptor.end(), chs);
    cv::Mat m(1, DIM, CV_32FC1, chs);
    float res = svm.predict(m);
    return res;
}
