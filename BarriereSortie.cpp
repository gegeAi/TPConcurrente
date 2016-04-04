/*************************************************************************
                           BarriereSortie  -  description
                             -------------------
    début                : 11/03/16
    copyright            : (C) 2016 par Toko Samuel
*************************************************************************/

//---------- Réalisation de la classe <BarriereSortie> (fichier BarriereSortie.cpp) --

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
# include <sys/shm.h>
//------------------------------------------------------ Include personnel

#include "Config.h"

//------------------------------------------------------------- Constantes

//---------------------------------------------------- Variables de classe

//----------------------------------------------------------- Types privés


//----------------------------------------------------------------- PUBLIC
//-------------------------------------------------------- Fonctions amies

//----------------------------------------------------- Méthodes publiques

static Voiture * parking;
static Requete * requete[3];
static int * compteur;

void run()
{
	while(true)
	{
		
	}
}

void BarriereSortie(int idParking, int idRequete[], int idAutorisation[], int idSortieVoiture, int semParking, int semRequete[], int idCpt, int semCpt)
{
	//init
	parking = (Voiture*) shmat(idParking, NULL, 0);
	compteur = (int *) shmat(idCpt, NULL, 0);

	for(int i=0; i<3; i++)
	{
		requete[i] = (Requete*) shmat(idRequete, NULL, 0);
	}


	// detachement	
	shmdt(parking);
	for(int i=0; i<3; i++)
	{
		shmdt(requete[i]);
	}
}


//------------------------------------------------------------------ PRIVE

//----------------------------------------------------- Méthodes protégées

//------------------------------------------------------- Méthodes privées
