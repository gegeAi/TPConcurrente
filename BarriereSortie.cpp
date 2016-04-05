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
#include <time.h>
//------------------------------------------------------ Include personnel

#include "Config.h"

#include <cstdio>

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

static int pid_voiturier[8] = {0, 0, 0, 0, 0, 0, 0, 0};

void end(int noSignal);
void run(int idSortieVoiture);
void finNormale(int noSignal);
void checkAutorisations();
void init(int idParking, int idCpt, int semCpt, int semParking, int idRequete[], int semRequete[], int idAutorisation[]);


void BarriereSortie(int idParking, int idRequete[], int idAutorisation[], int idSortieVoiture, int semParking, int semRequete[], int idCpt, int semCpt)
{
	//init
	init(idParking, idCpt, semCpt, semParking, idRequete, semRequete, idAutorisation);

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

void end(int noSignal)
{

	for(int i=0; i<8; i++)
	{
		if(pid_voiturier[i] != 0)
		{
			kill(pid_voiturier[i], SIGUSR2);
			waitpid(pid_voiturier[i], NULL, 0);
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

void run(int idSortieVoiture)
{
	while(true)
	{
		msgBaL * lecture = new msgBaL();
		while(msgrcv(idSortieVoiture, lecture, sizeof(int), 1, 0) == -1);
		pid_voiturier[lecture->place[0]-1] = SortirVoiture(lecture->place[0]);
		delete lecture;	
	}
}

void finNormale(int noSignal)
{
	int place;
	waitpid(0, &place, 0);
	place=WEXITSTATUS(place);

	Voiture v = parking[place-1];
	v.hSortie = time(NULL);
	AfficherSortie(v.type, v.num, v.hEntree, v.hSortie);

	while(semop(semCompteur, &reserver, 1)==-1);
	(*compteur)++;
	while(semop(semCompteur, &liberer, 1)==-1);
	switch(place-1)
	{
		case 0:
			Effacer(ETAT_P1);
			break;
		case 1:
			Effacer(ETAT_P2);
			break;
		case 2:
			Effacer(ETAT_P3);
			break;
		case 3:
			Effacer(ETAT_P4);
			break;
		case 4:
			Effacer(ETAT_P5);
			break;
		case 5:
			Effacer(ETAT_P6);
			break;
		case 6:
			Effacer(ETAT_P7);
			break;
		case 7:
			Effacer(ETAT_P8);
			break;
	}

	pid_voiturier[place-1]=0;
	checkAutorisations();
	
	
}


void checkAutorisations()
{
	if(*compteur==1)
	{
		int nChoisi = -1;
		while(semop(semReq[0], &reserver, 1)==-1);
		while(semop(semReq[1], &reserver, 1)==-1);
		while(semop(semReq[2], &reserver, 1)==-1);
		for(int i=0; i<3; i++)
		{
			if(nChoisi == -1 && requete[i]->used == 0)
			{
				nChoisi = i;
			}
			else if(requete[i]->used == 0)
			{
				if(requete[nChoisi]->type > requete[i]->type)
				{
					nChoisi=i;
				}
				else if(requete[nChoisi]->type == requete[i]->type)
				{
					if(requete[nChoisi]->hRequete > requete[i]->hRequete)
					{
						nChoisi=i;
					}
				}
			}
		}
		requete[nChoisi]->used = 1;
		switch(nChoisi)
		{
			case 0:
				Effacer(REQUETE_R1);
				break;
			case 1:
				Effacer(REQUETE_R2);
				break;
			case 2:
				Effacer(REQUETE_R3);
				break;
		}
		while(semop(idAuto[nChoisi], &liberer, 1)==-1);
			
		while(semop(semReq[0], &liberer, 1)==-1);
		while(semop(semReq[1], &liberer, 1)==-1);
		while(semop(semReq[2], &liberer, 1)==-1);
	}
}

void init(int idParking, int idCpt, int semCpt, int semParking, int idRequete[], int semRequete[], int idAutorisation[])
{
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

	//handler finVoiturier
	struct sigaction finVoiturier;
	finVoiturier.sa_handler = finNormale;
	sigemptyset(&finVoiturier.sa_mask);
	finVoiturier.sa_flags=0;
	sigaction(SIGCHLD, &finVoiturier, NULL);

	for(int i=0; i<3; i++)
	{
		requete[i] = (Requete*) shmat(idRequete[i], NULL, 0);
		semReq[i] = semRequete[i];
		idAuto[i] = idAutorisation[i];
	}
}


//----------------------------------------------------- Méthodes protégées

//------------------------------------------------------- Méthodes privées
