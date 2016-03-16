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

///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types
struct Voiture{
	
	char type;
	int num;
	int hEntree, hSortie, cout;
	
};
typedef struct Voiture Voiture;

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


static void init()
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
	
	//récupération des boîtes aux lettres
    key_t clefBALProfs = ftok(REFERENCE, 'R');
    id_balProfs = msgget(clefBALProfs, 0660);
    
    key_t clefBALAutres = ftok(REFERENCE, 'R');
    id_balAutres = msgget(clefBALAutres, 0660);
    
    key_t clefBALGB = ftok(REFERENCE, 'R');
    id_balGB = msgget(clefBALGB, 0660);
    
    key_t clefBALSortie = ftok(REFERENCE, 'R');
    id_balSortie = msgget(clefBALSortie, 0660);
    
    nouveau.num = 0;

}// fin de init

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques

/*type Nom ( liste de paramètres )
// Algorithme :
//
{
} //----- fin de Nom*/

void GestionMenu()
// Algorithme :
//
{
	init();
	char cmd;
	
	/*for(;;)
	{
		Menu();
	}*/
	sleep(20);
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
			exit(0);
		}
		case 'P' :
		{
			nouveau.type = 'P';
			nouveau.hEntree = time(NULL);
			nouveau.hSortie = 0;
			nouveau.cout = 0;
			nouveau.num = nouveau.num%1000;
			
			/*if(valeur == 1)
			{									
				if(/*msgsnd(id_balProfs,,,)==-1)
				{
					//Afficher(MESSAGE, "Voiture non creee", NORMALE);
				}
				else
				{
					//Afficher(MESSAGE, "", NORMALE);
				}
			}
			else
			{
				if(msgsnd(id_balGB,,,)==-1)
				{
					//Afficher(MESSAGE, "Voiture non creee", NORMALE);
				}
				else
				{
					//Afficher(MESSAGE, "", NORMALE);
				}
			}*/
		}
		case 'A' :
		{
		
			nouveau.type = 'A';
			nouveau.hEntree = time(NULL);
			nouveau.hSortie = 0;
			nouveau.cout = 0;
			nouveau.num = nouveau.num%1000;
			
			/*if(valeur == 1)
			{			
				if(msgsnd(id_balAutres,&nouveau,sizeof(Voiture),0)==-1)
				{
					//Afficher(MESSAGE, "Voiture non creee", NORMALE);
				}
				else
				{
					//Afficher(MESSAGE, "", NORMALE);
				}
			}
			else
			{
		
				if(msgsnd(id_balGB,&nouveau,sizeof(Voiture),0)==-1)
				{
					//Afficher(MESSAGE, "Voiture non creee", NORMALE);
				}
				else
				{
					//Afficher(MESSAGE, "", NORMALE);
				}
			}*/
		}
		case 'S' :
		{
			/*//Afficher(MENU,"Sortie - Choix de la place : [1] ... [8] Gaston Berger",NORMALE);
			if(msgsnd(id_balSortie,&valeur,sizeof(unsigned int),0)==-1)
			{
				//Afficher(MESSAGE, "Sortie non prise en compte", NORMALE);
			}
			else
			{
				//Afficher(MESSAGE, "", NORMALE);
			}*/
		}
	}
}//----- fin de Commande
