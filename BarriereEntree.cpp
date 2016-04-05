/*************************************************************************
                           BarriereEntree  -  description
                             -------------------
    début                : 11/03/16
    copyright            : (C) 2016 par Toko Samuel
*************************************************************************/

//---------- Réalisation de la classe <BarriereEntree> (fichier BarriereEntree.cpp) --

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
#include <sys/wait.h>
#include <errno.h>
#include <sys/sem.h>
#include <sys/shm.h>
//------------------------------------------------------ Include personnel
#include "BarriereEntree.h"

#include "Outils.h"


//------------------------------------------------------------- Constantes

//---------------------------------------------------- Variables de classe
static int id_bal;
static int id_semReq;
static int id_semSync;
static int id_semCompt;
static int id_semPark;
static int id_mpReq;
static int id_mpParking;
static int id_mpCompteur;
static int numMemoire;
static TypeBarriere barriere;
static messVoiture newCar ;
static Voiture * parking;
static Requete * voiturePresente;
static unsigned int * compteurPlace;
static pid_t noFils;
static VoitureEnMouvement * mouvement;

//operation p
struct sembuf semP = {0, -1,0};
//operation v
struct sembuf semV = {0, 1,0};
//----------------------------------------------------------- Types privés


//----------------------------------------------------------------- PUBLIC
//-------------------------------------------------------- Fonctions amies

//----------------------------------------------------- Méthodes publiques

static void ITFin(int noSig)
{
	//Envoi de SIGUSR2 aux Voituriers
	for(int i=0;i<NB_PLACES;i++)
	{
		if(mouvement[i].id!=0)
		{
			kill(mouvement[i].id,SIGUSR2);
		}
	}
	
	//Detachement des memoires partagees
	shmdt(voiturePresente);
	shmdt(parking);
	shmdt(compteurPlace);
	
	exit(0);
}

static void ITFinFils(int noSig)
{
	
	int position,place,id;
	
	//Recuperation du pid du Voiturier
	id = waitpid(0, &place, 0);
	place = WEXITSTATUS(place);
	
	//Recherche des infos de la voitures qui vient de se garer
	for(int i=0;i<NB_PLACES;i++)
	{
		if(mouvement[i].id == id)
		{
			//Mise à jour du champ
			AfficherPlace(place, mouvement[i].infoVoiture.type, mouvement[i].infoVoiture.num, mouvement[i].infoVoiture.hEntree);
			
			//Ecriture de la voiture dans la memoire partagée Parking
			while(semop(id_semPark, &semP,1 )==-1);
			parking[place-1] = mouvement[i].infoVoiture;
			while(semop(id_semPark, &semV,1 )==-1);
			
			mouvement[i].id = 0;		
			break;
		}
	}
}

static void init(TypeBarriere barr, unsigned int semMP, unsigned int semSync, unsigned int semCompteur, unsigned int semPark,  unsigned int boiteAL, unsigned int MPReq, unsigned int MPPark, unsigned int MPCompteur)
{
	//masque les signaux SIGUSR1 et SIGUSR2
	struct sigaction masquage;
	masquage.sa_handler = SIG_IGN;
	sigemptyset (&masquage.sa_mask);
	masquage.sa_flags = 0;
	sigaction (SIGUSR1, &masquage, NULL);
	sigaction (SIGUSR2, &masquage, NULL);
	
	//mise en place du handler ITFin
	struct sigaction action;
	action.sa_handler = ITFin;
	sigemptyset (&action.sa_mask);
	action.sa_flags = 0;
	sigaction (SIGUSR2, &action, NULL);
	
	//mise en place du handler ITFinFils
	struct sigaction action2;
	action2.sa_handler = ITFinFils;
	sigemptyset (&action2.sa_mask);
	action2.sa_flags = 0;
	sigaction (SIGCHLD, &action2, NULL);
	
	//recuperation de la barriere
	barriere = barr;
	
	switch(barriere) {
		case PROF_BLAISE_PASCAL :
			numMemoire = 0;
			break;
			
		case AUTRE_BLAISE_PASCAL :
			numMemoire = 1;
			break;
		
		case ENTREE_GASTON_BERGER :
			numMemoire = 2;
			break;
	}
	
	//récupération de la boîte aux lettres
    id_bal = boiteAL;
    
    //récupération du sémaphore sur MP req
    id_semReq = semMP;
    
    //récupération du sémaphore de synchro
    id_semSync = semSync;
    
    //récupération du sémaphore de parking
    id_semPark = semPark;
    
    //récup sémaphore de compteur
    id_semCompt = semCompteur;
   
    //récupération mémoires partagées pour la requete
    id_mpReq = MPReq;
    voiturePresente = (Requete *) shmat(id_mpReq,NULL,0);
    
    //récupération mémoires partagées pour le parking
    id_mpParking = MPPark;
    parking = (Voiture *) shmat(id_mpParking,NULL,0);
    
    //récupération mémoires partagées pour le compteur
    id_mpCompteur = MPCompteur;
    compteurPlace = (unsigned int *) shmat(id_mpCompteur,NULL,0);
    
    mouvement = new VoitureEnMouvement[NB_PLACES];
    for(int i=0;i<NB_PLACES;i++)
    {
		mouvement[i].id=0;
	}
}

/**
 * T0D0 : rajouter le numero de la boite au lettre dans param de BE
 */
void BarriereEntree(TypeBarriere barr, unsigned int semMP, unsigned int semSync, unsigned int semCompteur, unsigned int semPark,  unsigned int boiteAL, unsigned int MPReq, unsigned int MPPark, unsigned int MPCompteur)
{
	init(barr, semMP, semSync, semCompteur, semPark, boiteAL, MPReq, MPPark, MPCompteur);
	
	for(;;)
	{
		//Recuperation du message
		while(msgrcv(id_bal, &newCar, sizeof(newCar.mVoiture), 1, 0) == -1);
		
		while(semop(id_semCompt, &semP,1 )==-1);
		Afficher(MESSAGE, *compteurPlace);
		if((*compteurPlace)>0)
		{
			//Mise à jour du nombre de place libre dans le Parking
			(*compteurPlace)--;
			while(semop(id_semCompt, &semV,1 )==-1);	
			
		}
		else
		{
			while(semop(id_semCompt, &semV,1 )==-1);	
						
			AfficherRequete(barriere, newCar.mVoiture.type, time(NULL));
			//Depot de la requête
			semop(id_semReq, &semP,1 );
			voiturePresente[numMemoire].type = newCar.mVoiture.type;
			voiturePresente[numMemoire].hRequete = newCar.mVoiture.hEntree;
			semop(id_semReq, &semV,1 );
			
			// Attente de l'autorisation de garage
			semop(id_semSync, &semP,1 );
			
			semop(id_semCompt, &semP,1 );
			(*compteurPlace)--;
			semop(id_semCompt, &semV,1 );
		}
		
		//Creation voiturier
		pid_t voiturier = GarerVoiture(barriere);
		
		newCar.mVoiture.hEntree = time(NULL);
		
		for(int i=0;i<NB_PLACES;i++)
		{
			if(mouvement[i].id==0)
			{
				mouvement[i].id=voiturier;
				mouvement[i].infoVoiture=newCar.mVoiture;
				break;
			}
		}
		
		sleep(TEMPO);
		//AfficherRequete(barriere, newCar.type, newCar.hEntree);
		

	}
}


//------------------------------------------------------------------ PRIVE

//----------------------------------------------------- Méthodes protégées

//------------------------------------------------------- Méthodes privées
