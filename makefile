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
	$(EDL)  $(OUTPUT) $(EXE) $(OBJ) -ltermcap -ltcl8.5 -L "./" -l tp

Mere.o : Mere.cpp Config.h Outils.h
	$(COMP) $(CPPFLAGS) -c Mere.cpp

$(CLEAN):
	$(RM) $(RMFLAGS) $(EXE) $(OBJ) core
