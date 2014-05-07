/*
 * =====================================================================================
 *
 *       Filename:  TestPca.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/31/2013 01:14:14 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  NGUYEN Van Tho (), thonv133@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <iostream>
#include <sys/time.h>

#include "Pca.h"
#include "Utils.h"
#include "IO.h"
#include "ClusteringPca.h"
#include "ClassifyPca.h"


#define TYPE1_FOLDER "type1/"
#define TYPE2_FOLDER "type2/"
#define AUTRE_FOLDER "autre/"

#define K 8
#define RATIO 0.11
#define THRESHOLD_GRAY_LEVEL 240

void experiment(const std::vector<cv::Mat> &type1s, const std::vector<cv::Mat> &type2s,
        const std::vector<cv::Mat> &others, const std::vector<cv::Mat> tests, std::vector<std::string> listFileTest){

    ClassifyPca classifier(0);
    ClusteringPca cluster(12);
    std::vector<cv::Mat> images;
    std::vector<int> labels;
    for(unsigned int i = 0; i < type1s.size(); i++){
        images.push_back(type1s[i]);
        labels.push_back(1); 
    }

    for(unsigned int i = 0; i < type2s.size(); i++){
        images.push_back(type2s[i]);
        labels.push_back(2); 
    }

    struct timeval tim;
    gettimeofday(&tim, NULL);
    double t1=tim.tv_sec+(tim.tv_usec/1000000.0); 
    classifier.train(images, labels);
//debug(classifier.predict(images[0]));
    cluster.train(others, K);
    std::vector<std::vector<std::string>> results(K + 3);

    gettimeofday(&tim, NULL);
    double t2=tim.tv_sec+(tim.tv_usec/1000000.0); 

    for(unsigned int i = 0; i < tests.size(); i++){
        int predicted = classifier.predict(tests[i]);
        if(predicted < 1){
            predicted = cluster.predict(tests[i]);
        }
//        std::cout<<listFileTest[i]<< " is classified as class " << predicted << std::endl;
        std::vector<std::string> v = results.at(predicted);
        v.push_back(listFileTest[i]);
        results.at(predicted) = v;        
    }

    gettimeofday(&tim, NULL);
    double t3 = tim.tv_sec+(tim.tv_usec/1000000.0); 

    std::cout<< "Predict time: " << t3 - t2<< std::endl;
    std::cout<< "Trainning time: " << t2 - t1<< std::endl;

    for(unsigned int i = 0; i < results.size(); i++){
        std::vector<std::string> v = results[i];
        for (unsigned int j = 0; j < v.size(); j++){
            std::cout<<v[j]<< " is classified as class " << i << std::endl;
        }
    }
}

int main(int argc, char **argv){
/*    char *testFolder;
    if(argc > 1){
        testFolder = argv[1];
    }else{
        testFolder = TEST_FOLDER;
    }
*/
    std::vector<cv::Mat> type1s;
    std::vector<cv::Mat> type2s;
    std::vector<std::string> listFile;                                                                                                                      
    //read images of type 1 in folder and get list descriptors
    listFile = IO::getFilesInFolder(TYPE1_FOLDER);
    std::random_shuffle(listFile.begin(), listFile.end());

    for(unsigned int i = 0; i < listFile.size(); i++){
        std::string fileName = TYPE1_FOLDER + listFile[i];
        cv::Mat image = cv::imread(fileName, CV_LOAD_IMAGE_GRAYSCALE);
        //cv::threshold(image, image, THRESHOLD_GRAY_LEVEL, 255, CV_THRESH_TRUNC);
        //cv::GaussianBlur(image, image, cv::Size(3, 3), 0, 0, cv::BORDER_DEFAULT);
        type1s.push_back(Utils::getHeader(image, RATIO));
    }

    std::vector<std::string> listFile2 = IO::getFilesInFolder(TYPE2_FOLDER);
    std::random_shuffle(listFile2.begin(), listFile2.end());
    for(unsigned int i = 0; i < listFile2.size(); i++){                     
        std::string fileName = TYPE2_FOLDER + listFile2[i];
        cv::Mat image = cv::imread(fileName, CV_LOAD_IMAGE_GRAYSCALE);
       // cv::GaussianBlur(image, image, cv::Size(3, 3), 0, 0, cv::BORDER_DEFAULT);
        type2s.push_back(Utils::getHeader(image, RATIO));
    }


    std::vector<cv::Mat> others;
    std::vector<std::string> listFileAutre = IO::getFilesInFolder(AUTRE_FOLDER);
    std::random_shuffle(listFileAutre.begin(), listFileAutre.end());

    for(unsigned int i = 0; i < listFileAutre.size(); i++){                     
        std::string fileName = AUTRE_FOLDER + listFileAutre[i];
        cv::Mat image = cv::imread(fileName, CV_LOAD_IMAGE_GRAYSCALE);
        //cv::GaussianBlur(image, image, cv::Size(3, 3), 0, 0, cv::BORDER_DEFAULT);
        others.push_back(Utils::getHeader(image, RATIO));
    }
    //shuffle vectors
    //std::random_shuffle(type1s.begin(), type1s.end());
    //std::random_shuffle(type2s.begin(), type2s.end());
    //std::random_shuffle(others.begin(), others.end());

    unsigned int s1 = type1s.size()/3;
    unsigned int s2 = type2s.size()/3;
    unsigned int s3 = others.size()/3;

    for(int i = 0; i < 3; i++){
        if(i != 2){
            std::vector<cv::Mat> validation(type1s.begin() + s1 * i, type1s.begin() + s1 * (i + 1) );
            std::vector<cv::Mat> type1Train(type1s);
            type1Train.erase(type1Train.begin() + s1 * i, type1Train.begin() + s1 * (i + 1));

            std::vector<std::string> validationFileNames(listFile.begin() + s1 * i, listFile.begin() + s1 * (i + 1) ); 

            validation.insert(validation.end(), type2s.begin() + s2 * i, type2s.begin() + s2 * (i + 1) );
            std::vector<cv::Mat> type2Train(type2s);
            type2Train.erase(type2Train.begin() + s2 * i, type2Train.begin() + s2 * (i + 1));
            validationFileNames.insert(validationFileNames.end(), 
                    listFile2.begin() + s2 * i, listFile2.begin() + s2 * (i + 1));

            validation.insert(validation.end(), others.begin() + s3 * i, others.begin() + s3 * (i + 1) );
            std::vector<cv::Mat> type3Train(others);
            type3Train.erase(type3Train.begin() + s3 * i, type3Train.begin() + s3 * (i + 1));
            validationFileNames.insert(validationFileNames.end(), 
                    listFileAutre.begin() + s3 * i, listFileAutre.begin() + s3 * (i + 1));
//debug(validation.size());
//debug(validationFileNames.size());
//debug(type1Train.size());
//debug(type2Train.size());
            experiment(type1Train, type2Train, type3Train, validation, validationFileNames);
        }else{
            std::vector<cv::Mat> validation(type1s.begin() + s1 * i, type1s.end() );
            std::vector<cv::Mat> type1Train(type1s);
            type1Train.erase(type1Train.begin() + s1 * i, type1Train.end());

            std::vector<std::string> validationFileNames (listFile.begin() + s1 * i, listFile.end() ); 

            validation.insert(validation.end(), type2s.begin() + s2 * i, type2s.end() );
            std::vector<cv::Mat> type2Train(type2s);
            type2Train.erase(type2Train.begin() + s2 * i, type2Train.end());
            validationFileNames.insert(validationFileNames.end(), 
                    listFile2.begin() + s2 * i, listFile2.end());

            validation.insert(validation.end(), others.begin() + s3 * i, others.end() );
            std::vector<cv::Mat> type3Train(others);
            type3Train.erase(type3Train.begin() + s3 * i, type3Train.end());
            validationFileNames.insert(validationFileNames.end(), 
                    listFileAutre.begin() + s3 * i, listFileAutre.end());
            experiment(type1Train, type2Train, type3Train, validation, validationFileNames);
        }
    }
    return 0;
}
