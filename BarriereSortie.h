/*************************************************************************
                           BarriereSortie  -  description
                             -------------------
    début                : 16/03/16
    copyright            : (C) BarriereSortie par jliermann
    e-mail               : jerome.liermann@insa-lyon.fr
*************************************************************************/

//---------- Interface de la tâche <BarriereSortie> (fichier BarriereSortie.h) -------
#if ! defined ( BARRIERE_SORTIE_H )                                               
#define BARRIERE_SORTIE_H

//------------------------------------------------------------------------
// Rôle de la tâche <BarriereSortie>
//
//
//------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////  INCLUDE
//--------------------------------------------------- Interfaces utilisées

//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques

void BarriereSortie(int idParking, int idRequete[], int idAutorisation[], int idSortieVoiture, int semParking, int semRequete[], int idCpt, int semCpt);
// Mode d'emploi :
//
// Contrat :
//

#endif // BARRIERE_SORTIE_H

