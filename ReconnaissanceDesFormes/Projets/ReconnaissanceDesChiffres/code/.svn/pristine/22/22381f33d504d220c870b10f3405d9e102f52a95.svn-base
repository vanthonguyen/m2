#include <iostream>
#include <vector>
#include <stdio.h>
#include "control.h"
#include <dirent.h>
#include <fstream>

Control::Control(){}

/*
  Pour attendre l'utilisateur tapper des commandes
  Cette methode va traitter des commandes entrées pour
  réaliser des besoin en rappant des autres méthodes correspondantes
*/
void Control::processCommand(int argc, char* argv[])
{
    std::string train_path, test_path;
    std::string label_train_path, label_test_path;
    std::string result_path;
    std::string descriptor_train, descriptor_test;
    if(argc < 9)
        return;
    train_path = std::string(argv[1]);
    label_train_path = std::string(argv[2]);
    descriptor_train = std::string(argv[3]);
    test_path = std::string(argv[4]);
    label_test_path = std::string(argv[5]);
    descriptor_test = std::string(argv[6]);
    result_path = std::string(argv[7]);
    int K = atoi(argv[8]);
    this->categorisation(train_path, label_train_path, descriptor_train,
			 test_path, label_test_path,
			 descriptor_test, result_path, K);
}

void match(std::vector<Channel> &channels, std::vector<int> types)
{
    int N1 = channels.size();
    int N2 = types.size();
    if(N1 != N2)
        return;
    for (int i = 0; i < N1; ++i)
    {
        channels[i].setType(types[i]);
    }
}

void Control::categorisation(std::string train_path, std::string label_train_path,
			     std::string descriptor_train, std::string test_path,
			     std::string label_test_path, std::string descriptor_test,
			     std::string result_path, int K)
{
    std::vector<Channel> train_images;
    std::vector<int> train_types;
    this->getImageFromFile(train_path, train_images);
    this->getLabelFromFile(label_train_path, train_types);
    match(train_images, train_types);
    std::vector<Channel> test_images;
    std::vector<int> test_types;
    this->getImageFromFile(test_path, test_images);
    this->getLabelFromFile(label_test_path, test_types);
    match(test_images, test_types);

    IO train_io(false, strdup(descriptor_train.c_str()));
    IO test_io(false, strdup(descriptor_test.c_str()));
    std::vector< std::vector<int> > trains_v = train_io.getDescriptors();
    std::vector< std::vector<int> > tests_v = test_io.getDescriptors();    
    int N = test_images.size();
    if(N != (int)tests_v.size())
	return;
    int nb_true = 0;
    for (int i = 0; i < N; ++i)
    {
        this->predictNew(test_images[i], train_images, tests_v[i], trains_v, K);
	if(test_images[i].getType() == test_images[i].getPredict())
	    nb_true++;
    }
    
    if(N != 0)
    {
	double percent = (double)nb_true/(double)N;
	std::cout<<percent*100.0 <<"%" <<std::endl;
    }
    this->outputToFile(result_path, test_images);
}

/*
  type1_list: datas trainning of type 1
  type2_list: datas trainning of type 2
  image: test image
  This function return the type code of the type correct
  1:type1, 2:type2 and 3 is type code of other
*/
void Control::predict(Channel &image, const std::vector<Channel> &train_images,
		      int nb_block)
{
    int N = train_images.size();
    double min_distance = FLT_MAX;
    int index;
    for (int i = 0; i < N; ++i)
    {
        double D = image.getDistanceTo(train_images[i], nb_block);
        if(min_distance > D)
        {
            min_distance = D;
            index = i;
        }
    }
    int type = train_images[index].getType();
    image.setPredict(type);
}

template <typename T>
T getMostFreq(std::vector<T> v)
{
    T mostFreqElement = 0;
    int mostFreq = 0;
    int N = v.size();
    for (int i = 0; i < N; ++i)
    {
	int freq = std::count(v.begin(), v.end(), v[i]);
	if(freq > mostFreq)
	{
	    mostFreq = freq;
	    mostFreqElement = v[i];
	}
    }
    return mostFreqElement;
}

/*
  type1_list: datas trainning of type 1
  type2_list: datas trainning of type 2
  image: test image
  This function return the type code of the type correct
  1:type1, 2:type2 and 3 is type code of other
*/
void Control::predictNew(Channel &image, const std::vector<Channel> &train_images,
			 std::vector<int> v,
			 const std::vector< std::vector<int> > &trains_v, int K)
{
    int N = trains_v.size();
    std::vector<Distance> distance_v;
    Distance D;
    for (int i = 0; i < N; ++i)
    {
	D.index = i;
	D.value = this->descriptorsDiff(v, trains_v[i]);
	distance_v.push_back(D);
    }
    std::sort(distance_v.begin(), distance_v.end(), funcCompare);
    std::vector<int> types;
    if(K > (int)distance_v.size())
	return;
    for (int i = 0; i < K; ++i)
    {
	int index = distance_v[i].index;
	int type = train_images[index].getType();
        types.push_back(type);
    }
    int type = getMostFreq(types);
    image.setPredict(type);
}

void Control::getImageFromFile(std::string file_name, std::vector<Channel> &channels)
{
    std::ifstream ifs(file_name.c_str());
    int M = 28, N = 28;
    while(ifs.good())
    {
        cv::Mat image(M, N, CV_8UC1);
        Channel channel(image);
        ifs>>channel;
        channels.push_back(channel);        
    }
}

void Control::getLabelFromFile(std::string label_file_name, std::vector<int> &labels)
{
    std::ifstream ifs(label_file_name.c_str());
    int type;
    while(ifs.good())
    {
        ifs>>type;
        labels.push_back(type);
    }
}

void Control::outputToFile(std::string file_name, const std::vector<Channel> &channels)
{
    std::ofstream ofs(file_name.c_str());
    for (int i = 0; i < (int)channels.size(); ++i)
    {
        ofs<<channels[i];
    }
}

double Control::descriptorsDiff(const std::vector<int> &v1, const std::vector<int> &v2)
{
    double square = 0.0;
    int N = v1.size();
    for (int i = 0; i < N; ++i)
    {
	square += (v1[i] - v2[i])*(v1[i] - v2[i]);
    }
    return sqrt(square);
}

bool funcCompare(Distance d1, Distance d2)
{
    return (d1.value <= d2.value);
}
