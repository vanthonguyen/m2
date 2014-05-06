#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "channel.h"
#include <algorithm>
#include <iostream>
#include <stdio.h>

Channel::Channel()
    :Mat()
{
    this->type = 1;
}

Channel::Channel(cv::Mat image)
{
    image.copyTo(*this);
}

Channel::Channel(cv::Mat image, int t)
{
    image.copyTo(*this);
    this->type = t;
}

Channel::Channel(Channel& channel)
{
    channel.copyTo(*this);
    this->type = channel.type;
}

Channel& Channel::operator=(const Channel& channel)
{
    channel.copyTo(*this);
    this->type = channel.type;
    return *this;
}

int Channel::getType() const
{
    return this->type;
}

void Channel::setType(int t)
{
    this->type = t;
}

int Channel::getPredict()
{
    return this->type_predict;
}

void Channel::setPredict(int t)
{
    this->type_predict = t;
}

//Compare 2 reals with error e
bool Channel::compare(double a, double b, double e)
{
    double d = a - b;
    return(d < e && d > -e);
}

//Calcul the type different between 2 images
double Channel::different(cv::Mat img1, cv::Mat img2)
{
    int n1 = 0;
    int n2 = 0;
    cv::resize(img2, img2, img1.size());
    for (int i = 0; i < img1.rows; ++i)
    {
	for (int j = 0; j < img1.cols; ++j)
	{
	    if(img1.at<uchar>(i, j) != 0)
		n1++;
	}
    }
    for (int i = 0; i < img2.rows; ++i)
    {
	for (int j = 0; j < img2.cols; ++j)
	{
	    if(img2.at<uchar>(i, j) != 0)
		n2++;
	}
    }
    double s = n1 - n2;
    return s;
}

//Calcul the type different between 2 images
double Channel::differentNew(cv::Mat img1, cv::Mat img2)
{
    int n1 = 0;
    int n2 = 0;
    cv::resize(img2, img2, img1.size());
    n1 = cv::sum(img1)[0];
    n2 = cv::sum(img2)[0];
    double s = n1 - n2;
    return s;
}

//Calcul the type different between 2 images
double Channel::differentPixel(cv::Mat img1, cv::Mat img2)
{
    int M = img1.rows;
    int N = img1.cols;
    int nb_commun = 0;
    cv::resize(img2, img2, img1.size());
    for (int i = 0; i < img1.rows; ++i)
    {
        for (int j = 0; j < img1.cols; ++j)
        {
            if(img1.at<uchar>(i, j) != img2.at<uchar>(i, j))
                nb_commun++;

        }
    }
    return ((double)nb_commun/(double)M*N);
}

//Leave one part of image
void Channel::detectBlock(cv::Mat image, int start, int end, cv::Mat &block)
{
    cv::Rect rectangle(0, start, image.cols, end);
    block = image(rectangle).clone();
}

//Detect block by divide the image to nb_block^2 parts
void Channel::detectBlock(std::vector<cv::Mat> &blocks, int nb_block)
{
    double M = this->rows;
    double N = this->cols;
    double segment_m = M/(double)nb_block;
    double segment_n = N/(double)nb_block;

    for (int i = 0; i < nb_block; ++i)
    {
        for (int j = 0; j < nb_block; ++j)
        {
            cv::Rect rectangle(j*segment_n, i*segment_m,
                    segment_n, segment_m);
            cv::Mat image;
            (*this)(rectangle).copyTo(image);
            blocks.push_back(image);
        }
    }
}

std::vector<int> Channel::getDescriptor(int nb_block){
    double segment_m = WIDTH/(double)nb_block;
    double segment_n = HEIGHT/(double)nb_block;
    std::vector<int> descriptor;
    for (int i = 0; i < nb_block; ++i)
    {
        for (int j = 0; j < nb_block; ++j)
        {
            cv::Rect rectangle(j*segment_n, i*segment_m, segment_n, segment_m);
            int v = cv::sum( (*this)(rectangle))[0];
            descriptor.push_back(v);
        }
    }
    return descriptor; 
}

double Channel::getDistanceTo(Channel channel, int nb_block)
{
    std::vector<cv::Mat> blocks1;
    std::vector<cv::Mat> blocks2;
    this->detectBlock(blocks1, nb_block);
    channel.detectBlock(blocks2, nb_block);
    double distance = 0.0;
    int N = nb_block*nb_block;
    for (int i = 0; i < N; ++i)
    {
        double _distance = Channel::differentNew(blocks1[i], blocks2[i]);
        distance += _distance*_distance;
    }
    return distance;
}

std::istream& operator>>(std::istream& is, Channel &channel)
{
    int M = channel.rows;
    int N = channel.cols;
    char c;
    for (int i = 0; i < M; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            c = is.get();
            if(c == '#')
                channel.at<uchar>(i, j) = 2;
            else if(c == '+')
                channel.at<uchar>(i, j) = 1;
            else channel.at<uchar>(i, j) = 0;
        }
        is.get();
    }
    return is;
}

std::ostream& operator<<(std::ostream &os, const Channel &channel)
{
    int M = channel.rows;
    int N = channel.cols;
    for (int i = 0; i < M; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            if(channel.at<uchar>(i, j) == 2)
                os<<"+";
            else if(channel.at<uchar>(i, j) == 1)
                os<<"#";
            else os<<" ";
        }
        os<<std::endl;
    }
    os<<channel.type <<std::endl;
    os<<channel.type_predict <<std::endl;
    return os;
}
