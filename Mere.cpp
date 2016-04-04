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
//#include "BarriereEntree.h"
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
	int semRequete;
	int semParking;

	parking = shmget(ftok(REFERENCE, 1), sizeof(Voiture)*8, IPC_CREAT | IPC_EXCL | DROITS);
	arriveeVoiture[0] = msgget(ftok(REFERENCE, 2), IPC_CREAT | IPC_EXCL | DROITS);
	arriveeVoiture[1] = msgget(ftok(REFERENCE, 3), IPC_CREAT | IPC_EXCL | DROITS);
	arriveeVoiture[2] = msgget(ftok(REFERENCE, 4), IPC_CREAT | IPC_EXCL | DROITS);
	sortieVoiture = msgget(ftok(REFERENCE, 5), IPC_CREAT | IPC_EXCL | DROITS);
	requete[0] = shmget(ftok(REFERENCE, 6), sizeof(Requete), IPC_CREAT | IPC_EXCL | DROITS);
	requete[1] = shmget(ftok(REFERENCE, 7), sizeof(Requete), IPC_CREAT | IPC_EXCL | DROITS);
	requete[2] = shmget(ftok(REFERENCE, 8), sizeof(Requete), IPC_CREAT | IPC_EXCL | DROITS);
	compteurVoiture = shmget(ftok(REFERENCE, 9), sizeof(int), IPC_CREAT | IPC_EXCL | DROITS);
	autorisation[0] = semget(ftok(REFERENCE, 10), 1, IPC_CREAT | IPC_EXCL | DROITS);
	autorisation[1] = semget(ftok(REFERENCE, 11), 1, IPC_CREAT | IPC_EXCL | DROITS);
	autorisation[2] = semget(ftok(REFERENCE, 12), 1, IPC_CREAT | IPC_EXCL | DROITS);
	semCpt = semget(ftok(REFERENCE, 13), 1, IPC_CREAT | IPC_EXCL | DROITS);
	semRequete = semget(ftok(REFERENCE, 14), 1, IPC_CREAT | IPC_EXCL | DROITS);
	semParking = semget(ftok(REFERENCE, 15), 1, IPC_CREAT | IPC_EXCL | DROITS);

	//creation taches

	if((gestionMenu = fork()) == 0)
	{
		GestionMenu();
	}
	else
	{
		if((barrieresEntree[0] = fork()) == 0)
		{
			//BarriereEntree(PROF_BLAISE_PASCAL, unsigned int semMP, autorisation[0], semCpt, arriveeVoiture[0], requete[0], parking, compteur)
			exit(0);
		}
		else
		{
			if((barrieresEntree[1] = fork()) == 0)
			{
				//BarriereEntree(AUTRE_BLAISE_PASCAL, unsigned int semMP, autorisation[1], semCpt, arriveeVoiture[1], requete[1], parking, compteur)
				exit(0);
			}
			else
			{
				if((barrieresEntree[2] = fork()) == 0)
				{
					//BarriereEntree(ENTREE_GASTON_BERGER, unsigned int semMP, autorisation[2], semCpt, arriveeVoiture[2], requete[2], parking, compteur)
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
	msgctl(arriveeVoiture[0], IPC_RMID, NULL);
	msgctl(arriveeVoiture[1], IPC_RMID, NULL);
	msgctl(arriveeVoiture[2], IPC_RMID, NULL);
	msgctl(sortieVoiture, IPC_RMID, NULL);
	shmctl(requete[0], IPC_RMID, NULL);
	shmctl(requete[1], IPC_RMID, NULL);
	shmctl(requete[2], IPC_RMID, NULL);
	shmctl(compteurVoiture, IPC_RMID, NULL);
	semctl(autorisation[0], 0, IPC_RMID);
	semctl(autorisation[1], 0, IPC_RMID);
	semctl(autorisation[2], 0, IPC_RMID);
	semctl(semCpt, 0, IPC_RMID);
	semctl(semRequete, 0, IPC_RMID);
	semctl(semParking, 0, IPC_RMID);

	TerminerApplication(true);
	
	return 0;
}
