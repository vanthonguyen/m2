/*
 * =====================================================================================
 *       Filename:  IO.cpp
 *    Description:  
 *        Version:  1.0
 *        Created:  12/11/2013 04:47:53 PM
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  NGUYEN Van Tho (), thonv133@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdlib.h>
#include <unistd.h>
#include "string.h"
#include "stdio.h"
#include <dirent.h>

#include <sstream>

#include "IO.h"


std::vector<std::vector<double>> IO::getDescriptors(const char * fileName){
    std::ifstream reader(fileName);
    std::vector<std::vector<double> > desc;

    while(reader.good()){
        std::string line;
        std::getline(reader, line);
        std::istringstream iss(line);

        std::vector<double> descriptor;
        double d;
        while (iss >> d){
            descriptor.push_back(d);
        }   
        if(!descriptor.empty()){
            desc.push_back(descriptor);
        }   
    }   
    reader.close();
    return desc;
}

void IO::writeDescriptors(const std::vector<std::vector<double>> &descriptors, const char * fileName){
	std::ofstream ofs(fileName);
	for (uint i = 0; i < descriptors.size(); i++){
		std::vector<double> descriptor = descriptors[i];
		if(!descriptor.empty()){
	        std::vector<double>::const_iterator it = descriptor.begin();
	        for(; it != descriptor.end(); it++){
	            if(it != descriptor.end()){
	                ofs<< *it<< " ";
	            }else{
	                ofs<< *it;
	            }
	        }
	        ofs<<std::endl;
	    }
	}
	ofs.close();
}

std::vector<std::string> IO::getFilesInFolder(const char *folderName){
    DIR *dp;
    struct dirent *ep;
    std::vector<std::string> listFile;
 
    dp = opendir (folderName);
    if (dp != NULL){
        while (ep = readdir (dp)){
            if(isImageFile(ep->d_name)){
                listFile.push_back(ep->d_name);
// puts (ep->d_name);
            }
        }
        (void) closedir (dp);
    }else{
        perror ("Couldn't open the directory");
    }

    return listFile;
}

bool IO::isImageFile(char *f){
    if(strstr(f, IMAGE_EXTENSIONS)){
        return true;
    }
    return false;
}
