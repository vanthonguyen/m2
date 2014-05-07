/*
 * =====================================================================================
 *
 *       Filename:  Recognise.cpp
 *    Description:  
 *        Version:  1.0
 *        Created:  12/13/2013 11:07:17 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  NGUYEN Van Tho (), thonv133@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "Clustering.h"
#include "Utils.h"
#include "IO.h"

int main(int argc, char** argv) {
    IO io();
    std::vector<std::vector<double>> descriptors = io.getDescriptors(); 
    Recognise recognise(descriptors);
    char *imageName = argv[1];

    cv::Mat im = cv::imread(imageName, CV_LOAD_IMAGE_UNCHANGED);
    std::vector<double> descriptorOfNewImage = Utils::getDescriptors(im);
    int type = getType(descs);    
}


Recognise::Recognise(std::vector<std::vector<double>> descs): descriptors(descs){
}

int Recognise::getType(const std::vector<double> &desc){
    double d1 = Utils::distance(desc, descriptors[0]);
    double d2 = Utils::distance(desc, descriptors[1]);

    if( d1 < d2 && d1 < THRESHOLD){
        return 0;
    }

    if( d2 < d1 && d2 < THRESHOLD){
        return 1;
    }
    
    int position = 1000000;
    double minDistance = 1000000;
    for (int = 2; i < descriptors.size(); i++){
        double dist = Utils::distance(desc, descriptors[i]);    
        if( dist < minDistance){
            minDistance = dist;
            position = i;
        }
    }
    if(minDistance > THRESHOLD){
        return -1;
    }

    return position;
}
