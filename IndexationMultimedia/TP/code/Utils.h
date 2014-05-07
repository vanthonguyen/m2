/*
 * Utils.h
 *
 *  Created on: Dec 9, 2013
 *      Author: thanhkm
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "math.h"

#define ROWS 3
#define COLS 10
#define DIM ROWS*COLS

class Utils {
public:
	// get the header desciptors of an image input
	static std::vector<double> getDescriptors(cv::Mat image);
	// get the header of document with the parameter of ratio
	static cv::Mat getHeader(cv::Mat image, double ratio);
	static std::vector<double> getDescription(cv::Mat image, int nbXCut, int nbYCut);
	static double distance(std::vector<double> des1, std::vector<double> des2);
	static double distanceAbs(std::vector<double> des1, std::vector<double> des2);
	static void printDescriptor(std::vector<double> vec);
	static void printListDescriptor(std::vector<std::vector<double>> vec);
private:
	static void eraseBorder(cv::Mat image, int sizeOfEraseLine);
};


#endif /* UTILS_H_ */
