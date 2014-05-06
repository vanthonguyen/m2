#ifndef BACKGROUND_EXTRACTOR_H
#define BACKGROUND_EXTRACTOR_H 
 
#include <list>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

const unsigned int maxNumberOfBackground = 90;
const unsigned int maxNumberOfHistoryFrame = 50;
const unsigned int threshold = 30;
const int sampleRate = 10;
class BackgroundExtractor 
{
    public:
        BackgroundExtractor();
        cv::Mat getBackground(cv::Mat frame);
        static cv::Mat subtract(cv::Mat m1, cv::Mat m2);
//        virtual ~Detector();
	private:
        unsigned int numberOfHistoryFrame = 0;
        const unsigned int minNumberOfHistoryFrame = 2;
        cv::Mat background;
        std::list<cv::Mat> history;
        std::list<cv::Mat> backgroundHistory;
};
#endif

