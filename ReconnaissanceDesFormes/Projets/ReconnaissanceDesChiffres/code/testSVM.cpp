/*
 * =====================================================================================
 *
 *       Filename:  testSVM.cpp
 *    Description:  
 *        Version:  1.0
 *        Created:  12/22/2013 11:47:01 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  NGUYEN Van Tho (), thonv133@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <sys/time.h>  

#include "LearningSVM.h"

#define TEST_LABEL_FILE "../DigitData/testlabels.txt"

int execute(double c, double gamma, char *labels, char *testDescriptorsFile){

    LearningSVM classifier(c, gamma, DESCRIPTOR_FILE, labels);
    std::vector<std::vector<int> > testDescriptors = IO::getDescriptors(testDescriptorsFile);

    std::vector<float> testLabels = IO::getLabels(TEST_LABEL_FILE);
    classifier.train();
    int correct = 0;
    for(int i = 0; i < testDescriptors.size(); i++){
        int rs = classifier.classify(testDescriptors[i]);
debug(rs);
        if(abs(rs - testLabels[i]) < 0.00001){
            correct++;
        }
    }
    /*  
    std::vector<float> results = classifier.classifyAll(testDescriptors);
    int correct;
    for(int i = 0; i < testDescriptors.size(); i++){
        if(abs(results[i] - testLabels[i]) < 0.00001){
            correct++;
        }
    }
    */
    return correct;
}
int main(int argc, char** argv) {                                                                                               /*                               
    //@TODO: check argv, argc
      for (int i = -5; i < 16; i = i + 2){
        double c = pow(2, i);
        for (int j = -19; j < 4; j = j + 2){
            double g = pow(2, j);
            int correct = execute(c, g, argv[1], argv[2]);
std::cerr<<"C = "<<c<<" gamma = "<< g << " result = "<<correct<< std::endl;
        }
    }
    double c = 8;
    //g = 0.0005 0.00195312 0.005
    double minGama = 0.0009;
    for(int i = 1; i < 100; i++){
        double g = minGama + 0.00005*i;
        int correct = execute(c, g, argv[1], argv[2]);
std::cout<<"C = "<<c<<" gamma = "<< g << " result = "<<correct<< std::endl;
    }
*/

    for (int i = 0; i < 7; i++ ){
        double c = 2 + i;
        for (int j = 0; j < 15; j++ ){
            double g = 0.0004 + 0.0002*j; 
            struct timeval tim;
            gettimeofday(&tim, NULL);
            double t1=tim.tv_sec+(tim.tv_usec/1000000.0);
            int correct = execute(c, g, argv[1], argv[2]);
            gettimeofday(&tim, NULL);
            double t2=tim.tv_sec+(tim.tv_usec/1000000.0);
//std::cerr<<"C = "<<c<<" gamma = "<< g << " result = "<<correct<< std::endl;
std::cerr<<c<<","<< g << ","<<correct<<","<<t2 - t1<< std::endl;
        }
    }
/*  
    char *ptr;
    double c = strtod(argv[3], &ptr);
    double gamma = strtod(argv[4], &ptr); 
    int correct = execute(c, gamma, argv[1], argv[2]);
std::cout<<"C = "<<c<<" gamma = "<< gamma << " result = "<<correct<< std::endl;
*/
    return 0;
}

