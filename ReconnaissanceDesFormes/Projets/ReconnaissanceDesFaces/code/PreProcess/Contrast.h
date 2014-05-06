#include <iostream>
#include <opencv2/opencv.hpp>

/*
 ** =====================================================================================
 **          
 **       Filename:  tp1.h
 **
 **    Description:  
 **
 **        Version:  1.0
 **        Created:  03/04/2013 04:16:50 PM
 **       Revision:  1
 **       Compiler:  g++
 **         Editor:  vim 
 **         Author:  NGUYEN Van Tho 
 **   Organization:  IFI - Promotion 17 
 **
 ** =====================================================================================
 **/                     

struct point{
    uchar x;
    uchar y;
};

class Tp1
{
    public:
        Tp1 (char *imgName);                             /* constructor */
        //~Tp1();

        cv::Mat getImage();
        /**
         * La transformation de l'image par la méthode
         * linaire avec morceaux, les morceaux sont définis par
         * le tableau p
         **/
        cv::Mat transformLinear(double alpha, int beta);
        /** 
         * La correction de gamma
         **/
        cv::Mat transformNonLinear(float gamma);
        /**
         * L'égalisation de l'histogramme
         **/
        cv::Mat equalisation();

	private:

		/* ====================  METHODS       ======================================= */
        uchar* transformPiece(int x1, int x2, int y1, int y2);
        uchar getIntensity(int x, int min, int max);
        uchar maxIntensity();
		/* ====================  DATA MEMBERS  ======================================= */
        cv::Mat image; 
}; /* -----  end of class Tp1  ----- */

