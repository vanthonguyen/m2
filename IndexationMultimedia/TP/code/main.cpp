/*
 * main.cpp
 *
 *  Created on: Dec 11, 2013
 *      Author: thanhkm, tho
 */

#include "IO.h"
#include "Clustering.h"
#include "Utils.h"
#include <sys/time.h>

#define TYPE1_DESCRIPTOR "type1.txt"
#define TYPE2_DESCRIPTOR "type2.txt"
#define AUTRE_DESCRIPTOR "autre.txt"
#define TYPE1_FOLDER "type1/"
#define TYPE2_FOLDER "type2/"
#define AUTRE_FOLDER "autre/"
#define ALL_DATA_FOLDER "data/"

void experiment(	std::vector<std::string> listFile1Train,
					std::vector<std::string> listFile2Train,
					std::vector<std::string> listFile3Train,
					std::vector<std::string> listFileValidation,
					int numberCluster){

	cv::Mat image;
	std::vector<std::vector<double>> type1Des, type2Des, autreDes, autreCentroids;

	//--------learning----------------------
	//--------------------------------------
	// read images of type 1 in folder and get list descriptors
	for(uint i = 0; i < listFile1Train.size(); i++){
		std::string fileName = TYPE1_FOLDER + listFile1Train[i];
		image = cv::imread(fileName, CV_LOAD_IMAGE_COLOR);
		type1Des.push_back(Utils::getDescriptors(image));
	}

	// read images of type 2 in folder and get list descriptors
	for(uint i = 0; i < listFile2Train.size(); i++){
		std::string fileName = TYPE2_FOLDER + listFile2Train[i];
		image = cv::imread(fileName, CV_LOAD_IMAGE_COLOR);
		type2Des.push_back(Utils::getDescriptors(image));
	}

	// read images of type autre in folder and get list descriptors
	for(uint i = 0; i < listFile3Train.size(); i++){
		std::string fileName = AUTRE_FOLDER + listFile3Train[i];
		image = cv::imread(fileName, CV_LOAD_IMAGE_COLOR);
		autreDes.push_back(Utils::getDescriptors(image));
	}
struct timeval tim;
gettimeofday(&tim, NULL);
double t1=tim.tv_sec+(tim.tv_usec/1000000.0);
	autreCentroids = Clustering::cluster(autreDes, numberCluster);
gettimeofday(&tim, NULL);
double t2=tim.tv_sec+(tim.tv_usec/1000000.0);
std::cout <<"learning time : " << t2 - t1 << "s" << std::endl;

	//--------Validation----------------------
	//----------------------------------------
	// list descriptors of image for classification
	std::vector<std::vector<double>> listValidationDes;
	// read image for recognition
	for (uint i = 0; i < listFileValidation.size(); i++) {
		std::string fileName = ALL_DATA_FOLDER  + listFileValidation[i];
		image = cv::imread(fileName, CV_LOAD_IMAGE_COLOR);
		listValidationDes.push_back(Utils::getDescriptors(image));
	}

gettimeofday(&tim, NULL);
t2=tim.tv_sec+(tim.tv_usec/1000000.0);
	// classification
	std::vector<int> labels = Clustering::predict(type1Des, type2Des, autreCentroids, listValidationDes);
gettimeofday(&tim, NULL);
double t3=tim.tv_sec+(tim.tv_usec/1000000.0);
std::cout <<"predict time : " << t3 - t2 << "s" << std::endl;

	// print result
	std::vector<int>::const_iterator it = max_element(labels.begin(), labels.end());
	std::cout <<"number of cluster: " << *it + 1 << std::endl;
	for (int j = -1; j < *it + 1; j++){
		for (uint i = 0; i < listFileValidation.size(); i++){
			if (labels[i]==j){
				std::cout << listFileValidation[i] << " " << labels[i] << std::endl;
			}
		}
	}

}

int main(int argc, char** argv){
	int numberCluster = 3;
	for (int i = 1; i < argc; i++) {
		const char* s = argv[i];
		if (strcmp(s, "-k") == 0) {
			numberCluster = atoi(argv[++i]);
		} else {
			std::cout << "not recognize command" << std::endl;
			return -1;
		}
	}

	std::vector<std::string> listFile1, listFile2, listFile3;
	listFile1 = IO::getFilesInFolder(TYPE1_FOLDER);
	listFile2 = IO::getFilesInFolder(TYPE2_FOLDER);
	listFile3 = IO::getFilesInFolder(AUTRE_FOLDER);

    //shuffle vectors
    std::random_shuffle(listFile1.begin(), listFile1.end());
    std::random_shuffle(listFile2.begin(), listFile2.end());
    std::random_shuffle(listFile3.begin(), listFile3.end());

    unsigned int s1 = listFile1.size()/3;
    unsigned int s2 = listFile2.size()/3;
    unsigned int s3 = listFile3.size()/3;
std::cout << s1 << " " << s2 << " " << s3 << std::endl;

    for(int i = 0; i < 3; i++){
    	std::vector<std::string> listFile1Train(listFile1);
    	std::vector<std::string> listFile2Train(listFile2);
    	std::vector<std::string> listFile3Train(listFile3);
    	std::vector<std::string> listFileValidation;

		if (i != 2) {
			listFileValidation.insert(listFileValidation.end(),
					listFile1Train.begin() + s1 * i,
					listFile1Train.begin() + s1 * (i + 1));
			listFileValidation.insert(listFileValidation.end(),
					listFile2Train.begin() + s2 * i,
					listFile2Train.begin() + s2 * (i + 1));
			listFileValidation.insert(listFileValidation.end(),
					listFile3Train.begin() + s3 * i,
					listFile3Train.begin() + s3 * (i + 1));

			listFile1Train.erase(listFile1Train.begin() + s1 * i,
					listFile1Train.begin() + s1 * (i + 1));
			listFile2Train.erase(listFile2Train.begin() + s2 * i,
					listFile2Train.begin() + s2 * (i + 1));
			listFile3Train.erase(listFile3Train.begin() + s3 * i,
					listFile3Train.begin() + s3 * (i + 1));

			experiment(listFile1Train, listFile2Train,
					listFile3Train, listFileValidation, numberCluster);
		} else {
			listFileValidation.insert(listFileValidation.end(),
					listFile1Train.begin() + s1 * i, listFile1Train.end());
			listFileValidation.insert(listFileValidation.end(),
					listFile2Train.begin() + s2 * i, listFile2Train.end());
			listFileValidation.insert(listFileValidation.end(),
					listFile3Train.begin() + s3 * i, listFile3Train.end());

			listFile1Train.erase(listFile1Train.begin() + s1 * i,
					listFile1Train.end());
			listFile2Train.erase(listFile2Train.begin() + s2 * i,
					listFile2Train.end());
			listFile3Train.erase(listFile3Train.begin() + s3 * i,
					listFile3Train.end());

			experiment(listFile1Train, listFile2Train,
					listFile3Train,	listFileValidation, numberCluster);

		}
    }

	cv::waitKey(0);
	return 0;
}

int maina(int argc, char** argv){
	bool isLearning = false;
	int numberCluster = 3;
	double threshold = 0.3;
	std::string folderRecognition = "test/";
	for (int i = 1; i < argc; i++) {
		const char* s = argv[i];
		if (strcmp(s, "-learn") == 0) {
			isLearning = true;
		} else if (strcmp(s, "-folder") == 0) {
			 folderRecognition = std::string(argv[++i]);
		} else if (strcmp(s, "-k") == 0) {
			numberCluster = atoi(argv[++i]);
		} else if (strcmp(s, "-thresh") == 0) {
			threshold = strtod(argv[++i], NULL);
		} else {
			std::cout << "not recognize command" << std::endl;
			return -1;
		}
	}

	std::vector<std::vector<double>> type1Des, type2Des, autreDes, autreCentroids;
	cv::Mat image, imageResized;
	std::vector<std::string> listFile;

	if (isLearning){
		// read images of type 1 in folder and get list descriptors
		listFile = IO::getFilesInFolder(TYPE1_FOLDER);
		for(uint i = 0; i < listFile.size(); i++){
			std::string fileName = TYPE1_FOLDER + listFile[i];
			image = cv::imread(fileName, CV_LOAD_IMAGE_COLOR);
			type1Des.push_back(Utils::getDescriptors(image));
		}

		listFile.clear();
		// read images of type 2 in folder and get list descriptors
		listFile = IO::getFilesInFolder(TYPE2_FOLDER);
		for(uint i = 0; i < listFile.size(); i++){
			std::string fileName = TYPE2_FOLDER + listFile[i];
			image = cv::imread(fileName, CV_LOAD_IMAGE_COLOR);
			type2Des.push_back(Utils::getDescriptors(image));
		}

		listFile.clear();
		// read images of type autre in folder and get list descriptors
		listFile = IO::getFilesInFolder(AUTRE_FOLDER);
		for(uint i = 0; i < listFile.size(); i++){
			std::string fileName = AUTRE_FOLDER + listFile[i];
			image = cv::imread(fileName, CV_LOAD_IMAGE_COLOR);
			autreDes.push_back(Utils::getDescriptors(image));
		}
		autreCentroids = Clustering::cluster(autreDes, numberCluster);
//		autreCentroids = Clustering::iterativeCluster(autreDes, threshold);

		// write descriptor to disk
		IO::writeDescriptors(type1Des, TYPE1_DESCRIPTOR);
		IO::writeDescriptors(type2Des, TYPE2_DESCRIPTOR);
		IO::writeDescriptors(autreCentroids, AUTRE_DESCRIPTOR);

	} else {
		type1Des = IO::getDescriptors(TYPE1_DESCRIPTOR);
		type2Des = IO::getDescriptors(TYPE2_DESCRIPTOR);
		autreCentroids = IO::getDescriptors(AUTRE_DESCRIPTOR);

		// list descriptors of image for classification
		std::vector<std::vector<double>> listDescriptors;
		// read image for recognition
		listFile = IO::getFilesInFolder(folderRecognition.c_str());

		for (uint i = 0; i < listFile.size(); i++) {
			std::string fileName = folderRecognition + listFile[i];
			image = cv::imread(fileName, CV_LOAD_IMAGE_COLOR);
			cv::resize(image, imageResized, cv::Size(), 0.1, 0.1);
			listDescriptors.push_back(Utils::getDescriptors(image));
		}

struct timeval tim;
gettimeofday(&tim, NULL);
double t1=tim.tv_sec+(tim.tv_usec/1000000.0);
		// classification
		std::vector<int> labels = Clustering::predict(type1Des, type2Des, autreCentroids, listDescriptors);
gettimeofday(&tim, NULL);
double t2=tim.tv_sec+(tim.tv_usec/1000000.0);
std::cout <<"time: " << t2 - t1 << " s" << std::endl;

//		// interative classification
//		std::vector<int> labels = Clustering::iterativeClustering(listDescriptors, threshold);

		// print result
		std::vector<int>::const_iterator it = max_element(labels.begin(), labels.end());
		std::cout <<"number of cluster: " << *it + 1 << std::endl;
		for (int j = -1; j < *it + 1; j++){
			for (uint i = 0; i < listFile.size(); i++){
				if (labels[i]==j){
					std::cout << listFile[i] << " " << labels[i] << std::endl;
				}
			}
		}

//		// write result
//		std::cout <<"number of cluster: " << *it + 1 << std::endl;
//		for (int j = -1; j < *it + 1; j++){
//			for (uint i = 0; i < listFile.size(); i++){
//				if (labels[i]==j){
//					std::string fileName = folderRecognition + listFile[i];
//					image = cv::imread(fileName, CV_LOAD_IMAGE_COLOR);
//					std::ostringstream convert;   // stream used for the conversion
//					convert << labels[i];
//					fileName = folderRecognition + convert.str() + listFile[i];
//					cv::imwrite(fileName, image);
//				}
//			}
//		}
	}

	cv::waitKey(0);
	return 0;
}

