CFLAGS = `pkg-config --cflags opencv` -Wall -g
LDFLAGS = `pkg-config --libs opencv`  
LIBS = `pkg-config --libs opencv` -lm
CC = g++

All:
	$(CC) -o text2image TextToImage.cpp $(CFLAGS) $(LDFLAGS)
clean:
	rm -f *.o text2image
