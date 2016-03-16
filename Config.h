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
	TypeUsager type;
	int num;
	int hEntree;
};	

typedef struct Requete Requete;
struct Requete
{
	TypeUsager type;
	int hRequete;
};
	
#endif
