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
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
//------------------------------------------------------ Include personnel
#include "BarriereEntree.h"
#include "Config.h"
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
static Voiture n ;
static Voiture * parking;
static bool * voiturePresente;
static unsigned int * compteurPlace;
static pid_t noFils;
//operation p
struct sembuf semP = {0, -1,0};
//operation v
struct sembuf semV = {0, 1,0};
//----------------------------------------------------------- Types privés


//----------------------------------------------------------------- PUBLIC
//-------------------------------------------------------- Fonctions amies

//----------------------------------------------------- Méthodes publiques

static void ITFin(noSig)
{
	
}

static void ITFinFils(noSig)
{
	int place;
	
	/*//waitpid(noFils, &place, 0);
	semop(id_se, &semP,1 );
	*(parking+(place-1)) = newCar;
	semop(id_sem, &semV,1 );
	
	AfficherPlace(numPlace, typeUsager, numVoiture, tempsArrivee);*/
}

static void init(const char nomBal)
{
	//masquage de SIGUSR1 et SIGUSR2
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
    
    
    //récupération mémoires partagées
    id_mpReq = MPReq;
    voiturePresente = (Requete *) shmat(id_mpReq,NULL,0);
    
    id_mpParking = MPPark;
    parking = (Voiture *) shmat(id_mpParking,NULL,0);
    
    id_mpCompteur = MPCompteur;
    compteurPlace = (unsigned int *) shmat(id_mpCompteur,NULL,0);
}

/**
 * T0D0 : rajouter le numero de la boite au lettre dans param de BE
 */
void BarriereEntree(TypeBarriere barr, unsigned int semMP, unsigned int semSync, unsigned int semCompteur, unsigned int semPark,  unsigned int boiteAL, unsigned int MPReq, unsigned int MPPark, unsigned int MPCompteur)
{
	init(TypeBarriere barr, unsigned int semMP, unsigned int semSync, unsigned int semCompteur, unsigned int semPark, unsigned int boiteAL, unsigned int MPReq, unsigned int MPPark, unsigned int MPCompteur);
	
	for(;;)
	{
		
		//Recuperation du message
		while(msgrcv(id_bal, &newCar, TAILLE_MSG_VOITURE, 0, 0) == -1 && errno == EINTR);
		
		semop(id_semCompt, &semP,1 );
		if(*(compteurPlace)>0)
		{
			*(compteurPlace)--;
			semop(id_semCompt, &semV,1 );
			
			pid_t voiturier = GarerVoiture(barriere);
			int place;
			waitpid(noFils, &place, 0);
			
			semop(id_semPark, &semP,1 );
			*(parking+(place-1)) = newCar;
			semop(id_semPark, &semV,1 );
			
		}
		else
		{
			semop(id_semCompt, &semV,1 );
			
			//Depot de la requête
			semop(id_sem, &semP,1 );
			voiturePresente[numMemoire].type = newCar.type;
			voiturePresente[numMemoire].hRequete = newCar.hEntree;
			semop(id_sem, &semV,1 );
		
			
			// Demande de garage
			semop(id_sem, &semP,1 );
			while(shmat(id_mpReq, &newCar) == -1 && errno == EINTR);
			semop(id_sem, &semV,1 );
			
			// Attente de l'autorisation de garage
			semop(id_semSync, &semP,1 );
			
			semop(id_semCompt, &semP,1 );
			*(compteurPlace)--;
			semop(id_semCompt, &semV,1 );
			
			pid_t voiturier = GarerVoiture(barriere);
			int place;
			waitpid(noFils, &place, 0);
			
			semop(id_semPark, &semP,1 );
			*(parking+(place-1)) = newCar;
			semop(id_semPark, &semV,1 );
		}
		
		
		//AfficherRequete(barriere, newCar.type, newCar.hEntree);
		
		
		
		
	}
}


//------------------------------------------------------------------ PRIVE

//----------------------------------------------------- Méthodes protégées

//------------------------------------------------------- Méthodes privées
