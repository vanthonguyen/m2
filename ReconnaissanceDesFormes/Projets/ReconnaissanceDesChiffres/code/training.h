#ifndef TRAINNING_H
#define TRAINNING_H

#include "channel.h"
#include "IO.h"

#define DESCRIPTOR_FILE "train_descriptors"
class Training{
    public:
    Training(char *fileName);
    Training(char *fileName, char* descriptorName);
        void train();
    private:
	char *imageFile;
	char *descriptorFile;
	IO io;
};
#endif
