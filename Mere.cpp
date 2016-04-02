/*************************************************************************
                           Mere  -  description
                             -------------------
    début                : 
    copyright            : (C) 
    e-mail               : 
*************************************************************************/

//---------- Réalisation de la tâche <Mere> (fichier Mere.cpp) ---

/////////////////////////////////////////////////////////////////  INCLUDE
//-------------------------------------------------------- Include système

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

//------------------------------------------------------ Include personnel
#include "Mere.h"
#include "GestionMenu.h"
#include "Outils.h"
#include "Heure.h"
#include "Config.h"

///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//---------------------------------------------------- Variables statiques

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

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques

int main()
{
	InitialiserApplication(XTERM);

	//pid fils
	int gestionMenu;
	int barrieresEntree[3];
	int barriereSortie;
	int heure;

	//creation ipcs
	int parking;
	int arriveeVoiture[3];
	int sortieVoiture;
	int requete;
	int compteurVoiture;
	int semaphore;

	parking = shmget(ftok(REFERENCE, 1), sizeof(Voiture)*8, IPC_CREAT | IPC_EXCL | DROITS);
	arriveeVoiture[0] = msgget(ftok(REFERENCE, 2), IPC_CREAT | IPC_EXCL | DROITS);
	arriveeVoiture[1] = msgget(ftok(REFERENCE, 3), IPC_CREAT | IPC_EXCL | DROITS);
	arriveeVoiture[2] = msgget(ftok(REFERENCE, 4), IPC_CREAT | IPC_EXCL | DROITS);
	sortieVoiture = msgget(ftok(REFERENCE, 5), IPC_CREAT | IPC_EXCL | DROITS);
	requete = shmget(ftok(REFERENCE, 6), sizeof(Requete)*3, IPC_CREAT | IPC_EXCL | DROITS);
	compteurVoiture = shmget(ftok(REFERENCE, 7), sizeof(int), IPC_CREAT | IPC_EXCL | DROITS);
	semaphore = semget(ftok(REFERENCE, 8), 6, IPC_CREAT | IPC_EXCL | DROITS);

	//creation taches

	if((gestionMenu = fork()) == 0)
	{
		GestionMenu();
		exit(0);
	}
	else
	{
		if((barrieresEntree[0] = fork()) == 0)
		{
			exit(0);
		}
		else
		{
			if((barrieresEntree[1] = fork()) == 0)
			{
				exit(0);
			}
			else
			{
				if((barrieresEntree[2] = fork()) == 0)
				{
					exit(0);
				}
				else
				{
					if((barriereSortie = fork()) == 0)
					{
						exit(0);
					}
					else
					{
						heure = ActiverHeure();
						sleep(10);
						kill(gestionMenu, SIGUSR2);
						waitpid(gestionMenu, NULL, 0);
						kill(barrieresEntree[0], SIGUSR2);
						waitpid(barrieresEntree[0], NULL, 0);
						kill(barrieresEntree[1], SIGUSR2);
						waitpid(barrieresEntree[1], NULL, 0);
						kill(barrieresEntree[2], SIGUSR2);	
						waitpid(barrieresEntree[2], NULL, 0);
						kill(barriereSortie, SIGUSR2);
						waitpid(barriereSortie, NULL, 0);
						kill(heure, SIGUSR2);
						waitpid(heure, NULL, 0);
					} // else barriereSortie
				}// else barriereEntree2
			}// else barriereEntree1
		}// else barriereEntree0
	}// else getionMenu

	//destruction ipcs
	shmctl(parking, IPC_RMID, NULL);
	msgctl(arriveeVoiture[0], IPC_RMID, NULL);
	msgctl(arriveeVoiture[1], IPC_RMID, NULL);
	msgctl(arriveeVoiture[2], IPC_RMID, NULL);
	msgctl(sortieVoiture, IPC_RMID, NULL);
	shmctl(requete, IPC_RMID, NULL);
	shmctl(compteurVoiture, IPC_RMID, NULL);
	semctl(semaphore, 0, IPC_RMID);

	TerminerApplication(true);
	
	return 0;
}
