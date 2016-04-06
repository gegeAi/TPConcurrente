EXE = Parking
RM = rm
EDL = g++
COMP = g++
ECHO = @echo
INT = Mere.h GestionMenu.h BarriereSortie.h BarriereEntree.h
REAL = $(INT:.h=.cpp)
OBJ = $(INT:.h=.o)
OUTPUT = -o
CPPFLAGS = -Wall #-DMAP
CLEAN = efface

$(EXE): $(OBJ)
	$(EDL)  $(OUTPUT) $(EXE) $(OBJ) -L/shares/public/tp/tp-multitache/ -ltp -ltcl8.5 -lncurses 

Mere.o : Mere.cpp Config.h /shares/public/tp/tp-multitache/Outils.h /shares/public/tp/tp-multitache/Heure.h
	$(COMP) $(CPPFLAGS) -c Mere.cpp

GestionMenu.o : GestionMenu.cpp
	$(COMP) $(CPPFLAGS) -c GestionMenu.cpp

BarriereSortie.o : BarriereSortie.cpp
	$(COMP) $(CPPFLAGS) -c BarriereSortie.cpp

BarriereEntree.o : BarriereEntree.cpp
	$(COMP) $(CPPFLAGS) -c BarriereEntree.cpp

$(CLEAN):
	$(RM) $(RMFLAGS) $(EXE) $(OBJ) core
