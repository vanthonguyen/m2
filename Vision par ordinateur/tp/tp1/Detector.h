
#ifndef DETECTOR_H 
#define DETECTOR_H 
 
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#define HISTOGRAM_SKIN_DIR "histSkin"
#define HISTOGRAM_NON_SKIN_DIR "histNonSkin"
#define SEPERATOR "/"
#define IMAGE_EXT ".jpg"
#define MAX_VALUE  256

class Detector 
{
    public:
        Detector(int s, double t, char *td);
        void massDetect();
//        virtual ~Detector();
	private:
        int scale;
        char *testDir;
        int totalPixelSkin;
        int totalPixelSkinBase;
        int totalPixelNonSkin;
        int totalPixelNonSkinBase;
        int histoS[MAX_VALUE][MAX_VALUE];
        int histoNS[MAX_VALUE][MAX_VALUE];

        double threshold; 
        void getHistogramSkin();
        void getHistogramNonSkin();

        void getHistogram(char *file, bool isSkinExample);
        std::vector<std::string> getExamples(const char *dir);                
        int isImageFile(char *f);
        bool histogramTest(char *file);
        void writeHistogram();
        void readHistogram();
};
#endif

