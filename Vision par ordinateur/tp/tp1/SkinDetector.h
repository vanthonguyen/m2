#ifndef SKINDETECTOR_H 
#define SKINDETECTOR_H 
 
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "Learning.h"

class SkinDetector 
{
    public:
        SkinDetector(int s, char *file);
        void detect();
        void save();
        //virtual ~Learning();
	private:
        int scale;
        char *imageName;
        float possibilityOfSkin;
        float possibilityOfNonSkin;
        cv::Mat histogramSkin; 
        cv::Mat histogramNonSkin; 
        Learning learning;
        cv::Mat image;
        int getPosibilityOfSkin();
        int getPosibilityOfNonSkin();
};
#endif
