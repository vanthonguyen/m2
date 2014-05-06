CFLAGS = `pkg-config --cflags opencv` -pthread -Wall -g -std=c++11
LDFLAGS = `pkg-config --libs opencv` -pthread 
LIBS = `pkg-config --libs opencv` -lm
CC = g++
OBJ = TestScene.o Classifier.o
OBJ2 = Training.o Classifier.o
%.o: %.cpp 
		$(CC) -c -o $@ $< $(CFLAGS)

classifier: $(OBJ) 
	$(CC) -o $@ $^ $(LDFLAGS)

training: $(OBJ2) 
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *.o classifier training 
