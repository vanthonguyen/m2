#What needs to be built (TARG) from what source code (SRC)
SRC = faceRecognition.cpp
TARG = faceRecognition

#Tell make what compiler we use
CCC = g++

#Tell gcc about non-standard places to find include (.h) files
#For a system wide installation use -I /usr/include/opencv
INC = -I/usr/local/include/opencv 

#Also tell gcc to include debug symbols (-g), give all possible warnings
#(-Wall), but don't generate the annoying "unused function" warning
CFLAGS = -g -Wall -Wno-unused-function $(INC)

#Tell the linker where to look for libraries and what's needed for OpenCV
#For a system wide installation -L isn't necessary
LIB = -L/usr/local/lib -lopencv_calib3d -lopencv_contrib -lopencv_core  -lopencv_features2d  -lopencv_flann -lopencv_gpu -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_nonfree -lopencv_objdetect -lopencv_photo -lopencv_stitching -lopencv_ts -lopencv_video -lopencv_videostab
LDFLAGS = $(CFLAGS) $(LIB)

#The object files we want are just the source files with .c -> .o
OBJ = $(SRC:.c=.o)

#What make should try to build by default
all: $(TARG)

#What object files the target executable depends on
$(TARG): $(OBJ)

#Clean up executables, object files, and temp files
clean: rm -f *~ *.o $(TARG)
