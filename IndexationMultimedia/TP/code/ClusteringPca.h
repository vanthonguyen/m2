#ifndef CLUSTERING_PCA
#define CLUSTERING_PCA

#include <vector>

#include "LearningPca.h"

class ClusteringPca: public LearningPca {
    public:
        ClusteringPca(int nComponent = 0);       
        void train(const std::vector<cv::Mat> &images, int k);
        int predict(const cv::Mat &image);
    private:
        std::vector<int> labels; 
        cv::Mat centroids; 
};
#endif
