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

	parking = shmget(ftok(REFERENCE, 1), sizeof(Voiture)*8, IPC_CREAT | DROITS);
	Voiture * leParking = (Voiture *) shmat(parking,NULL,0);
	leParking = new Voiture[8];

	for(int i=0; i<8; i++)
	{
		park[i].num=-1;
	}
	shmdt(leParking);
	semParking = semget(ftok(REFERENCE, 2), 1, IPC_CREAT | DROITS);
	semctl(semParking, 0, SETVAL, 1);

	for(int i=0; i<3; i++)
	{
		arriveeVoiture[i] = msgget(ftok(REFERENCE, 3+i), IPC_CREAT | DROITS);

		requete[i] = shmget(ftok(REFERENCE, 6+i), sizeof(Requete), IPC_CREAT | DROITS);
		Requete * req = (Requete*) shmat(requete[i], NULL, 0);
		req->used=1;
		shmdt(req);
		semRequete[i] = semget(ftok(REFERENCE, 9+i), 1, IPC_CREAT | DROITS);
		semctl(semRequete[i], 0, SETVAL, 1);

		autorisation[i] = semget(ftok(REFERENCE, 12+i), 1, IPC_CREAT | DROITS);
		semctl(autorisation[i], 0, SETVAL, 0);
		
	}

	sortieVoiture = msgget(ftok(REFERENCE, 15), IPC_CREAT | DROITS);

	compteurVoiture = shmget(ftok(REFERENCE, 16), sizeof(int), IPC_CREAT | DROITS);
	int * cpt = (int *) shmat(compteurVoiture, NULL, 0);
	*cpt=8;
	shmdt(cpt);
	semCpt = semget(ftok(REFERENCE, 17), 1, IPC_CREAT | DROITS);
	semctl(semCpt, 0, SETVAL, 1);	

	//creation taches

	if((barriereSortie = fork()) == 0)
	{
		BarriereSortie(parking, requete, autorisation, sortieVoiture, semParking, semRequete, compteurVoiture, semCpt);
	}
	else
	{
		if((barrieresEntree[0] = fork()) == 0)
		{
			BarriereEntree(PROF_BLAISE_PASCAL, semRequete[0], autorisation[0], semCpt, semParking, arriveeVoiture[0], requete[0], parking, compteurVoiture);
		}
		else
		{
			if((barrieresEntree[1] = fork()) == 0)
			{
				BarriereEntree(AUTRE_BLAISE_PASCAL, semRequete[1], autorisation[1], semCpt, semParking, arriveeVoiture[1], requete[1], parking, compteurVoiture);
			}
			else
			{
				if((barrieresEntree[2] = fork()) == 0)
				{
					BarriereEntree(ENTREE_GASTON_BERGER, semRequete[2], autorisation[2], semCpt, semParking, arriveeVoiture[2], requete[2], parking, compteurVoiture);
				}
				else
				{
					if((gestionMenu = fork()) == 0)
					{
						GestionMenu(arriveeVoiture[1], arriveeVoiture[0], arriveeVoiture[2], sortieVoiture);
					}
					else
					{
						heure = ActiverHeure();
						waitpid(gestionMenu, NULL, 0);
						kill(barriereSortie, SIGUSR2);
						waitpid(barriereSortie, NULL, 0);	
						for(int i=0; i<3; i++)
						{
							kill(barrieresEntree[i], SIGUSR2);
							waitpid(barrieresEntree[i], NULL, 0);
						}
						kill(heure, SIGUSR2);
						waitpid(heure, NULL, 0);

						//destruction ipcs
						shmctl(parking, IPC_RMID, NULL);
						semctl(semParking, 0, IPC_RMID);

						for(int i=0; i<3; i++)
						{
							msgctl(arriveeVoiture[i], IPC_RMID, NULL);
							shmctl(requete[i], IPC_RMID, NULL);
							semctl(semRequete[i], 0, IPC_RMID);
							semctl(autorisation[i], 0, IPC_RMID);
						}

						msgctl(sortieVoiture, IPC_RMID, NULL);

						shmctl(compteurVoiture, IPC_RMID, NULL);
						semctl(semCpt, 0, IPC_RMID);
							
						TerminerApplication(true);
					} // else barriereSortie
				}// else barriereEntree2
			}// else barriereEntree1
		}// else barriereEntree0
	}// else gestionMenu

	
	
	return 0;
}
