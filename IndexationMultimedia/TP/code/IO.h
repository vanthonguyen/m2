#ifndef IO_H
#define IO_H

#include<iostream>
#include<vector>
#include <fstream>

#define FILE_NAME "descriptors"
#define IMAGE_EXTENSIONS ".png"
class IO{
    public:
        static std::vector<std::vector<double>> getDescriptors(const char *fileName);
        void writeDescriptor(std::vector<double> descriptor);
        static void writeDescriptors(const std::vector<std::vector<double>> &descriptors, const char * fileName);
        static std::vector<std::string> getFilesInFolder(const char *folderName);
    private:
        static bool isImageFile(char *f);
};

#endif
