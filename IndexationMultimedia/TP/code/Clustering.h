#ifndef CLUSTERING_H
#define CLUSTERING_H

#include <vector>
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "Utils.h"

#define DIM 30
#define K 4
#define THRESH 0.3

class Clustering{
    public:
        static std::vector<double> getMeanVector(const std::vector<std::vector<double>> &descriptors);
        static std::vector<std::vector<double>>
        	cluster(const std::vector<std::vector<double>> &descriptors, int nbCluster);
        static std::vector<std::vector<double>>
            iterativeCluster(const std::vector<std::vector<double>> &descriptors, double thresholdDistance);
        static std::vector<int>
            iterativeClustering(const std::vector<std::vector<double>> &descriptors, double thresholdDistance);

        static std::vector<int> predict(const std::vector<std::vector<double>> &type1_descriptors,
        					 	 const std::vector<std::vector<double>> &type2_descriptors,
        					 	 const std::vector<std::vector<double>> &autre_centroids,
        					 	 const std::vector<std::vector<double>> &descriptors);
    private:
        static int predictOne(const std::vector<std::vector<double>> &type1_descriptors,
        					 	 const std::vector<std::vector<double>> &type2_descriptors,
        					 	 const std::vector<std::vector<double>> &autre_centroids,
        					 	 const std::vector<double> &descriptor);
};
#endif
