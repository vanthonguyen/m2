
#include "control.h"
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

int main(int argc, char* argv[])
{
    Control control;
    control.processCommand(argc, argv);
    return 0;

    /*
    std::ifstream ifs(argv[1]);
    std::ofstream ofs("test.txt");
    char c;
    int row = 0, col = 0;    
    cv::Mat image(28, 28, CV_8UC1);
    std::vector<cv::Mat> images;
    while(c != EOF){
	c = ifs.get();
	col++;
	if(c == 13)
	{
	    col = 0;
	    row++;
	}
	else{
	    if(c == '#' || c == '+')
		image.at<uchar>(row, col) = 255;
	    else image.at<uchar>(row, col) = 0;
	}
	
    }
    return 0;
    */
}
