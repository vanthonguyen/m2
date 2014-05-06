/*
 * =====================================================================================
 *
 *       Filename:  Detector.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/15/2013 12:48:19 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  NGUYEN Van Tho (), thonv133@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */


#include <iostream>
#include <opencv2/opencv.hpp> 
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>

#include "Detector.h"

//float *histogram;
const double THRESHOLD = 0.2;
#define dirExamplesSkin  "examples/skin"
#define dirExamplesNonSkin  "examples/nonskin"
#define imageExtension ".jpg"
#define IMAGES "images"

int main(int argc, char ** argv){
    //devoir utiliser parametre
    if(argc < 4){
        printf("Parammeter is not enough, use ./detector scale image_name ...");
        return 2;
    }
    char *ptr;
    int scale = strtol(argv[1], &ptr, 10);
    double threshold = strtod(argv[2], &ptr);
    Detector detector(scale, threshold, argv[3]);
    detector.massDetect();
    return 0;
}

Detector::Detector(int s, double t, char *td){
    totalPixelSkin = 0;
    totalPixelNonSkinBase = 0;
    totalPixelSkinBase = 0;
    totalPixelNonSkin = 0;
    scale = s;
    threshold = t;
    testDir = td;
    getHistogramSkin(); 
    getHistogramNonSkin(); 
    writeHistogram();
}

void Detector::massDetect(){
    std::vector<std::string> listFile = getExamples(testDir); 

//    double possibilityOfSkin = 1.0*totalPixelSkin/totalPixelSkinBase;
//    double possibilityOfNonSkin = 1.0*totalPixelNonSkin/totalPixelNonSkinBase;
    double delta = (1.0*totalPixelSkin/totalPixelSkinBase)/(1.0*totalPixelNonSkin/totalPixelNonSkinBase);
    for (std::vector<std::string>::iterator it = listFile.begin() ; it != listFile.end(); ++it){
        char imageUrl[128];
        sprintf(imageUrl, "%s/%s", testDir, (*it).c_str());
        cv::Mat img2;    
        cv::Mat img = cv::imread(imageUrl, CV_LOAD_IMAGE_UNCHANGED);
        if(!img.data){
            printf("Detector, No data: %s\n", imageUrl );
        }
        cv::GaussianBlur(img, img, cv::Size(3,3), 0);
        cv::cvtColor(img, img, CV_BGR2Lab);
        img.copyTo(img2);

std::cout<<"---------------------------------------------------------"<<std::endl;
        for (int row = 0; row < img.size().height; row++){
            for (int col = 0; col < img.size().width; col++){
                cv::Vec3b labPixel = img.at<cv::Vec3b>(row, col);
                //calculer index
                int a = (int)labPixel.val[1]*scale / MAX_VALUE;
                int b = (int)labPixel.val[2]*scale / MAX_VALUE;
                double ps = 1.0*histoS[b][a]/totalPixelSkinBase; 
                double pns = 1.0*histoNS[b][a]/totalPixelNonSkinBase; 
//                std::cout<<a<< "  " << b << "  "<<histoS[b][a] << "  "<< histoNS[b][a] << "  "<< 1.0*histoS[b][a]/totalPixelSkinBase <<"  "<<  1.0*histoNS[b][a]/totalPixelNonSkinBase<< std::endl; 
                labPixel.val[1] = 128;
                labPixel.val[2] = 128;
                labPixel.val[0] = 0;
//                if(possibilityIsSkin < possibilityOfNonSkin){
//                if(possibilityIsSkin < THRESHOLD){

                if(ps* delta/ pns < threshold){
                    img.at<cv::Vec3b>(row, col) = labPixel;
                }else{
                    img2.at<cv::Vec3b>(row, col) = labPixel;
                }

            }
        }
        cv::medianBlur(img, img, 5);
        cv::medianBlur(img2, img2, 5);
        cv::cvtColor(img, img, CV_Lab2BGR);
        cv::cvtColor(img2, img2, CV_Lab2BGR);
        //write image

        char imageOutSkin[128];
        char imageOutNonSkin[128];
        sprintf(imageOutSkin, "output/skin/%s",  (*it).c_str());
        sprintf(imageOutNonSkin, "output/nonskin/%s",  (*it).c_str());
        cv::imwrite(imageOutSkin, img);
        cv::imwrite(imageOutNonSkin, img2);
    }
                                                                                                                                                               
} 

void Detector::getHistogramSkin(){
    std::vector<std::string> listFile = getExamples(dirExamplesSkin);
    for (std::vector<std::string>::iterator it = listFile.begin() ; it != listFile.end(); ++it){
        char imageUrl[128];
        sprintf(imageUrl, "%s/%s", dirExamplesSkin, (*it).c_str());
        //read file
        cv::Mat image = cv::imread(imageUrl, CV_LOAD_IMAGE_UNCHANGED);
        cv::cvtColor(image, image, CV_BGR2Lab);     

        for (int row = 0; row < image.size().height; row++){
            for (int col = 0; col < image.size().width; col++){
                cv::Vec3b labPixel = image.at<cv::Vec3b>(row, col);
                //calculer index
                if(labPixel.val[0] == 0 && labPixel.val[1] == 128 && labPixel.val[2] == 128){
                    continue;
                }
                int a = (int)labPixel.val[1]*scale / MAX_VALUE;
                int b = (int)labPixel.val[2]*scale / MAX_VALUE;
                histoS[b][a] ++;
                totalPixelSkin++;
            }
        }

        totalPixelSkinBase += image.size().width * image.size().height;
        //        getHistogram(imageUrl, true);
    }
}   

void Detector::getHistogramNonSkin(){
    std::vector<std::string> listFile = getExamples(dirExamplesNonSkin);
    for (std::vector<std::string>::iterator it = listFile.begin() ; it != listFile.end(); ++it){
        char imageUrl[128];
        sprintf(imageUrl, "%s/%s", dirExamplesNonSkin, (*it).c_str());
        cv::Mat image = cv::imread(imageUrl, CV_LOAD_IMAGE_UNCHANGED);
        cv::cvtColor(image, image, CV_BGR2Lab);     

        for (int row = 0; row < image.size().height; row++){
            for (int col = 0; col < image.size().width; col++){
                cv::Vec3b labPixel = image.at<cv::Vec3b>(row, col);
                //calculer index
                if(labPixel.val[0] == 0 && labPixel.val[1] == 128 && labPixel.val[2] == 128){
                    continue;
                }
                int a = (int)labPixel.val[1]*scale / MAX_VALUE;
                int b = (int)labPixel.val[2]*scale / MAX_VALUE;
                histoNS[b][a] ++;
                totalPixelNonSkin++;
            }
        }
        totalPixelNonSkinBase += image.size().width * image.size().height;
        //        getHistogram(imageUrl, false); 
    }   
}  


void Detector::getHistogram(char *file,  bool isSkinExample){ 
    //read file
    cv::Mat image = cv::imread(file, CV_LOAD_IMAGE_UNCHANGED);
    cv::cvtColor(image, image, CV_BGR2Lab);     
    //calculer l'histogramme                                                                                           
    for (int row = 0; row < image.size().height; row++){
        for (int col = 0; col < image.size().width; col++){
            cv::Vec3b labPixel = image.at<cv::Vec3b>(row, col);
            //calculer index
            if(labPixel.val[0] == 0 && labPixel.val[1] == 128 && labPixel.val[2] == 128){
                continue;
            }
            int a = (int)labPixel.val[1]*scale / MAX_VALUE;
            int b = (int)labPixel.val[2]*scale / MAX_VALUE;
            if(isSkinExample){
                histoS[b][a] ++;
                totalPixelSkin++;
            }else{
                histoNS[b][a] ++;
                totalPixelNonSkin++;
            }
        }
    }
    if(isSkinExample){ 
        totalPixelSkinBase += image.size().width * image.size().height;
    }else{
        totalPixelNonSkinBase += image.size().width * image.size().height;
    }
}

std::vector<std::string> Detector::getExamples(const char *dir){
    DIR *dp;                                                                                                                                                   
    struct dirent *ep;
    std::vector<std::string> listFile;
         
    dp = opendir (dir);
    if (dp != NULL){    
        while (ep = readdir (dp)){
            if(isImageFile(ep->d_name)){
                listFile.push_back(ep->d_name);
//                puts (ep->d_name);
            }
        }
        (void) closedir (dp);
    }else{
        perror ("Couldn't open the directory");
    }

    return listFile;
}

int Detector::isImageFile(char *f){
    if(strstr(f, imageExtension) || strstr(f, ".png")){
        return 1;
    }
    return 0;
}

bool Detector::histogramTest(char *file) {
    return (access(file, F_OK ) == 0);
}

void Detector::writeHistogram(){
    cv::Mat hist(scale + 1, scale + 1, CV_32FC1);
    cv::Mat histN;
    hist.copyTo(histN);
//    cv::cvtColor(image, newImage, CV_Lab2BGR);
    //matrice à cv Mat
    for (int row = 0; row < scale; row++){
        for (int col = 0; col < scale; col++){
            hist.at<double>(row, col) = 1.0*histoS[row][col]/totalPixelSkinBase;
        }
    }
    for (int row = 0; row < scale; row++){
        for (int col = 0; col < scale; col++){
            histN.at<double>(row, col) = 1.0*histoNS[row][col]/totalPixelNonSkinBase;
        }
    }
    char histName[64];
    char histNName[64];
    sprintf(histName, "histogram%d.jpg", scale);
    sprintf(histNName, "histogramNonSkin%d.jpg", scale);
//    cv::normalize(hist, hist, 1, cv::NORM_MINMAX);
    cv::imwrite( histName, hist);
    cv::imwrite( histNName, histN);

}
void Detector::readHistogram(){

}
