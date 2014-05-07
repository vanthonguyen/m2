CFLAGS = `pkg-config --cflags opencv` -pthread -Wall -g -std=c++11
LDFLAGS = `pkg-config --libs opencv` -pthread 
LIBS = `pkg-config --libs opencv` -lm
CC = g++

OBJ2 = Utils.o main.o IO.o Clustering.o
OBJ3 = Utils.o Pca.o TestPca.o IO.o LearningPca.o ClusteringPca.o ClassifyPca.o
%.o: %.cpp 
		$(CC) -c -o $@ $< $(CFLAGS)

pca: $(OBJ3)
	$(CC) -o $@ $^ $(LDFLAGS)

blocs: $(OBJ2) 
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *.o pca blocs
