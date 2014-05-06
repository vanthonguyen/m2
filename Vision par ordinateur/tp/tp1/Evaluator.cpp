/*
 * =====================================================================================
 *
 *       Filename:  Evaluator.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/17/2013 06:39:02 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  NGUYEN Van Tho (), thonv133@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

int main(int argc, char ** argv){
    if(argc < 3){
        std::cout<<"Parammeter is not enough, use ./detector scale image_name ..."<<std::endl;
        return 2;
    }
    cv::Mat img = cv::imread(argv[1], CV_LOAD_IMAGE_UNCHANGED);
    cv::Mat ref = cv::imread(argv[2], CV_LOAD_IMAGE_UNCHANGED);
//    cv::cvtColor(img,img,  CV_BGR2Lab);
//    cv::cvtColor(ref, img, CV_BGR2Lab);
    if(img.size().width != ref.size().width || img.size().height != ref.size().height){
        std::cout<<"Sizes of image and image reference are different..."<<std::endl;
        return 1;
    }
     
    int tp = 0;
    int fp = 0;
    int tn = 0;
    int fn = 0;
    double tpr;
    double fpr;
    double tnr;
    double fnr;
    for (int row = 0; row < ref.size().height; row++){
        for (int col = 0; col < ref.size().width; col++){
            cv::Vec3b refPixel = ref.at<cv::Vec3b>(row, col);
            cv::Vec3b imgPixel = img.at<cv::Vec3b>(row, col);
            //calculer index
            if(refPixel.val[0] == 0 && refPixel.val[1] == 0 && refPixel.val[2] == 0){
                //true negative
                if(imgPixel.val[0] == 0 && imgPixel.val[1] == 0 && imgPixel.val[2] == 0){
                    tn++;
                }else{//false positive
                    fp++;
                }
            }else{
                if(imgPixel.val[0] == 0 && imgPixel.val[1] == 0 && imgPixel.val[2] == 0){
                    fn++;
                }else{//false positive
                    tp++;
                }
            }
        }
    }
    tpr = 1.0*tp/(tp + fn);
    fpr = 1.0*fp/(fp + tn);
    tnr = 1.0*tn/(tn + fp);
    fnr = 1.0*fn/(fn + tp);

    //output
    std::cout<<"True Positive: "<<tp<<std::endl;
    std::cout<<"False Positive: "<<fp<<std::endl;
    std::cout<<"True Negative: "<<tn<<std::endl;
    std::cout<<"False Negative: "<<fn<<std::endl;

    std::cout<<"True Positive Rate: "<<tpr<<std::endl;
    std::cout<<"False Positive Rate: "<<fpr<<std::endl;
    std::cout<<"True Negative Rate: "<<tnr<<std::endl;
    std::cout<<"False Negative Rate: "<<fnr<<std::endl;
}

