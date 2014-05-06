CFLAGS = `pkg-config --cflags opencv` -pthread -Wall -g -std=c++11
LDFLAGS = `pkg-config --libs opencv` -pthread 
LIBS = `pkg-config --libs opencv` -lm
CC = g++
OBJ = Matching.o 
%.o: %.cpp 
		$(CC) -c -o $@ $< $(CFLAGS)

matching: $(OBJ) 
	$(CC) -o $@ $^ $(LDFLAGS)
All:matching

clean:
	rm -f *.o matching 
