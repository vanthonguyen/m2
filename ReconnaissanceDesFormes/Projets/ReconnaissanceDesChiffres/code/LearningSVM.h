#ifndef LEARNING_SVM_H 
#define LEARNING_SVM_H 

#include<iostream>
#include<vector>

#include <opencv2/opencv.hpp>                                                                                                                                   
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/ml/ml.hpp>

#include "IO.h"

#define DESCRIPTOR_FILE "descriptors"
#define DIM W*W 

#define NUMBER_OF_EXAMPLE 5000

#define debug(str)\
        std::cout<<str<<std::endl;

class LearningSVM{
    public:
        LearningSVM(double cValue, double g, char *trainingDescriptorsFile, char *trainingLables);
        void train();
        int classify(std::vector<int> descriptor);
        std::vector<float> classifyAll(std::vector<std::vector<int> > descriptors);
    private:
        CvSVM svm;        
        std::vector<std::vector<int> > descriptors;
//        std::vector<std::vector<int> > testDescriptors;
        std::vector<float> labels;
        double c;
        double gamma;
};
#endif
