#include <iostream>
#include <vector>
#include <stdio.h>
#include "channel.h"
#include "control.h"
#include <dirent.h>
#include <fstream>

#define CMD_CLASS "class"
#define CMD_HIST "hist"

Control::Control(){}
/*
  Pour attendre l'utilisateur tapper des commandes
  Cette methode va traitter des commandes entrées pour
  réaliser des besoin en rappant des autres méthodes correspondantes
*/
void Control::processCommand(int argc, char* argv[])
{
    std::string command, image_name;
    if(argc < 2)
	return;
    command = std::string(argv[1]);
    if(command.compare(CMD_HIST) == 0)
    {
	image_name = std::string(argv[2]);
	this->drawHistogram(image_name);
    }
    if(command.compare(CMD_CLASS) == 0)
    {
	std::string csv_train = std::string(argv[2]);
	std::string csv_test = std::string(argv[3]);
	classification(csv_train, csv_test);
    }
}

void Control::read_csv(const std::string& filename,
		       std::vector<Channel>& images,
		       char separator)
{
    std::ifstream file(filename.c_str(), std::ifstream::in);
    if (!file) {
	std::string error_message = "No valid input file was given";
        CV_Error(CV_StsBadArg, error_message);
    }
    std::string line, path, classlabel;
    while (getline(file, line)) {
	std::stringstream liness(line);
        getline(liness, path, separator);
        getline(liness, classlabel);
        if(!path.empty() && !classlabel.empty()) {
	    int type = atoi(classlabel.c_str());
	    Channel image(cv::imread(path, 0));
	    image.setType(type-1);
	    image.setName(path);
	    images.push_back(image);
        }
    }
}

void Control::drawHistogram(std::string image_name)
{
    cv::Mat image = cv::imread(image_name);
    cv::Mat hist(200, 256, CV_8UC1, cv::Scalar(0));
    Channel channel(image);
    channel.drawHistogram(hist, 255);
    cv::imwrite("hist.png", hist);
}

void Control::classification(std::string csv_train, std::string csv_test)
{
    std::vector<Channel> train_images;
    std::vector<Channel> test_images;
    // Read in the data. This can fail if no valid
    // input filename is given.
    try {
	read_csv(csv_train, train_images);
	read_csv(csv_test, test_images);
    } catch (cv::Exception& e) {
	std::cerr << "Error opening file \""
	     << csv_train <<"or "  <<csv_test
		  << "\". Reason: " << e.msg << std::endl;
        // nothing more we can do
        exit(1);
    }
    // Quit if there are not enough images for this demo.
    if(train_images.size() <= 1 || test_images.size() <= 1) {
	std::string error_message = "This demo needs at least 2 images to work.";
        CV_Error(CV_StsError, error_message);
    }
    int N = test_images.size();

    std::vector<double*> train_hists;
    Channel::train(train_images, train_hists);
    std::ostringstream oss;
    for (int i = 0; i < 13; ++i)
    {
	oss<<i;
	cv::Mat h(200, 256, CV_8UC1, cv::Scalar(0));
	Channel::drawHistogram(train_hists[i], h, 255);
	cv::imwrite(oss.str() + ".png", h);
	oss.str("");
    }
    for (int i = 0; i < N; ++i)
    {
	this->classImage(test_images[i], train_hists);
    }

/*
    for (int i = 0; i < N; ++i)
    {
	this->classImage(test_images[i], train_images);
    }
*/
    int nb_true = 0;
    for (int i = 0; i < N; ++i)
    {
	std::cout<<test_images[i].getPredict() <<" "
		 <<test_images[i].getType() <<std::endl;
	if(test_images[i].getPredict() == test_images[i].getType())
	{
	    nb_true++;
	}
    }
    std::cout<<(double)nb_true/(double)N*100.0 <<"%";
}

void Control::classImage(Channel& image, const std::vector<double*>& histograms)
{
    double _max = FLT_MAX;
    int index = 0;
    int N = histograms.size();
    for (int i = 0; i < N; ++i)
    {
	double D = image.distIntersection(histograms[i]);
	if(_max > D)
	{
	    _max = D;
	    index = i;
	}
    }
    image.setPredict(index);
    //std::cout<<images[index].getType() <<std::endl;
}

void Control::classImage(Channel& image, const std::vector<Channel>& images)
{
    double _max = FLT_MAX;
    int index = 0;
    int N = images.size();
    for (int i = 0; i < N; ++i)
    {
	double D = image.distIntersection(images[i]);
	if(_max > D)
	{
	    _max = D;
	    index = i;
	}
    }
    image.setPredict(images[index].getType());
    //std::cout<<images[index].getType() <<std::endl;
}
