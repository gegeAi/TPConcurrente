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
#include "Stats_C_Tab.h"

//------------------------------------------------------------- Constantes

//---------------------------------------------------- Variables de classe
static int id_bal;
static int id_sem;
static int id_mpReq;
static int id_mpParking;
//----------------------------------------------------------- Types privés


//----------------------------------------------------------------- PUBLIC
//-------------------------------------------------------- Fonctions amies

//----------------------------------------------------- Méthodes publiques

static void ITFin(noSig)
{
	
}

static void ITFinFils(noSig)
{
	
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
	sigaction (SIGUSR1, &action, NULL);
	sigaction (SIGUSR2, &action, NULL);
	
	//mise en place du handler ITFinFils
	struct sigaction action2;
	action2.sa_handler = ITFinFils;
	sigemptyset (&action2.sa_mask);
	action2.sa_flags = 0;
	sigaction (SIGUSR1, &action2, NULL);
	sigaction (SIGUSR2, &action2, NULL);
	
	//récupération de la boîte aux lettres
    key_t clefBAL = ftok(nomBal, 1);
    id_bal = msgget(clefBAL, 0660);
    
    //récupération du sémaphore
    key_t clefSem = ftok("Synchro",1);
    id_sem = semget(clefSem,1);
    
    
    //récupération mémoires partagées
    key_t clefMPReq = ftok("Requete",1);
    id_mpReq = shmget(clefMPReq,1);
    
    key_t clefMPReq = ftok("Parking",1);
    id_mpParking = shmget(clefMPParking,1);;
}


void BarriereEntree(const char * nomBal)
{
	init(nomBal);
	
	/*for(;;)
	{
		//Recuperation du message
		MsgVoiture newCar;
		while(msgrcv(id_bal, &newCar, TAILLE_MSG_VOITURE, typeVoie, 0) == -1 && errno == EINTR);
		
		
	}*/
}


//------------------------------------------------------------------ PRIVE

//----------------------------------------------------- Méthodes protégées

//------------------------------------------------------- Méthodes privées
