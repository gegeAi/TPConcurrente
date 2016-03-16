EXE = Parking
RM = rm
EDL = g++
COMP = g++
ECHO = @echo
INT = Mere.h Config.h libtp.a
REAL = $(INT:.h=.cpp)
OBJ = $(INT:.h=.o)
OUTPUT = -o
CPPFLAGS = -Wall #-DMAP
CLEAN = efface

$(EXE): $(OBJ)
	$(EDL)  $(OUTPUT) $(EXE) $(EDLFLAGS) $(OBJ)

Mere.o : Mere.cpp Config.h libtp.a
	$(COMP) $(CPPFLAGS) -c Mere.cpp

$(CLEAN):
	$(RM) $(RMFLAGS) $(EXE) $(OBJ) core
