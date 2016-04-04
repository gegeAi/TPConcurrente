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
#include "BarriereEntree.h"
#include "BarriereSortie.h"
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
	int requete[3];
	int compteurVoiture;
	int autorisation[3];
	int semCpt;
	int semRequete[3];
	int semParking;

	parking = shmget(ftok(REFERENCE, 1), sizeof(Voiture)*8, IPC_CREAT | 0660);
	sortieVoiture = msgget(ftok(REFERENCE, 2), IPC_CREAT | 0660);
	compteurVoiture = shmget(ftok(REFERENCE, 3), sizeof(int), IPC_CREAT | 0660);
	semCpt = semget(ftok(REFERENCE, 4), 1, IPC_CREAT | 0660);
	semParking = semget(ftok(REFERENCE, 5), 1, IPC_CREAT | 0660);
	semctl(semCpt, 0, SETVAL, 1);
	semctl(semParking, 0, SETVAL, 1);
	for(int i=0; i<3; i++)
	{
		arriveeVoiture[i] = msgget(ftok(REFERENCE, 6+i), IPC_CREAT | 0660);
		requete[i] = shmget(ftok(REFERENCE, 9+i), sizeof(Requete), IPC_CREAT | 0660);
		autorisation[i] = semget(ftok(REFERENCE, 12+i), 1, IPC_CREAT | 0660);
		semRequete[i] = semget(ftok(REFERENCE, 15+i), 1, IPC_CREAT | 0660);
		semctl(autorisation[i], 0, SETVAL, 1);
		semctl(semRequete[i], 0, SETVAL, 1);
	}

	//creation taches

	if((gestionMenu = fork()) == 0)
	{
		GestionMenu(arriveeVoiture[1], arriveeVoiture[0], arriveeVoiture[2], sortieVoiture);
	}
	else
	{
		if((barrieresEntree[0] = fork()) == 0)
		{
			BarriereEntree(PROF_BLAISE_PASCAL, semRequete[0], autorisation[0], semCpt, semParking, arriveeVoiture[0], requete[0], parking, compteurVoiture);
			exit(0);
		}
		else
		{
			if((barrieresEntree[1] = fork()) == 0)
			{
				BarriereEntree(AUTRE_BLAISE_PASCAL, semRequete[1], autorisation[1], semCpt, semParking, arriveeVoiture[1], requete[1], parking, compteurVoiture);
				exit(0);
			}
			else
			{
				if((barrieresEntree[2] = fork()) == 0)
				{
					BarriereEntree(ENTREE_GASTON_BERGER, semRequete[2], autorisation[2], semCpt, semParking, arriveeVoiture[2], requete[2], parking, compteurVoiture);
					exit(0);
				}
				else
				{
					if((barriereSortie = fork()) == 0)
					{
						BarriereSortie(parking, requete, autorisation, sortieVoiture, semParking, semRequete, compteurVoiture, semCpt);
						exit(0);
					}
					else
					{
						heure = ActiverHeure();
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
	msgctl(sortieVoiture, IPC_RMID, NULL);
	shmctl(compteurVoiture, IPC_RMID, NULL);
	semctl(semCpt, 0, IPC_RMID);
	semctl(semParking, 0, IPC_RMID);

	for(int i=0; i<3; i++)
	{
		msgctl(arriveeVoiture[i], IPC_RMID, NULL);
		shmctl(requete[i], IPC_RMID, NULL);
		semctl(semRequete[i], 0, IPC_RMID);
		semctl(autorisation[i], 0, IPC_RMID);
	}

	TerminerApplication(true);
	
	return 0;
}
