/*
 * =====================================================================================
 *
 *       Filename:  Clustering.cpp
 *
 *    Description:  
 *        Version:  1.0
 *        Created:  12/12/2013 04:38:27 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  NGUYEN Van Tho (), thonv133@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "Clustering.h"

//return vector average
std::vector<double> 
Clustering::getMeanVector(const std::vector<std::vector<double>> &descriptors){
    std::vector<double> meanVector;

    //return an empty vector if input is empty
    if(descriptors.empty()){
        return meanVector;
    }
    int numberOfExample = descriptors.size();
    for(int j = 0; j < DIM; j++){
        double element = 0.0;
        for(int i = 0; i < numberOfExample; i++){
            element += descriptors[i][j]; 
        }
        meanVector.push_back(element/numberOfExample);
    }
    return meanVector;
}

std::vector<std::vector<double>> 
Clustering::cluster(const std::vector<std::vector<double>> &descriptors, int k){
    std::vector<std::vector<double>> centroids;

    //return an empty vector if input is empty
    if(descriptors.empty()){
        return centroids;
    }
    std::vector<double> desc = descriptors[0];

    cv::Mat in( descriptors.size() , 1, CV_32FC(DIM));

//    std::cout<<desc.size()<<std::endl;
    for(uint i = 0; i < descriptors.size(); i++){
        cv::Vec<float, DIM> vec;
        for(int j = 0; j < DIM; j++){
            vec.val[j] = descriptors[i][j];
        }
        in.at<cv::Vec<float, DIM>>(i) = vec; 
    }

    cv::Mat bestLabels;
    cv::Mat centers(k, 1, in.type());
    cv::kmeans(in, k, bestLabels, cv::TermCriteria( CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 100, 0.00001),
               30, cv::KMEANS_PP_CENTERS, centers);
    //cvMat to vector
    for(int i = 0; i < centers.rows; i++){
        cv::Vec<float, DIM> vec = centers.at<cv::Vec<float, DIM>>(i);
        std::vector<double> c;
        for(int j = 0; j < DIM; j++){
            c.push_back((double)vec.val[j]);
        }
        centroids.push_back(c);
    }

    return centroids;
}

std::vector<std::vector<double>>
Clustering::iterativeCluster(const std::vector<std::vector<double>> &descriptors, double thresholdDistance){
	std::vector<std::vector<double>> centroids;
	std::vector<int> labels;
	labels = iterativeClustering(descriptors, thresholdDistance);

	std::vector<int>::const_iterator it = max_element(labels.begin(), labels.end());
//	"number of cluster: " = (*it + 1)
	for (int j = 0; j < *it + 1; j++){
		std::vector<std::vector<double>> centers;
		int count = 0;
		for (uint i = 0; i < descriptors.size(); i++){
			if (labels[i]==j){
				centers.push_back(descriptors[i]);
				count++;
			}
		}
		if (count >= 1){
			centroids.push_back(getMeanVector(centers));
		} else {
			std::cout << "number of instance: " << count << std::endl;
		}
	}
	return centroids;
}


std::vector<int>
Clustering::iterativeClustering(const std::vector<std::vector<double>> &descriptors, double thresholdDistance){
	std::vector<int> labels;
	// if empty return empty
	if (descriptors.empty()){
		return labels;
	}

	labels.push_back(0);
	int numberCluster = 1;
	double distance;
	for(uint i = 1; i < descriptors.size(); i++){

		// search classes which are classed
		// if the descriptor is enough near, classe it.
		bool isClassed = false;
		for (uint j = 0; j < i; j++){
			distance = Utils::distance(descriptors[i], descriptors[j]);
			if (distance < thresholdDistance){
				labels.push_back(labels[j]);
				isClassed = true;
			}
			if (isClassed){
				break;
			}
		}
		// if not yet classed, create new classe
		if (!isClassed){
			numberCluster++;
			labels.push_back(numberCluster-1);
		}
	}

	return labels;
}

std::vector<int> Clustering::predict(const std::vector<std::vector<double>> &type1_descriptors,
        					 	 const std::vector<std::vector<double>> &type2_descriptors,
        					 	 const std::vector<std::vector<double>> &autre_centroids,
        					 	 const std::vector<std::vector<double>> &descriptors){
	std::vector<int> labels;
	int label = -1;
	for (uint i =0; i < descriptors.size(); i++){
		label = predictOne(type1_descriptors, type2_descriptors, autre_centroids, descriptors[i]);
		labels.push_back(label);
	}

	return labels;
}

int Clustering::predictOne(const std::vector<std::vector<double>> &type1_descriptors,
        					 	 const std::vector<std::vector<double>> &type2_descriptors,
        					 	 const std::vector<std::vector<double>> &autre_centroids,
        					 	 const std::vector<double> &descriptor){
	double bestDistance = 10000, distance = 10000;
	int label = -1;

	for (uint i =0; i < type1_descriptors.size(); i++){
		distance = Utils::distance(descriptor, type1_descriptors[i]);
		if (bestDistance >= distance){
			bestDistance = distance;
		}
		if (bestDistance < THRESH){
			return 1; // if matched return class 1
		}
	}

	bestDistance = 10000;
	for (uint i = 0; i < type2_descriptors.size(); i++) {
		distance = Utils::distance(descriptor, type2_descriptors[i]);
		if (bestDistance >= distance) {
			bestDistance = distance;
		}
		if (bestDistance < THRESH){
			return 2; // if matched return class 2
		}
	}

	bestDistance = 10000;
	for (uint i = 0; i < autre_centroids.size(); i++) {
		distance = Utils::distance(descriptor, autre_centroids[i]);
		if (bestDistance >= distance) {
			bestDistance = distance;
			label = 3 + i;
		}
	}

	if (bestDistance > 0.7) {
		label = -1;
	}

	return label;
}

