/*************************************************************************
                           GestionMenu  -  description
                             -------------------
    début                : ${date}
    copyright            : (C) ${year} par ${user}
*************************************************************************/

//---------- Réalisation de la tache GestionMenu (fichier ${file_name}) --

//---------------------------------------------------------------- INCLUDE

//-------------------------------------------------------- Include système
using namespace std;
#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>

//------------------------------------------------------ Include personnel
#include "GestionMenu.h"
#include "Menu.h"

#include "Outils.h"
#include "Config.h"

///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types
/*struct Voiture{
	
	char type;
	int num;
	int hEntree, hSortie, cout;
	
};
typedef struct Voiture Voiture;*/

//---------------------------------------------------- Variables statiques
static int id_balProfs;
static int id_balAutres;
static int id_balGB;
static int id_balSortie;
static Voiture nouveau;

//------------------------------------------------------ Fonctions privées
//static type nom ( liste de paramètres )
// Mode d'emploi :
//
// Contrat :
//
// Algorithme :
//
//{
//} //----- fin de nom


static void init(unsigned int balAutres, unsigned int balProfs, unsigned int balGB, unsigned int balSortie)
// Mode d'emploi :
//
// Contrat :
//
// Algorithme :
//
{
	//masquage des signaux de l'appli
	struct sigaction masquage;
	masquage.sa_handler = SIG_IGN;
	sigemptyset (&masquage.sa_mask);
	masquage.sa_flags = 0;
	sigaction (SIGUSR1, &masquage, NULL);
	sigaction (SIGUSR2, &masquage, NULL);

	//Recuperation des boites aux lettre
	id_balAutres = balAutres;
	id_balProfs = balProfs;
	id_balGB = balGB;
	id_balSortie = balSortie;
	
    nouveau.num = 0;

}// fin de init

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques

/*type Nom ( liste de paramètres )
// Algorithme :
//
{
} //----- fin de Nom*/


void GestionMenu(unsigned int balAutres, unsigned int balProfs, unsigned int balGB, unsigned int balSortie)
// Algorithme :
//
{
	init(balAutres, balProfs, balGB, balSortie);
	
	for(;;)
	{
		Menu();
	}
	exit(0);
	
}//----- fin de GestionMenu



void Commande( char code, unsigned int valeur)
// Algorithme :
//
{
	switch(code)
	{
		case 'E' :
		{
			Afficher(MESSAGE, "lol lol");
			exit(0);
		}
		case 'P' :
		{
			
			nouveau.type = PROF;
			nouveau.hEntree = time(NULL);
			nouveau.hSortie = 0;
			nouveau.num = (nouveau.num+1)%1000;
			nouveau.mtype = 0;
			
			if(valeur == 1)
			{									
				msgsnd(id_balProfs,&nouveau,TAILLE_MSG_VOITURE,IPC_NOWAIT);
			}
			else
			{
				msgsnd(id_balGB,&nouveau,TAILLE_MSG_VOITURE,IPC_NOWAIT);

			}
			break;
		}
		case 'A' :
		{
		
			nouveau.type = AUTRE;
			nouveau.hEntree = time(NULL);
			nouveau.hSortie = 0;
			nouveau.num = (nouveau.num+1)%1000;
			nouveau.mtype = 0;
			
			if(valeur == 1)
			{			
				msgsnd(id_balAutres,&nouveau,TAILLE_MSG_VOITURE,IPC_NOWAIT);

			}
			else
			{
		
				msgsnd(id_balGB,&nouveau,TAILLE_MSG_VOITURE,IPC_NOWAIT);
			}
			break;
		}
		case 'S' :
		{
			msgsnd(id_balSortie,&valeur,TAILLE_MSG_VOITURE,IPC_NOWAIT);
			break;
		}
	}
}//----- fin de Commande
