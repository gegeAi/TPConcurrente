#ifndef CONFIG_H
#define CONFIG_H

#include "/shares/public/tp/tp-multitache/Outils.h"

#include <time.h>

static const char * REFERENCE = "./Mere.cpp"; 
static const int DROITS = 0666;
static const int TEMPO = 1;

typedef struct Voiture Voiture;
struct Voiture
{
	TypeUsager type;
	int num;
	int hEntree, hSortie;
	
};	

typedef struct
{
	long mtype;
	Voiture mVoiture;
	
} messVoiture;

static const int TAILLE_MSG_VOITURE = sizeof(Voiture);

typedef struct Requete Requete;
struct Requete
{
	TypeUsager type;
	time_t hRequete;
	short used;
};

typedef struct msgBaL msgBaL;
struct msgBaL
{
	long mtype;
	int place[1];
};
	
#endif
