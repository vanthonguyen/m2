/*
 * =====================================================================================
 *
 *       Filename:  training.cpp
 *    Description:  
 *        Version:  1.0
 *        Created:  12/18/2013 03:35:16 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  NGUYEN Van Tho (), thonv133@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "training.h"

int main(int argc, char* argv[]){
    Training training(argv[1], argv[2]);
    training.train();
}

Training::Training(char *fileName): imageFile(fileName),io(true, DESCRIPTOR_FILE){
}  
void Training::train(){
    std::ifstream ifs(imageFile);
    while(ifs.good())
    {
        cv::Mat image(WIDTH, HEIGHT, CV_8UC1);
        Channel channel(image);
        ifs>>channel;
        io.writeDescriptor(channel.getDescriptor(8));
    }   
}

Training::Training(char *fileName, char* descriptorName)
    :imageFile(fileName), descriptorFile(descriptorName), io(true, descriptorFile){
}
