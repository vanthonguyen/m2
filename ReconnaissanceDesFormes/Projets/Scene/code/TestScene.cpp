/*
 * =====================================================================================
 *
 *       Filename:  testScene.cpp
 *    Description:  
 *        Version:  1.0
 *        Created:  01/04/2014 10:43:16 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/time.h>


#include "Classifier.h"
#include "Common.h"

#define NUMBER_OF_SCENE 13

typedef std::vector<int> row;
typedef std::vector<row> matrix;

int main(int argc, char **argv){
    if(argc < 2){
        std::cout << "usage: " << argv[0] << " learning_file test_file " << std::endl;
        exit(1);
    }

    std::vector<cv::Mat> testImages;
    cv::Mat testLabels(0, 1, CV_32FC1);
    std::vector<std::string> imageNames;
    readCsv(argv[1], testImages, testLabels, imageNames);
   
    
    char *vocabularyFile = VOCABULARY_FILE;
    char *svmFile = SVM_FILE;
    if(argc > 2){
        vocabularyFile = argv[2];
    }
    if(argc > 3){
        svmFile = argv[3];
    }
    
    int dictionarySize = DICTIONARY_SIZE;
    if(argc > 4){
        dictionarySize = atoi(argv[4]);
    }

    Classifier classifier(dictionarySize, vocabularyFile, svmFile);

    cv::Mat results(0, 1, CV_32FC1);


    //std::vector< std::vector<int> >matriceConfusion(NUMBER_OF_SCENE, NUMBER_OF_SCENE); 
    matrix matriceConfusion(NUMBER_OF_SCENE); 
    for (unsigned int i = 0; i < NUMBER_OF_SCENE; i++) {
        std::vector<int> row(NUMBER_OF_SCENE); 
        //matriceConfusion.push_back(row); // Add the row to the main vector
        matriceConfusion[i] = row;
    }
    struct timeval tim;        
    gettimeofday(&tim, NULL);
    double t1=tim.tv_sec+(tim.tv_usec/1000000.0);
debug("#####");
debug(testLabels.size());
debug(testImages.size());
debug(testLabels.at<int>(1201));
debug(testLabels.at<int>(1200));

debug("#####");

    for(unsigned int i = 0; i < testImages.size(); i++){
        float result = classifier.predict(testImages[i]);
        results.push_back(result);
debug(i);
        matriceConfusion[testLabels.at<int>(i) - 1][result - 1]++;            
          if(result != testLabels.at<int>(i)){   
              std::string result_message = cv::format("Predicted class = %d / Actual class = %d.", result, testLabels.at<int>(i));
              std::cout << "Image name = "<<imageNames[i]<<" / " <<result_message << std::endl;
          }
    }


    gettimeofday(&tim, NULL);
    double t2=tim.tv_sec+(tim.tv_usec/1000000.0);
    testLabels.convertTo(testLabels, CV_32FC1);
    double errorRate = (double) cv::countNonZero(testLabels - results) / results.rows;
    std::cout<<"ERR rate " << errorRate << std::endl;
    std::cout<<"Predict in" << t2 -t1 <<"s"<<std::endl;
    printConfusionMatrix(matriceConfusion);
    return 0;
}


