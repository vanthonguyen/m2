#ifndef CLASSIFY_PCA_H
#define CLASSIFY_PCA_H 

#include <vector>

#include "LearningPca.h"

class ClassifyPca: public LearningPca {
    public:
        ClassifyPca(int nComponent = 0);       
        void train(const std::vector<cv::Mat> &images, const std::vector<int> &labels);
        int predict(const cv::Mat &image);
    private:
        std::vector<int> labels; 
};
#endif
