#ifndef LEARNING_H 
#define LEARNING_H 
 
#include <iostream>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

class Learning 
{
    public:
        Learning(int s);
        cv::Mat getHistogramSkin();
        cv::Mat getHistogramNonSkin();
//        virtual ~Learning();
	private:
        int scale;
        std::vector<double> histogramSkin;
        std::vector<double> histogramNonSkin;
        cv::Mat imageHistogramSkin;
        cv::Mat imageHistogramNonSkin;
        int totalPixelSkin;
        int totalPixelNonSkin;
        void getHistogram(char *file, int isSkinExample);
        std::vector<std::string> getExamples(const char *dir);                
        void saveHistogramAsImage(int isHistogramSkin);
        int isImageFile(char *f);
        bool histogramTest(char *file);
};
#endif
