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
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sem.h>
//------------------------------------------------------ Include personnel

#include "Config.h"

//------------------------------------------------------------- Constantes

//---------------------------------------------------- Variables de classe

//----------------------------------------------------------- Types privés


//----------------------------------------------------------------- PUBLIC
//-------------------------------------------------------- Fonctions amies

//----------------------------------------------------- Méthodes publiques

static int pidFils[8] = {0,0,0,0,0,0,0,0};
static Voiture * parking;
static Requete * requete[3];
static int * compteur;

static struct sembuf reserver = {0, -1, 0};
static struct sembuf liberer = {0, 1, 0};
static int semCompteur;
static int semPark;

void checkAutorisations()
{
	
}

void run(int idAutorisation[], int idSortieVoiture, int semParking, int semRequete[], int semCpt)
{
	
	while(true)
	{
		msgBaL * lecture = new msgBaL();
		msgrcv(idSortieVoiture, lecture, sizeof(int), 0, 0);
		pidFils[lecture->place[0]] = SortirVoiture(lecture->place[0]);	
		delete lecture;	
	}
}

void end(int noSignal)
{
	for(int i=0; i<8; i++)
	{
		if(pidFils != 0)
		{
			kill(pidFils[i], SIGUSR2);
			waitpid(pidFils[i], NULL, 0); 
		}
	}

	// detachement	
	shmdt(parking);
	shmdt(compteur);
	for(int i=0; i<3; i++)
	{
		shmdt(requete[i]);
	}

	exit(0);
}

void finNormale(int noSignal)
{
	Voiture v = parking[noSignal];
	AfficherSortie(v.type, v.num, v.hEntree, v.hSortie);
	semop(semCompteur, &reserver, 1);
	*compteur++;
	semop(semCompteur, &liberer, 1);
	semop(semPark, &reserver, 1);
	parking[noSignal]=NULL;
	semop(semPark, &liberer, 1);
	pidFils[noSignal]=0;
	
}

void BarriereSortie(int idParking, int idRequete[], int idAutorisation[], int idSortieVoiture, int semParking, int semRequete[], int idCpt, int semCpt)
{
	//init
	parking = (Voiture*) shmat(idParking, NULL, 0);
	compteur = (int *) shmat(idCpt, NULL, 0);

	semCompteur = semCpt;
	semPark = semParking;

	//handler terminaison
	struct sigaction termine;
	termine.sa_handler = end;
	sigemptyset(&termine.sa_mask);
	termine.sa_flags=0;
	sigaction(SIGUSR2, &termine, NULL);

	//handler fin normale
	struct sigaction finSortie;
	finSortie.sa_handler = finNormale;
	sigemptyset(&finSortie.sa_mask);
	finSortie.sa_flags=0;
	sigaction(SIGCHLD, &finSortie, NULL);

	for(int i=0; i<3; i++)
	{
		requete[i] = (Requete*) shmat(idRequete[i], NULL, 0);
	}


	// detachement	
	shmdt(parking);
	shmdt(compteur);
	for(int i=0; i<3; i++)
	{
		shmdt(requete[i]);
	}
}


//------------------------------------------------------------------ PRIVE

//----------------------------------------------------- Méthodes protégées

//------------------------------------------------------- Méthodes privées
