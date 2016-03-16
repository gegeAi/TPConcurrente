EXE = Parking
RM = rm
EDL = g++
COMP = g++
ECHO = @echo
INT = Mere.h 
REAL = $(INT:.h=.cpp)
OBJ = $(INT:.h=.o)
OUTPUT = -o
CPPFLAGS = -Wall #-DMAP
CLEAN = efface

$(EXE): $(OBJ)
	$(EDL)  $(OUTPUT) $(EXE) $(OBJ) -L ./libtp

Mere.o : Mere.cpp Config.h 
	$(COMP) $(CPPFLAGS) -c Mere.cpp

$(CLEAN):
	$(RM) $(RMFLAGS) $(EXE) $(OBJ) core
