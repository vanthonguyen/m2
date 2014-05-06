CFLAGS = `pkg-config --cflags opencv` -pthread -Wall -g -std=c++11
LDFLAGS = `pkg-config --libs opencv` -pthread 
LIBS = `pkg-config --libs opencv` -lm
CC = g++
OBJ = Detector.o BackgroundExtractor.o
OBJ2 = Tracking.o MovingObject.o BackgroundExtractor.o
%.o: %.cpp 
		$(CC) -c -o $@ $< $(CFLAGS)

detector: $(OBJ) 
	$(CC) -o $@ $^ $(LDFLAGS)
tracking:$(OBJ2)
	$(CC) -o $@ $^ $(LDFLAGS)
All:tracking

clean:
	rm -f *.o detector tracking
