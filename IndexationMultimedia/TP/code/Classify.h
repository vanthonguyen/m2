#ifndef CLASSIFY_H
#define CLASSIFY_H


#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "XYCut.h"

class Classify{
    public:
        //constructor
        Classify(char *imgName);
    private:
        cv::Mat imageOriginal;
        cv::Mat imageGray; 
        XYCut xyCut;
};

#endif
