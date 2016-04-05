/*************************************************************************
                           GestionMenu  -  description
                             -------------------
    début                : ${date}
    copyright            : (C) ${year} par ${user}
*************************************************************************/

//---------- Réalisation de la tache GestionMenu (fichier ${file_name}) --

//---------------------------------------------------------------- INCLUDE

//-------------------------------------------------------- Include système

#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
//#include <time.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
//------------------------------------------------------ Include personnel
#include "GestionMenu.h"
#include "Menu.h"

#include "Outils.h"
#include "Config.h"
using namespace std;
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
static messVoiture nouveau;
static msgBaL voitureASortir;

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
	//Masque les signaux SIGUSR1 et SIGUSR2
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
	
	//Initialisation du numero des voitures
    nouveau.mVoiture.num = 0;

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
	//Lancement de la phase d'initialisation de la tâche
	init(balAutres, balProfs, balGB, balSortie);
	
	//Lancement de la phase moteur de la tâche
	for(;;)
	{
		Menu();
	}
	
}//----- fin de GestionMenu



void Commande( char code, unsigned int valeur)
// Algorithme :
//
{
	switch(code)
	{
		//quitte l'application
		case 'E' :
		{
			
			exit(0);
		}
		case 'P' :
		{
			//Mise en place des attributs de la voiture
			nouveau.mVoiture.type = PROF;
			nouveau.mVoiture.num = (nouveau.mVoiture.num+1)%1000;
			nouveau.mtype = 1;

			//Envoi du message à la boite au lettre correspondante
			if(valeur == 1)
			{									
				msgsnd(id_balProfs,&nouveau,sizeof(nouveau.mVoiture),IPC_NOWAIT);
			}
			else
			{
				msgsnd(id_balGB,&nouveau,TAILLE_MSG_VOITURE,IPC_NOWAIT);

			}
			break;
		}
		case 'A' :
		{
		
			//Mise en place des attributs de la voiture
			nouveau.mVoiture.type = AUTRE;
			nouveau.mVoiture.num = (nouveau.mVoiture.num+1)%1000;
			nouveau.mtype = 1;
			
			//Envoi du message à la boite au lettre correspondante
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
			//Mise à jour des paramètres du message
			voitureASortir.place[0]=valeur;
			voitureASortir.mtype=1;
			
			//Envoi du message à la boite au lettre de sortie
			Afficher(MESSAGE,msgsnd(id_balSortie,&voitureASortir,sizeof(int),IPC_NOWAIT));
			
			break;
		}
	}
}//----- fin de Commande
