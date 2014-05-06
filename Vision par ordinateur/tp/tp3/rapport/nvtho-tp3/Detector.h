
#ifndef DETECTOR_H 
#define DETECTOR_H 
 
#include <list>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "BackgroundExtractor.h"

class Detector 
{
    public:
        Detector(char * fileName);
        void begin();
//        virtual ~Detector();
	private:
        char * videoFile;
        cv::VideoCapture capture;
        cv::Mat background;
        cv::Mat foreground;
        cv::Mat frame;
        BackgroundExtractor backgroundSubtructor;
};
#endif

