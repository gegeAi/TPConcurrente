EXE = Analog
RM = rm
EDL = g++
COMP = g++
ECHO = @echo
INT = GestionGraphViz.h StockageDonnee.h TraitementFichier.h main.h
REAL = $(INT:.h=.cpp)
OBJ = $(INT:.h=.o)
OUTPUT = -o
CPPFLAGS = -Wall #-DMAP
CLEAN = efface

$(EXE): $(OBJ)
	$(EDL)  $(OUTPUT) $(EXE) $(EDLFLAGS) $(OBJ)

main.o : main.cpp TraitementFichier.h GestionGraphViz.h StockageDonnee.h
	$(COMP) $(CPPFLAGS) -c main.cpp

StockageDonnee.o : StockageDonnee.cpp StockageDonnee.h
	$(COMP) $(CPPFLAGS) -c StockageDonnee.cpp
	
GestionGraphViz.o : GestionGraphViz.cpp StockageDonnee.h
	$(COMP) $(CPPFLAGS) -c GestionGraphViz.cpp
	
TraitementFichier.o : TraitementFichier.cpp GestionGraphViz.h StockageDonnee.h
	$(COMP) $(CPPFLAGS) -c TraitementFichier.cpp

$(CLEAN):
	$(RM) $(RMFLAGS) $(EXE) $(OBJ) core
