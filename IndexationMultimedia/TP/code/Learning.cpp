/*
 * =====================================================================================
 *
 *       Filename:  Learning.cpp
 *    Description:  
 *        Version:  1.0
 *        Created:  12/13/2013 10:48:36 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  NGUYEN Van Tho (), thonv133@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "Utils.h"
#include "Clustering.h"
#include "IO.h"

int main(int argc, char** argv) {
    //IO, truncate descriptors file
    IO io(true);
    //get descriptors for type 1
    std::vector<std::string> listFiles = io.getFilesInFolder(argv[1]);
    std::vector<std::vector<double>> descriptors1; 
    for (uint i = 0; i < listFiles.size(); i++){
        cv::Mat im = cv::imread(argv[1] + listFiles[i], CV_LOAD_IMAGE_UNCHANGED);
        descriptors1.push_back(Utils::getDescriptors(im));
    }
    //get mean vector and write to descriptors file
    io.writeDescriptor(Clustering::getMeanVector(descriptors1)); 

    //get descriptors for type 2
    std::vector<std::string> listFiles2 = io.getFilesInFolder(argv[2]);
    std::vector<std::vector<double>> descriptors2; 
    for (uint i = 0; i < listFiles2.size(); i++){
        cv::Mat im = cv::imread(argv[2] + listFiles2[i], CV_LOAD_IMAGE_UNCHANGED);
        descriptors2.push_back(Utils::getDescriptors(im));
    }

    //get mean vector and write to descriptors file
    io.writeDescriptor(Clustering::getMeanVector(descriptors2)); 

    //get descriptors for type other
    std::vector<std::string> listFiles3 = io.getFilesInFolder(argv[3]);
    std::vector<std::vector<double>> descriptors3; 
    for (uint i = 0; i < listFiles3.size(); i++){
        cv::Mat im = cv::imread(argv[3] + listFiles3[i], CV_LOAD_IMAGE_UNCHANGED);
        descriptors3.push_back(Utils::getDescriptors(im));
    }

    //clustering and  write to descriptors file
    std::vector<std::vector<double>> centroids = Clustering::cluster(descriptors3); 
    for(uint i = 0; i < centroids.size(); i++){
        io.writeDescriptor(centroids[i]);
    }
}
