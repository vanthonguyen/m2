/*
 * =====================================================================================
 *       Filename:  Matching.h
 *    Description: Matching 2 images using SIFT feature detector 
 *        Created:  12/05/2013 11:00:00 AM
 *       Compiler:  g++
 *         Author:  NGUYEN Van Tho (), thonv133@gmail.com
 *   Organization:  IFI 
 *
 * =====================================================================================
 */


#ifndef MATCHING_H
#define MATCHING_H

#include <vector>
#include <iostream>
#include <fstream>

#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/nonfree/features2d.hpp"

//distance 
#define RANSAC_PARAM1 3.0
//confidence propability
#define RANSAC_PARAM2 0.99

//min distance threshold, use when the min distance is too small
//used with heuristic algorithm
#define DISTANCE_THRESHOLD 0.3

const std::string OUTPUT_DIR = "../output/";
const std::string MATCH_DRAW = "2images";
const std::string FINAL_RESULT = "final";
const std::string RANSAC = "ransac";
const std::string HEURISTIC = "heuristic";
const std::string IMAGE_EXTENSION = ".jpg";
const std::string TEXT_OUTPUT = "text";
/* *
 * Using SIFT descriptor for matching 2 images
 * */
class Matching{
    public:
        /* *
         * Input: 2 images
         * */
        Matching(char *img1, char *img2, double thr);
        ~Matching();
        void match();
        void heuristicMatch();
    private:
        //private methods
        void getDescriptors();

        //combine 2 images vertically
        void combineImage();

        cv::Point2f transformPoint(cv::Point2f p2);

        cv::vector<cv::DMatch> getSymetryMatches(const cv::vector<cv::DMatch> &matches1, const cv::vector<cv::DMatch> &matches2);

        //private variables
        double threshold;

        cv::Mat image1;
        cv::Mat image2;

        //output image
        cv::Mat output;

        //keypoints of image 1
        std::vector<cv::KeyPoint> keyPoints1;
        //keypoints of image 2
        std::vector<cv::KeyPoint> keyPoints2;
        //image of descriptors of image1
        cv::Mat descriptors1;
        //image of descriptors of image2
        cv::Mat descriptors2;

        //Sift feature detector and extractor
        cv::SiftDescriptorExtractor descriptorExtractor;
        cv::SiftFeatureDetector     featureDetector;// = cv::SiftFeatureDetector(4000);
        
        //matcher
        cv::FlannBasedMatcher matcher;

        //3d stuff
        cv::Mat fundamentalMatrix;

        //color to draw matches 
        cv::Scalar color = cv::Scalar(123, 12, 111);

        std::string imageIdentify;

        std::ofstream writeRansac;
        std::ofstream writeHeuristic;
};

#endif

