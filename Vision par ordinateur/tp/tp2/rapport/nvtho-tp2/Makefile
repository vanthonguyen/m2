
# ------------------ Compilation options ------------------------

# Loads math library.  
LIBS = -lm

# Flags for the C compiler:
#   -Wall for strict gcc warnings (requires prototypes for all functions).
#   -g to produce debug data for gdb
#   -O for optimization
CFLAGS = -Wall -g

CC = gcc

CVCFLAGS = `pkg-config --cflags opencv` -Wall -g
CVLDFLAGS = `pkg-config --libs opencv`
CVCC = g++
# --------------------- Code modules ----------------------------

# Object files
OBJ = match.o util.o

# Definitions
DEFS = defs.h

# ------------------------ Rules --------------------------------

match: ${OBJ}
	${CC} -o $@ ${CFLAGS} ${OBJ} ${LIBS}
text2image:
	${CVCC} -o text2image TextToImage.cpp $(CVCFLAGS) $(CVLDFLAGS) 
# Implicit rule used by Gnu Make: $(CC) -c $(CPPFLAGS) $(CFLAGS)
${OBJ}: ${DEFS}
