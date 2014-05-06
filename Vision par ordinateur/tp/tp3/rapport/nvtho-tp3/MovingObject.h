#ifndef MOVING_OBJECT_H
#define MOVING_OBJECT_H

#include <vector>
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/nonfree/features2d.hpp"

/**
 * Tracking objects
 * using keypoints to detect objects
 * and KalmanFilter to tracking objects's velocity and position 
 */
const int maxVelocity = 3.0;
class MovingObject{
    public:
        /**
         * Constructor
         *
         */
        MovingObject(cv::Point ip, cv::Mat initialDescriptor);
        void process(cv::Point2f measurementPosition);
        void updateWithoutCorrectrion();

        void draw(cv::Mat &mat);
        cv::Point getEstimatedPosition();
        cv::Point getMeasurement();
        cv::Point getPrediction();

        void setPrediction(cv::Mat pred);
        void setMeasurement(cv::Point meas);
        void setEstimated(cv::Mat est);

        //public variable difination
        cv::Scalar predictionColor;        //color for display the predict point
        cv::Scalar measurementColor;    //color for display the measurement point
        cv::Scalar estimatedColor;          //color for display the state point
        cv::Mat_<float> measurement = cv::Mat_<float>(2,1); 
        cv::Point center;
        cv::Mat descriptor; //track this descriptor
        int notFoundCount = 0;
        int noMeasurement = 0;
        int found = 0;

//        cv::RNG rng;
    private:
        cv::KalmanFilter filter;
        //cv::Mat measurement = cv::Mat::zeros(2, 1, CV_32F);
        cv::Mat estimated = cv::Mat(4, 1, CV_32F);
        cv::Mat prediction = cv::Mat(4, 1, CV_32F);
        cv::Mat processNoise = cv::Mat(4, 1, CV_32F);
        cv::Point2f prevMeasurement;
        cv::Point2f prevEstimated;
        cv::Point2f prevPrediction;

};
#endif
