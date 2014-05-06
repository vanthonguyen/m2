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

#include "Tracking.h"

// plot points
#define drawCross(img, center, color, d )                               \
    line( img, cv::Point( center.x - d, center.y - d ),                 \
        cv::Point( center.x + d, center.y + d ), color, 1, CV_AA, 0);   \
    line( img, cv::Point( center.x + d, center.y - d ),                 \
        cv::Point( center.x - d, center.y + d ), color, 1, CV_AA, 0 )
#define drawCircle(img, center, color, r)\
    cv::circle(img, center, r, color)
#define drawRectangle(img, center, color, d)\
    cv::rectangle(img, cv::Point(center.x -d , center.y -d), cv::Point(center.x +d , center.y +d), color)
#define debug(text)\
    std::cout<<text<<std::endl;

cv::Mat imageMultiply(cv::Mat img1, cv::Mat img2){
    cv::Mat r = cv::Mat::zeros(img1.size(), img1.type());
    for(int row = 0; row < img1.size().height; row++){
        for(int col = 0; col < img1.size().width; col++){
            r.at<uchar>(row,col) = img1.at<uchar>(row,col)*img2.at<uchar>(row,col);
        }
    }
    return r;
}

int main(int argc, char ** argv){
    Tracking tracking(argv[1]);        
    tracking.begin();
}
Tracking::Tracking(char *file): videoFile(file), capture(videoFile){
    
}

void Tracking::begin(){
    if(!capture.isOpened()){
        std::cerr << "Problem opening video source" << std::endl;
    }

    std::vector<cv::KeyPoint> newKeyPoints;
    int nf = 0;
    while((char)cv::waitKey(20) != 'q' && capture.grab()){
        capture.retrieve(frame);
        cv::Mat gray;
        cv::cvtColor(frame, gray, CV_BGR2GRAY);
        if(background.size().width == 0){
            background = gray.clone();
        }

        int rnd = rand()%sampleRate;
        if(rnd == 0){
            background = backgroundSubtructor.getBackground(gray);
        }
        //skip 20 first frames
        if(nf < 20){
            nf++;
            continue;
        }

        foreground = backgroundSubtructor.subtract(gray, background );
        cv::erode(foreground, foreground, cv::Mat());
        cv::dilate(foreground, foreground, cv::Mat());
        cv::dilate(foreground, foreground, cv::Mat());
        cv::erode(foreground, foreground, cv::Mat());


        cv::imshow("xx", background);

        std::vector<std::vector<cv::Point> > newContours;
        cv::findContours(foreground, newContours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
        std::vector<cv::Mat> movingRegions;

        std::vector<std::vector<cv::Point> > contours;
        for(size_t i = 0; i < newContours.size(); i++){
            if(cv::contourArea(newContours[i]) > minContourArea){
                cv::Mat region = cv::Mat::zeros(frame.size(), CV_8UC1);
                cv::drawContours(region, newContours, i, cv::Scalar::all(1), CV_FILLED, 8, std::vector<cv::Vec4i>(), 0, cv::Point());
                movingRegions.push_back(imageMultiply(gray, region));
                contours.push_back(newContours[i]);
            }
        }

        cv::drawContours(frame, newContours, -1, cv::Scalar(0,0,255), 1);
        std::vector<cv::Moments> mu(contours.size() );                                                                                                          
        for( size_t i = 0; i < contours.size(); i++ ){
            mu[i] = cv::moments( contours[i], false );
        }
        
        std::vector<cv::Point2f> newMc( contours.size() );                                                                                                         
        for( size_t i = 0; i < contours.size(); i++ ){ 
            newMc[i] = cv::Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); 
        }

        //if(measurement.at<float>(0) < 0){
        if(!initialised){
            tracker = cv::Mat::zeros(frame.size(), CV_8UC3);
            initialised = true;
        }
        //cv::drawKeypoints(foreground, newKeyPoints, tracker, CV_RGB(0,255,0), cv::DrawMatchesFlags::DEFAULT);
        std::vector<cv::Mat> newDescriptors;
        std::vector<int> keyPointCount;
        std::vector<cv::Point2f> mc;
        for(int i = 0; i < movingRegions.size(); i++){
            cv::Mat des;

            std::vector<cv::KeyPoint> kp; 
            detector.detect(movingRegions[i], kp);
            extractor.compute(frame, kp, des);
            if(des.size().width > 0){
                newDescriptors.push_back(des);
                keyPointCount.push_back(kp.size());
                mc.push_back(newMc[i]);
            }
        }
        if(newDescriptors.size() > 0 && objects.size() == 0){
            for(int i = 0; i < newDescriptors.size(); i++){
                MovingObject obj(mc[i], newDescriptors[i]);
                objects.push_back(obj);
            }
        }else if(newDescriptors.size() > 0 && objects.size() > 0 ){
            for(int objIndex = 0; objIndex < objects.size(); objIndex++){
                double bestMatch = minDistance;
                double score = minScore;
                int found = -1;
                for(int desIndex = 0; desIndex < newDescriptors.size(); desIndex++){
                    cv::vector<cv::DMatch> matches;
                    matcher.match(objects[objIndex].descriptor, newDescriptors[desIndex], matches);
                    int nm = 0;
                    for (int matchIndex = 0; matchIndex < matches.size(); matchIndex++){
                        if(matches[matchIndex].distance < minDistance){
                            nm++; 
                        }
                    }
                    if(nm*1.0 / keyPointCount[desIndex] > score){
                        score = nm*1.0 / keyPointCount[desIndex];
                        found = desIndex;
                    }
                }
                if( found >= 0){
                    objects[objIndex].descriptor = newDescriptors[found];
                    objects[objIndex].process(mc[found]);
                    mc.erase(mc.begin() + found);
                    newDescriptors.erase(newDescriptors.begin() + found);

                    //draw new Point
                    drawCross(tracker, objects[objIndex].getMeasurement(), objects[objIndex].measurementColor, 2);
                    drawCross(tracker, objects[objIndex].getPrediction(), objects[objIndex].predictionColor, 2);
                    drawCross(tracker, objects[objIndex].getEstimatedPosition(), objects[objIndex].estimatedColor, 2);
  //                  objects[objIndex].draw(tracker);
                }else{
                    objects[objIndex].updateWithoutCorrectrion();
                }
                
                if(objects[objIndex].noMeasurement > 50 || objects[objIndex].found == 0){
                    objects.erase(objects.begin() + objIndex);
                }

            }
                //add new objects
            for(int i = 0; i < newDescriptors.size(); i++){
                MovingObject obj(mc[i], newDescriptors[i]);
                objects.push_back(obj);
            }
        }

        cv::imshow("Video", frame);
        cv::imshow("Tracker", tracker);
    }
}
