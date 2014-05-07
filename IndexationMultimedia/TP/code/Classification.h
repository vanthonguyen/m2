/*
 * Classification.h
 *
 *  Created on: Dec 12, 2013
 *      Author: thanhkm
 */

#ifndef CLASSIFICATION_H_
#define CLASSIFICATION_H_

#include "Utils.h"

class Classification {
public:
	Classification();
	virtual ~Classification();
	static std::vector<double> mul(std::vector<double> des, double a);
	static std::vector<double> sum(std::vector<double> des, double a);
	static std::vector<double> sum(std::vector<double> des1, std::vector<double> des2);
	static std::vector<double> sub(std::vector<double> des1, std::vector<double> des2);
	// return a vector of center
	int cluster(std::vector<std::vector<double>> vecDes,
			std::vector<std::vector<double>> centers, std::vector<int> indices);
};

#endif /* CLASSIFICATION_H_ */
