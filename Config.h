#ifndef CONFIG_H
#define CONFIG_H

#include "Outils.h"
static const char * REFERENCE = "./keyB3130"; 
static const int DROITS = 0660;

typedef struct Voiture Voiture;
struct Voiture
{
	TypeUsager type;
	int num;
	int hEntree, hSortie;
};	

static const int TAILLE_MSG_VOITURE = sizeof(Voiture);

typedef struct Requete Requete;
struct Requete
{
	TypeUsager type;
	int hRequete;
};

typedef struct msgBaL msgBaL;
struct msgBaL
{
	long type;
	int place[1];
};
	
#endif
