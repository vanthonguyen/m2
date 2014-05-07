#ifndef RECOGNISE_H 
#define RECOGNISE_H 

#include <vector>
#define THRESHOLD 0.25

class Recognise{
    public:
        Recognise(std::vector<std::vector<double>> descs);
        int getType(const std::vector<double> &desc);
    private:
        std::vector<std::vector<double>> descriptors;        
};
#endif
