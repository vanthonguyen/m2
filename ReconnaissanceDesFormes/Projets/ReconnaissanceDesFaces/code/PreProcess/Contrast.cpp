/*
 * =====================================================================================
 *
 *       Filename:  tp1.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/04/2013 04:16:50 PM
 *       Revision:  1
 *       Compiler:  g++
 *         Editor:i vim 
 *         Author:  NGUYEN Van Tho 
 *   Organization: IFI - Promotion 17 
 *
 * =====================================================================================
 */


#include	<stdlib.h>
#include	<stdio.h>
#include	<unistd.h>
#include	<getopt.h>
#include	<math.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "Contrast.h"
#include "util.h"
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */

int
main ( int argc, char *argv[] )
{
    char *ptr;
    int n = 0;
    point m[10];
    
    Tp1 tp1 = Tp1(argv[1]); 
    
    if(strcmp(argv[2], "linear") == 0){
        double alpha;
        int beta;
        alpha = strtod(argv[4], &ptr);
        beta = strtol(argv[5], &ptr, 10);

        cv::Mat transformed = tp1.transformLinear(alpha, beta);
        imwrite( argv[3], transformed);
    }else if(strcmp(argv[2], "nonlinear") == 0){
        float gamma = strtof(argv[4], &ptr);
        cv::Mat transformed = tp1.transformNonLinear(gamma);
        imwrite( argv[3], transformed);
    }else if(strcmp(argv[2], "equalisation") == 0){
        cv::Mat transformed = tp1.equalisation();
        imwrite( argv[3], transformed);
    }else {
        eprintf("Parammeter is not enough, use ./tp1 img_name ...");
        return 1;
    }
    return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */

Tp1::Tp1(char *imgName){
    image = cv::imread(imgName, CV_LOAD_IMAGE_UNCHANGED);
}


cv::Mat Tp1::getImage(){
    return image;
}

uchar Tp1::getIntensity(int x, int min, int max){
    if(x > max){ 
        x = max; 
    }else if (x < min){
        x = min; 
    }   
    return x;
}

cv::Mat Tp1::transformLinear(double alpha, int beta){
    cv::Mat imageTransformed = cv::Mat::zeros( image.size(), image.type() );
 // Do the operation new_image(i,j) = alpha*image(i,j) + beta
    for( int y = 0; y < image.rows; y++ ){
        for( int x = 0; x < image.cols; x++ ){
            imageTransformed.at<uchar>(y,x) =
                cv::saturate_cast<uchar>( alpha*( image.at<uchar>(y,x)) + beta );
        }
    }

    return imageTransformed;
}

cv::Mat Tp1::transformNonLinear(float gamma){
    cv::Size imageSize = image.size();
    cv::Mat imageTransformed = image.clone();

    //lookup table. pour optimiser la performance
    int lut[256];

    //l'intensité est normalisé dans le domaine 0 - 1 avant d'appliquer le formule
    for (int i = 0; i < 256; i++){
        lut [i] = 255*pow(i/255.0, gamma);
    }
    if(image.channels() == 1){
        for (int i = 0; i < imageSize.width; i++){
            for (int j = 0; j < imageSize.height; j++){
                imageTransformed.at<uchar>(j, i) = lut[image.at<uchar>(j, i)]; 
            }
        }
    }
    return imageTransformed;
}

uchar Tp1::maxIntensity(){
    return *std::max_element(image.begin<uchar>(),image.end<uchar>());
}
cv::Mat Tp1::equalisation(){
    cv::Mat imageTransformed;
    cv::equalizeHist( image, imageTransformed);
    return imageTransformed;
}
