#ifndef IO_H
#define IO_H

#include<iostream>
#include<vector>
#include <fstream>

#include <opencv2/opencv.hpp>                                                                                                                                   
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#define CONTOUR_PIXEL_VALUE 1
#define NORMAL_PIXEL_VALUE 1
#define IMAGE_WIDTH 28
#define IMAGE_HEIGHT 28
#define IMAGE_SIZE 28*28
#define CONTOUR_TEXTE_VALUE '+'
#define NORMAL_TEXTE_VALUE '#'

#define W 7

class IO{
    public:
        IO(bool isNew, char *fileName);  
        ~IO();
        std::vector<std::vector<int> > getDescriptors();                                                                                                      
        static std::vector<std::vector<int> > getDescriptors(char *fileName);
        static std::vector<float> getLabels(char *labelFile);
        void writeDescriptor(std::vector<int> descriptor);
    private:
        std::vector<std::vector<int> > descriptors;
        void readFile();
        std::fstream writer;
};

#endif
