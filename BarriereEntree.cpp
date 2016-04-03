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
static int id_sem;
static int id_semSync;
static int id_mpReq;
static int id_mpParking;
static TypeBarriere barriere;
static Voiture newCar;
static Voiture * parking;
static bool * voiturePresente;
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
	
	waitpid(noFils, &place, 0);
	semop(id_se, &semP,1 );
	*(parking+(place-1)) = newCar;
	semop(id_sem, &semV,1 );
	
	AfficherPlace(numPlace, typeUsager, numVoiture, tempsArrivee);
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
	
	//récupération de la boîte aux lettres
    key_t clefBAL = ftok(REFERENCE, numBal);
    id_bal = msgget(clefBAL, 0660);
    
    //récupération du sémaphore sur MP req
    key_t clefSem = ftok("Req",1);
    id_sem = semget(clefSem,1);
    
    //récupération du sémaphore de synchro
    key_t clefSem = ftok("Synchro",1);
    id_semSync = semget(clefSem,1);
    
    
    //récupération mémoires partagées
    key_t clefMPReq = ftok(REFERENCE, 6);
    id_mpReq = shmget(clefMPReq,1);
    
    void *  mP_req = shmat(id_mpReq,NULL,0);
    voiturePresente = (bool *) mP_req;
    
    semop(id_sem, &semP,1 );
	voiturePresente = false;
	semop(id_sem, &semV,1 );
    
    key_t clefMPParking = ftok(REFERENCE, 1);
    id_mpParking = shmget(clefMPParking,1);
    
    void *  mP_Park = shmat(id_mpParking,NULL,0);
    parking = (Voiture *) mP_Park;
}

/**
 * T0D0 : rajouter le numero de la boite au lettre dans param de BE
 */
void BarriereEntree(int numBal)
{
	init(numBal);
	
	for(;;)
	{
		semop(id_sem, &semP,1 );
		voiturePresente = false;
		semop(id_sem, &semV,1 );
		
		//Recuperation du message
		while(msgrcv(id_bal, &newCar, TAILLE_MSG_VOITURE, 0, 0) == -1 && errno == EINTR);
		
		AfficherRequete(barriere, newCar.type, newCar.hEntree);
		
		
		semop(id_sem, &semP,1 );
		voiturePresente = true;
		semop(id_sem, &semV,1 );
		
		// Demande de garage
		semop(id_sem, &semP,1 );
		while(shmat(id_mpReq, &newCar) == -1 && errno == EINTR);
		semop(id_sem, &semV,1 );
		
		// Attente de l'autorisation de garage
		semop(id_semSync, &semP,1 );
		while(GarerVoiture(barriere) == -1);
		
		sleep(1);
		
		
	}
}


//------------------------------------------------------------------ PRIVE

//----------------------------------------------------- Méthodes protégées

//------------------------------------------------------- Méthodes privées
