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

//------------------------------------------------------ Include personnel
#include "Mere.h"

#include "Outils.h"
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
	//InitialiserApplication(XTERM);
	//creation memoires partagee
	int parking;
	int arriveeVoiture;
	int requete;
	int compteurVoiture;
	int semaphore;

	parking = shmget(ftok(REFERENCE, 'P'), sizeof(Voiture)*8, IPC_CREAT | IPC_EXCL | DROITS);
	arriveeVoiture = msgget(ftok(REFERENCE, 'A'), IPC_CREAT | IPC_EXCL | DROITS);
	requete = shmget(ftok(REFERENCE, 'R'), sizeof(Requete)*3, IPC_CREAT | IPC_EXCL | DROITS);
	compteurVoiture = shmget(ftok(REFERENCE, 'C'), sizeof(int), IPC_CREAT | IPC_EXCL | DROITS);


	semaphore = semget(ftok(REFERENCE, 'S'), 6, IPC_CREAT | DROITS);

	sleep(10);

	shmctl(parking, IPC_RMID, NULL);
	msgctl(arriveeVoiture, IPC_RMID, NULL);
	shmctl(requete, IPC_RMID, NULL);
	shmctl(compteurVoiture, IPC_RMID, NULL);
	semctl(semaphore, 0, IPC_RMID);

	

	//TerminerApplication(true);
	
	return 0;
}
