/*
 * =====================================================================================
 *
 *       Filename:  Tracking.cpp
 *
 *    Description:  Tracking objects in video using Kalman filter 
 *
 *        Version:  1.0
 *        Created:  11/23/2013 10:47:42 AM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  NGUYEN Van Tho (), thonv133@gmail.com
 *   Organization:  IFI 
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "MovingObject.h"


#define debug(text)\
    std::cout<<text<<std::endl;

MovingObject::MovingObject(cv::Point initialPoint, cv::Mat initialDescriptor): 
    center(initialPoint), descriptor(initialDescriptor){
    /**
     * 4 dynamic params: x, y, dx, dy, 2 measurement
     */
    filter.init(4, 2, 0);
    filter.transitionMatrix = *(cv::Mat_<float>(4, 4) << 1,0,1,0,   0,1,0,1,  0,0,1,0,  0,0,0,1);
    filter.processNoiseCov = *(cv::Mat_<float>(4,4) << 0.1,0,0.1,0,  0,0.1,0,0.1,  0,0,0.1,0,  0,0,0,0.1);
    cv::setIdentity(filter.measurementMatrix);
    cv::setIdentity(filter.processNoiseCov, cv::Scalar::all(1e-5));
    cv::setIdentity(filter.measurementNoiseCov, cv::Scalar::all(1e-1));
    cv::setIdentity(filter.errorCovPost, cv::Scalar::all(1));

    filter.statePost = *(cv::Mat_<float>(4, 1) << center.x,center.y,0,0);
    cv::randn( estimated, cv::Scalar::all(0), cv::Scalar::all(0.1) );

    srand(time(NULL));
    int r = rand()%256;
    int g = rand()%256;
    int b = rand()%256;
    //predictionColor = cv::Scalar(b, g, r);
    //measurementColor = cv::Scalar(b, g, r);
    estimatedColor= cv::Scalar(b, g, r);
//    predictionColor = cv::Scalar(255, 0, 0);
//    measurementColor = cv::Scalar(0, 255, 0);
//    estimatedColor= cv::Scalar(0, 0, 255);
    prevMeasurement.x = -1.0;
    prevMeasurement.y = -1.0;
    measurement(0) = -1.0;
    measurement(1) = -1.0;

}

void MovingObject::process(cv::Point2f measurementPosition){
    setPrediction(filter.predict());
    //first time
    if(measurement(0) == -1.0 && measurement(1) == -1.0){
        setMeasurement(measurementPosition);
        setEstimated(filter.correct(measurement));
        found++;        
    }else 
    if(abs(measurement(0) - measurementPosition.x) <= noMeasurement*maxVelocity &&
       abs(measurement(1) - measurementPosition.y) <= noMeasurement*maxVelocity){
        setMeasurement(measurementPosition);
        setEstimated(filter.correct(measurement));
        found++;        
        noMeasurement = 0;
    }else{
        updateWithoutCorrectrion();
    }
}
void MovingObject::updateWithoutCorrectrion(){
    setPrediction(filter.predict());
    noMeasurement++;
}

void MovingObject::draw(cv::Mat &img){
    cv::line(img, prevMeasurement, getMeasurement(), measurementColor);
    cv::line(img, prevPrediction, getPrediction(), predictionColor);
    cv::line(img, prevEstimated, getEstimatedPosition(), estimatedColor);

}

cv::Point MovingObject::getEstimatedPosition(){
    cv::Point estimatedPt(estimated.at<float>(0), estimated.at<float>(1));
    return estimatedPt; 
}

cv::Point MovingObject::getPrediction(){
    cv::Point predictPt(prediction.at<float>(0), prediction.at<float>(1));
    return predictPt; 
}
cv::Point MovingObject::getMeasurement(){
    cv::Point measurementPt(measurement(0),measurement(1)); 
    return measurementPt; 
}

void MovingObject::setEstimated(cv::Mat est){
    prevEstimated.x = estimated.at<float>(0);
    prevEstimated.y = estimated.at<float>(1);
    estimated = est;
}
void MovingObject::setPrediction(cv::Mat pred){
    prevPrediction.x = prediction.at<float>(0);
    prevPrediction.y = prediction.at<float>(1);
    prediction = pred;
}
void MovingObject::setMeasurement(cv::Point meas){
    prevMeasurement.x = measurement(0);
    prevMeasurement.y = measurement(1);
    measurement (0) = meas.x;
    measurement (1) = meas.y;
}
