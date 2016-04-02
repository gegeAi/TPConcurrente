#ifndef CONFIG_H
#define CONFIG_H

static const char * REFERENCE = "./keyB3130"; 
static const int DROITS = 0660;

enum SemUsg
{
	PARKING,
	REQUETE,
	CPT,
	AUTO1,
	AUTO2, 
	AUTO3
};

typedef struct Voiture Voiture;
struct Voiture
{
	char type;
	int num;
	int hEntree, hSortie, cout;
};	

typedef struct Requete Requete;
struct Requete
{
	char type;
	int hRequete;
};
	
#endif
