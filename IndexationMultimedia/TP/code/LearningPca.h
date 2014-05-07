#ifndef LEARNING_PCA_H
#define LEARNING_PCA_H

#include <vector>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "Pca.h"

class LearningPca{
    public:
        LearningPca(int nComponent = 0);
        void train(const std::vector<cv::Mat> &images);
        virtual int predict(const cv::Mat &image);
    protected:
        Pca pca;
        cv::Mat trainData;
        int nComponent;
};
#endif
