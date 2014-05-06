/*
 * =====================================================================================
 *       Filename:  Training.cpp
 *    Description:  
 *        Version:  1.0
 *        Created:  01/04/2014 06:06:40 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 * =====================================================================================
 */

#include "Classifier.h"
#include "Common.h"


int main(int argc, char **argv){
    if(argc < 2){
        std::cout << "usage: " << argv[0] << " learning_file test_file " << std::endl;
        exit(1);
    }

    char *vocabularyFile = VOCABULARY_FILE;
    char *svmFile = SVM_FILE;
    if(argc > 2){
        vocabularyFile = argv[2];
    }
    if(argc > 3){
        svmFile = argv[3];
    }

    double c = 512; 
    double gamma = 0.5;

    if(argc > 4){
        c = atof(argv[4]);
    }
    if(argc > 5){
        gamma  = atof(argv[5]);
    }

    int dictionarySize = DICTIONARY_SIZE;
    if(argc > 6){
        dictionarySize = atoi(argv[6]);
    }

    std::vector<cv::Mat> learningImages;
    cv::Mat learningLabels(0, 1, CV_32FC1);
    readCsv(argv[1], learningImages, learningLabels);

    Classifier classifier(dictionarySize);
    
    classifier.train(learningImages, learningLabels, c, gamma);

    classifier.save(vocabularyFile, svmFile);
    return 0;
}
