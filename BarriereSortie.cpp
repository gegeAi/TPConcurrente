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

static Voiture * parking;
static Requete * requete[3];
static int * compteur;

static struct sembuf reserver = {0, -1, 0};
static struct sembuf liberer = {0, 1, 0};
static int semCompteur;
static int semPark;
static int semReq[3];
static int idAuto[3];
static int currentSortie=0;

void checkAutorisations()
{
	if((*compteur) < 3)
	{
		int nChoisi = -1;
		while(semop(semReq[0], &reserver, 1)==-1);
		while(semop(semReq[1], &reserver, 1)==-1);
		while(semop(semReq[2], &reserver, 1)==-1);
		for(int j=0; j<(*compteur); j++)
		{
			for(int i=0; i<3; i++)
			{
				if(nChoisi == -1 && requete[i] != NULL)
				{
					nChoisi = i;
				}
				else if(nChoisi != -1 && requete[i] != NULL)
				{
					if(requete[i]->type==PROF)
					{
						if(requete[nChoisi]->type!=PROF || (requete[nChoisi]->type==PROF && requete[i]->hRequete < requete[nChoisi]->hRequete))
						{
							nChoisi = i;
						}
					}
				}
			}

			requete[nChoisi]=NULL;
			while(semop(idAuto[nChoisi], &liberer, 1)==-1);
			
		}
		while(semop(semReq[0], &liberer, 1)==-1);
		while(semop(semReq[1], &liberer, 1)==-1);
		while(semop(semReq[2], &liberer, 1)==-1);
	}
}

void finNormale(int noPlace)
{
	Voiture v = parking[noPlace];
	AfficherSortie(v.type, v.num, v.hEntree, v.hSortie);
	while(semop(semCompteur, &reserver, 1)==-1);
	(*compteur)++;
	while(semop(semCompteur, &liberer, 1)==-1);
	checkAutorisations();
	
}

void run(int idSortieVoiture)
{
	while(true)
	{
		msgBaL * lecture = new msgBaL();
		msgrcv(idSortieVoiture, lecture, sizeof(int), 1, 0);
		currentSortie = SortirVoiture(lecture->place[0]);
		int place=lecture->place[0];	
		//waitpid(currentSortie, &place, 0);
		finNormale(place);
		currentSortie=0;
		delete lecture;	
	}
}

void end(int noSignal)
{
	if(currentSortie != 0)
	{
		kill(currentSortie, SIGUSR2);
		waitpid(currentSortie, NULL, 0);
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

	/*//handler fin normale
	struct sigaction finSortie;
	finSortie.sa_handler = finNormale;
	sigemptyset(&finSortie.sa_mask);
	finSortie.sa_flags=0;
	sigaction(SIGCHLD, &finSortie, NULL);*/

	for(int i=0; i<3; i++)
	{
		requete[i] = (Requete*) shmat(idRequete[i], NULL, 0);
		semReq[i] = semRequete[i];
		idAuto[i] = idAutorisation[i];
	}

	run(idSortieVoiture);

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
