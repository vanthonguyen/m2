#ifndef _CHANNEL_H_
#define _CHANNEL_H_

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <vector>

#define WIDTH 28
#define HEIGHT 28

class Channel : public cv::Mat
{
  private:
    int type;
    int type_predict;
  public:
    Channel();
    Channel(cv::Mat image);
    Channel(cv::Mat image, int t);
    Channel(Channel& channel);
    Channel& operator=(const Channel& channel);
    int getType() const;
    void setType(int t);
    int getPredict();
    void setPredict(int t);
    static bool compare(double a, double b, double e);
    static double different(cv::Mat img1, cv::Mat img2);
    static double differentNew(cv::Mat img1, cv::Mat img2);
    static double differentPixel(cv::Mat img1, cv::Mat img2);
    static void detectBlock(cv::Mat image, int start, int end, cv::Mat &block);
    void detectBlock(std::vector<cv::Mat> &blocks, int seuil);
    std::vector<int> getDescriptor(int nb_block);
    double getDistanceTo(Channel channel, int nb_block);
    friend std::istream& operator>>(std::istream& is, Channel &channel);
    friend std::ostream& operator<<(std::ostream& os, const Channel &channel);
};

#endif /* _CHANNEL_H_ */
