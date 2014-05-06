#ifndef _CONTROL_H_
#define _CONTROL_H_
#include <string>
#include "channel.h"
#include "IO.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

class Control
{
  public:
    Control();
    void processCommand(int argc, char* argv[]);
    void categorisation(std::string train_path, std::string label_train_path,
			std::string descriptor_train, std::string test_path,
			std::string label_test_path, std::string descriptor_test,
			std::string result_path, int K);
    void train();
    void predict(Channel &image, const std::vector<Channel> &train_images,
		 int nb_block);
    void predictNew(Channel &image, const std::vector<Channel> &train_images,
		    std::vector<int> v,
		    const std::vector< std::vector<int> > &trains_v, int K);
    void getImageFromFile(std::string file_name, std::vector<Channel> &channels);
    void getLabelFromFile(std::string label_file_name, std::vector<int> &labels);
    void outputToFile(std::string file_name, const std::vector<Channel> &channels);
    double descriptorsDiff(const std::vector<int> &v1, const std::vector<int> &v2);
};

struct Distance
{
    int index;
    double value;
};

bool funcCompare(Distance d1, Distance d2);


#endif /* _CONTROL_H_ */
