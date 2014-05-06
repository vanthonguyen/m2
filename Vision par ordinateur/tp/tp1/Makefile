CFLAGS = `pkg-config --cflags opencv` -Wall -g
LDFLAGS = `pkg-config --libs opencv`  
LIBS = `pkg-config --libs opencv` -lm
CC = g++
OBJ = Detector.o

%.o: %.cpp 
		$(CC) -c -o $@ $< $(CFLAGS)

detector: $(OBJ) 
	$(CC) -o $@ $^ $(LDFLAGS)
evaluator:
	$(CC) -o evaluator Evaluator.cpp $(CFLAGS) $(LDFLAGS)
All:detector

clean:
	rm -f *.o detector
