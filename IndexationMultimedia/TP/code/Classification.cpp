/*
 * Classification.cpp
 *
 *  Created on: Dec 12, 2013
 *      Author: thanhkm
 */

#include "Classification.h"


Classification::Classification() {
	// TODO Auto-generated constructor stub

}

Classification::~Classification() {
	// TODO Auto-generated destructor stub
}

std::vector<double> Classification::mul(std::vector<double> des, double a){
	std::vector<double> rs;
	for (uint i = 0; i < des.size(); i++){
		rs.push_back(des[i] * a);
	}
	return rs;
}

std::vector<double> Classification::sum(std::vector<double> des, double a){
	std::vector<double> rs;
	for (uint i = 0; i < des.size(); i++) {
		rs.push_back(des[i] + a);
	}
	return rs;
}

std::vector<double> Classification::sum(std::vector<double> des1, std::vector<double> des2){
	std::vector<double> rs;
	for (uint i = 0; i < des1.size(); i++) {
		rs.push_back(des1[i] + des2[i]);
	}
	return rs;
}

std::vector<double> Classification::sub(std::vector<double> des1, std::vector<double> des2){
	std::vector<double> rs;
	for (uint i = 0; i < des1.size(); i++) {
		rs.push_back(des1[i] - des2[i]);
	}
	return rs;
}


// return a vector of center
int Classification::cluster(std::vector<std::vector<double>> vecDes,
		std::vector<std::vector<double>> centers, std::vector<int> indices){
	int k = 5;
	int descriptorsDimension = vecDes[0].size();
	int numberDescriptors = vecDes.size();

	// initialize centers
	int idx =0;
	for(int i = 0; i < k; i++){
		idx = rand() % numberDescriptors;
		centers.push_back(vecDes[idx]);
	}

	// loops for calculate the centers
	double err = 100000;
	double alpha = 0.01;
	while (true){
		for (int i = 0; i < numberDescriptors; i++){

			// calculate the nearest center descriptor and modify it
			// for eache descriptor
			int minIdx = -1;
			double minDist = 1000000;
			for (int j = 0; j < k; j++){
				double dist = Utils::distance(vecDes[i], centers[j]);
				if (dist < minDist){
					minDist = dist;
					minIdx = j;
				}
			}
//			std::vector<double> v = mul( );
//			centers[minIdx] = sum( centers[minIdx], mul() )



		}

	}

	return 0;
}
