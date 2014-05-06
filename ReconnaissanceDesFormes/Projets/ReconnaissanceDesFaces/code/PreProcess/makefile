CFLAGS = `pkg-config --cflags opencv` -pthread -Wall -g -std=c++11
LDFLAGS = `pkg-config --libs opencv` -pthread 
LIBS = `pkg-config --libs opencv` -lm
CC = g++
OBJ = IO.o Learning.o
%.o: %.cpp 
		$(CC) -c -o $@ $< $(CFLAGS)

learning: $(OBJ) 
	$(CC) -o $@ $^ $(LDFLAGS)
All:learning

clean:
	rm -f *.o learning 
