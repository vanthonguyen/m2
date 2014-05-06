#ifndef TRACKING_H
#define TRACKING_H

#include <vector>
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/nonfree/features2d.hpp"

#include "MovingObject.h"
#include "BackgroundExtractor.h"
/**
 * Tracking objects
 * using keypoints to detect objects
 * and KalmanFilter to tracking objects's velocity and position 
 */

const double minDistance = 0.81;
const int minHessian = 400; 
const double minContourArea = 290;
const double minScore = 0.45;

class Tracking{
    public:
        /**
         * Constructor
         *
         */
        Tracking(char *file);
        void begin();
    private:
        char *videoFile;
        cv::Mat background;
        cv::Mat foreground;
        cv::Mat tracker;
        std::vector<MovingObject> objects;
        cv::Mat frame;
        cv::Mat thresholdFrame;
        std::vector<cv::KeyPoint> keyPoints;
        cv::VideoCapture capture;
        BackgroundExtractor backgroundSubtructor;
        cv::SurfDescriptorExtractor extractor;
        cv::SurfFeatureDetector detector = cv::SurfFeatureDetector(minHessian);
        std::vector<cv::Mat> descriptors;    
        cv::BFMatcher matcher;

        int sampleRate = 10;
        bool initialised = false;

};
#endif

