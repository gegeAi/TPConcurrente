EXE = Parking
RM = rm
EDL = g++
COMP = g++
ECHO = @echo
INT = Mere.h GestionMenu.h BarriereSortie.h
REAL = $(INT:.h=.cpp)
OBJ = $(INT:.h=.o)
OUTPUT = -o
CPPFLAGS = -Wall #-DMAP
CLEAN = efface

$(EXE): $(OBJ)
	$(EDL)  $(OUTPUT) $(EXE) $(OBJ) -L./ -ltp -ltcl8.5 -lncurses 

Mere.o : Mere.cpp Config.h Outils.h Heure.h
	$(COMP) $(CPPFLAGS) -c Mere.cpp

GestionMenu.o : GestionMenu.cpp
	$(COMP) $(CPPFLAGS) -c GestionMenu.cpp

BarriereSortie.o : BarriereSortie.cpp
	$(COMP) $(CPPFLAGS) -c BarriereSortie.cpp

$(CLEAN):
	$(RM) $(RMFLAGS) $(EXE) $(OBJ) core
