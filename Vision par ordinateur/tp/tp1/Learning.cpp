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

#include "Learning.h"
#include "def.h"

const char *dirExamplesSkin = "examples/skin";
const char *dirExamplesNonSkin = "examples/nonskin";
const char *imageExtension = ".jpg";
//float *histogram;

/*int main(int argc, char ** argv){
    //devoir utiliser parametre
    Learning learning(32);
    learning.getHistogramSkin();
    return 0;
}
*/

Learning::Learning(int s): imageHistogramSkin(s, s, CV_32FC1, cv::Scalar(0)), imageHistogramNonSkin(s, s, CV_32FC1, cv::Scalar(0)){
//Learning::Learning(int s){
    scale = s;
//    histogram = new float[s*s];
    histogramSkin.assign(s*s, 0.0); 
    histogramNonSkin.assign(s*s, 0.0); 
    totalPixelSkin = 0;
    totalPixelNonSkin = 0;
}
/*Learning::~Learning(){
   imageHistogramSkin.release();
   imageHistogramNonSkin.release();
}
*/

cv::Mat Learning::getHistogramSkin(){
    char fileImage[128]; 
    sprintf(fileImage, "%s/%d%s", HISTOGRAM_SKIN_DIR, scale, IMAGE_EXT);

/*     if(histogramTest(fileImage)){
        cv::Mat image = cv::imread(fileImage, CV_LOAD_IMAGE_UNCHANGED);
        if(!image.data){
            printf("Learning, No data: %s\n", fileImage);
        }
        return image;
    }
*/

    std::vector<std::string> listFile;
    listFile = getExamples(dirExamplesSkin);
    for (std::vector<std::string>::iterator it = listFile.begin() ; it != listFile.end(); ++it){
        char imageUrl[128];
        sprintf(imageUrl, "%s/%s", dirExamplesSkin, (*it).c_str());
        getHistogram(imageUrl, 1);
    }

//std::cout<<"Latot"<<totalPixelSkin<<"Total"<<std::endl;
//    saveHistogramAsImage(1);
    for(int i = 0; i < scale; i++){
        for(int j = 0; j < scale; j++){
//std::cout<<"before"<<imageHistogramSkin.at<double>(j, i)<<std::endl;
            imageHistogramSkin.at<double>(j, i) = imageHistogramSkin.at<double>(j, i) / totalPixelSkin;
//std::cout<<"after"<<imageHistogramSkin.at<double>(j, i)<<std::endl;
        }
    }
    cv::Mat ret;
    imageHistogramSkin.copyTo(ret);
    return ret;
}

cv::Mat Learning::getHistogramNonSkin(){
    char fileImage[128]; 
    sprintf(fileImage, "%s/%d%s", HISTOGRAM_NON_SKIN_DIR, scale, IMAGE_EXT);

/*     if(histogramTest(fileImage)){
        cv::Mat image = cv::imread(fileImage, CV_LOAD_IMAGE_UNCHANGED);
        return image;
    }
*/
    std::vector<std::string> listFile;
    listFile = getExamples(dirExamplesNonSkin);
    for (std::vector<std::string>::iterator it = listFile.begin() ; it != listFile.end(); ++it){
        char imageUrl[128];
        sprintf(imageUrl, "%s/%s", dirExamplesNonSkin, (*it).c_str());
        getHistogram(imageUrl, 0);
    }
    for(int i = 0; i < scale; i++){
        for(int j = 0; j < scale; j++){
//            std::cout<<"before"<<imageHistogramNonSkin.at<double>(j, i)<<std::endl;
            imageHistogramNonSkin.at<double>(j, i) = imageHistogramNonSkin.at<double>(j, i) / totalPixelNonSkin;
//            std::cout<<"after"<<imageHistogramNonSkin.at<double>(j, i)<<std::endl;
        }
    }

    cv::Mat ret;
    imageHistogramNonSkin.copyTo(ret);
    return ret;
}

void Learning::getHistogram(char *file, int isSkinExample){
    //read file
    cv::Mat image = cv::imread(file, CV_LOAD_IMAGE_UNCHANGED);
    cv::cvtColor(image, image, CV_BGR2Lab);     
    //calculer l'histogramme                                                                                           
    for (int i = 0; i < image.size().width; i++){
        for (int j = 0; j < image.size().height; j++){
            cv::Vec3b labPixel = image.at<cv::Vec3b>(j, i);
            //calculer index
            if(labPixel.val[0] == 0 && labPixel.val[1] == 128 && labPixel.val[2] == 128){
                continue;
            }
            int a = (int)labPixel.val[1]*scale / MAX_VALUE;
            int b = (int)labPixel.val[2]*scale / MAX_VALUE;
            int index = b*scale + a;
            if(isSkinExample){
           //     histogramSkin.at(index) += 1;
                imageHistogramSkin.at<double>(b, a) = imageHistogramSkin.at<double>(b, a) + 1;
            }else{
                imageHistogramNonSkin.at<double>(b, a) = imageHistogramNonSkin.at<double>(b, a) + 1;
            }
        }
    }
    if(isSkinExample){
        totalPixelSkin += image.size().width * image.size().height;
    }else{
        totalPixelNonSkin += image.size().width * image.size().height;
    }
}

std::vector<std::string> Learning::getExamples(const char *dir){
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

void Learning::saveHistogramAsImage(int isHistogramSkin){                                                                                               
    char fileImage[128]; 
    if(isHistogramSkin){
        //normaliser
        //cv::divide(totalPixelSkin, imageHistogram, imageHistogram);
        //imageHistogramSkin = imageHistogramSkin / totalPixelSkin;
        for(int i = 0; i < scale; i++){
            for(int j = 0; j < scale; j++){
//std::cout<<"before"<<imageHistogramSkin.at<double>(j, i)<<std::endl;
                imageHistogramSkin.at<double>(j, i) = imageHistogramSkin.at<double>(j, i) / totalPixelSkin;
//std::cout<<"after"<<imageHistogramSkin.at<double>(j, i)<<std::endl;
            }
        }
        //imageHistogramSkin.at<double>(0, 0) = 0;
        sprintf(fileImage, "%s/%d%s", HISTOGRAM_SKIN_DIR, scale, IMAGE_EXT);
        //stocker image
        //cv::imwrite( fileImage, imageHistogramSkin);
        cv::imshow("Skin", imageHistogramSkin);
        cv::waitKey();
        //return imageHistogramSkin;
    }else{
        //normaliser
        for(int i = 0; i < scale; i++){
            for(int j = 0; j < scale; j++){
                imageHistogramNonSkin.at<double>(j, i) = imageHistogramNonSkin.at<double>(j, i) / totalPixelNonSkin;
            }
        }
        //imageHistogramNonSkin.at<double>(0, 0) = ;
        //imageHistogramNonSkin = imageHistogramNonSkin / totalPixelNonSkin;
        sprintf(fileImage, "%s/%d%s", HISTOGRAM_NON_SKIN_DIR, scale, IMAGE_EXT);
        //stocker image
        //cv::imwrite( fileImage, imageHistogramNonSkin);
        cv::imshow("Non", imageHistogramNonSkin);
        cv::waitKey();
        //return imageHistogramNonSkin;
    }
}

/*  cv::Mat Learning::saveHistogramAsImage(int isHistogramSkin){
    cv::Mat imageHistogram (scale + 1, scale + 1, CV_32FC1, cv::Scalar(0));

    char fileImage[128]; 
    if(isHistogramSkin){
        for(int i = 1; i < histogramSkin.size(); i++){
           // imageHistogram.at<double>(y, x) = 1.0;
            imageHistogram.at<double>(i / scale, i % scale) = histogramSkin.at(i);
        }
        //normaliser
        cv::divide(totalPixelSkin, imageHistogram, imageHistogram);
        sprintf(fileImage, "%s/%d%s", HISTOGRAM_SKIN_DIR, scale, IMAGE_EXT);
    }else{
        for(int i = 1; i < histogramNonSkin.size(); i++){
            //imageHistogram.at<double>(i / scale, i % scale) = histogramSkin.at(i);
            imageHistogram.at<double>(i / scale, i % scale) = histogramNonSkin.at(i);
std::cout<< histogramNonSkin.at(i);
        }
        //normaliser
        cv::divide(totalPixelNonSkin, imageHistogram, imageHistogram);
        sprintf(fileImage, "%s/%d%s", HISTOGRAM_NON_SKIN_DIR, scale, IMAGE_EXT);
    }
    //stocker image
    imwrite( fileImage, imageHistogram);  
    return imageHistogram;
}
*/

int Learning::isImageFile(char *f){
    if(strstr(f, imageExtension)){
        return 1;
    }
    return 0;
}

bool Learning::histogramTest(char *file) {
    return (access(file, F_OK ) == 0);
}
