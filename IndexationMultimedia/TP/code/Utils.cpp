/*
 * Utils.cpp
 *
 *  Created on: Dec 9, 2013
 *      Author: thanhkm
 */

#include "Utils.h"

std::vector<double> Utils::getDescriptors(cv::Mat image){

	// convert to binary image
	float thresh = 170.0;
	cv::Mat imageBinary;
	if (image.type() == CV_8UC3){
		cv::cvtColor(image, imageBinary, CV_BGR2GRAY);
	} else {
		imageBinary = image;
	}
	cv::threshold(imageBinary, imageBinary, thresh, 255, CV_THRESH_BINARY);

	cv::Mat headerImage;
	headerImage = getHeader(imageBinary, 0.21);
	std::vector<double> descriptors = getDescription(headerImage, ROWS, COLS);

	return descriptors;
}

cv::Mat Utils::getHeader(cv::Mat image, double ratio){
	cv::Point2i tl = cv::Point2i(0,0);
	cv::Point2i br = cv::Point2i(image.cols, image.rows * ratio);
	cv::Rect rect(tl, br);
	cv::Mat header = image(rect).clone();
	return header;
}

std::vector<double> Utils::getDescription(cv::Mat image,
												  int nbXCut, int nbYCut) {
	std::vector<double> descriptors;

	double x = image.cols / nbXCut; // width of block
	double y = image.rows / nbYCut; // height of block

	// calculate descriptors
	for (int j = 0; j < nbYCut; j++) {
		for (int i = 0; i < nbXCut; i++) {
			cv::Point2i tl = cv::Point2i(i * x, j * y);
			cv::Point2i br = cv::Point2i((i + 1) * x, (j + 1) * y);
			cv::Rect block(tl, br);
			double levelGray = cv::sum(image(block) / 255).val[0] / (x * y);
			descriptors.push_back(levelGray);
		}
	}

	return descriptors;
}

void Utils::eraseBorder(cv::Mat image, int size) {
	for (int x = 0; x < image.cols; x++) {
		for (int y = 0; y < size; y++) {
			image.at<cv::Vec3b>(y, x) = cv::Vec3b(255, 255, 255);
			image.at<cv::Vec3b>(image.rows - y, x) = cv::Vec3b(255, 255, 255);
		}
	}

	for (int y = 0; y < image.rows; y++) {
		for (int x = 0; x < size; x++) {
			image.at<cv::Vec3b>(y, x) = cv::Vec3b(255, 255, 255);
			image.at<cv::Vec3b>(y, image.cols - x) = cv::Vec3b(255, 255, 255);
		}
	}
}

double Utils::distance(std::vector<double> des1, std::vector<double> des2){
	if (des1.size() != des2.size()) {
		std::cout << " not same size " << std::endl;
		return 1000000;
	}
	// calculate distance Euclide of two descriptors
	double norm = 0;
	for (uint i = 0; i < des1.size(); i++) {
		norm += (des1[i] - des2[i]) * (des1[i] - des2[i]);
	}

	return sqrt(norm);
}

double Utils::distanceAbs(std::vector<double> des1, std::vector<double> des2){
	if (des1.size() != des2.size()) {
		std::cout << " not same size " << std::endl;
		return 1000000;
	}
	// calculate distance Absolute of two descriptors
	double norm = 0;
	for (uint i = 0; i < des1.size(); i++) {
		double a = des1[i] > des2[i]?des1[i] - des2[i]:des2[i] - des1[i];
		norm += a;
	}

	return norm;
}

void Utils::printDescriptor(std::vector<double> vec){
	if (vec.empty()){
		std::cout << "empty" << std::endl;
	} else {
		for (uint i = 0; i < vec.size(); i++){
			std::cout << vec[i] << " ";
		}
		std::cout << std::endl;
	}

}

void Utils::printListDescriptor(std::vector<std::vector<double>> vec){
	if (vec.empty()) {
		std::cout << "empty" << std::endl;
	} else {
		for (uint i = 0; i < vec.size(); i++) {
			printDescriptor(vec[i]);
		}
	}
}



