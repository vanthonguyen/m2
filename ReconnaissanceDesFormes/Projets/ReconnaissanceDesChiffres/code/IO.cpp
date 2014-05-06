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

IO::IO(bool isNew, char *fileName){
    if(isNew){
        writer.open(fileName, std::ios::in | std::ios::out);
    }else{
        writer.open(fileName, std::ios::in | std::ios::out | std::ios::app);
    }
    //readFile(); 
}
IO::~IO(){
    writer.close();
}

std::vector<std::vector<int> > IO::getDescriptors(){
    readFile();
    return descriptors;
}   

void IO::writeDescriptor(std::vector<int> descriptor){
    if(!descriptor.empty()){
        std::vector<int>::const_iterator it = descriptor.begin();
        for(; it != descriptor.end(); it++){
            if(it != descriptor.end()){
                writer<< *it<< " ";
            }else{
                writer<< *it;
            }   
        }   
        writer<<std::endl;
    }   
}   
void IO::readFile(){
    descriptors.clear();
    writer.seekg(0, writer.beg);
    while(writer.good()){
        std::string line;
        std::getline(writer, line);
        std::istringstream iss(line);

//std::cout<<line<<std::endl;
        std::vector<int> descriptor;
        int d;
        while (iss >> d){
	        descriptor.push_back(d);
        }
        if(!descriptor.empty()){
            descriptors.push_back(descriptor);
        }   
    }   
}   

std::vector<std::vector<int> > IO::getDescriptors(char *fileName){
    std::fstream reader;
    reader.open(fileName);
    std::vector<std::vector<int> > desc;

    while(reader.good()){
        std::string line;
        std::getline(reader, line);
        std::istringstream iss(line);

        std::vector<int> descriptor;
        int d;
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

std::vector<float> IO::getLabels(char *labelFile){
    std::vector<float> labels;
    std::ifstream ifs(labelFile);                                                                                                                 
    int type;    
    while(ifs.good())
    {            
        ifs>>type;
        labels.push_back(type);
    }  

    ifs.close();
    return labels;
}
