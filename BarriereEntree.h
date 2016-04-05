/*************************************************************************
                           BarriereEntree  -  Interface BarriereEntree
                             -------------------
    début                : 16/03/16
    copyright            : (C) BarriereEntree par stoko
    e-mail               : samuel.toko@insa-lyon.fr
*************************************************************************/

//---------- Interface de la tâche <BarriereEntree> (fichier BarriereEntree.h) -------
#if ! defined ( BARRIEREENTREE_H )
#define BARRIEREENTREE_H

//------------------------------------------------------------------------
// Rôle de la tâche <BarriereEntree>
// 
// Cette tâche permet de gérer l'arrivée de voiture aux barrières du parking
//------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////  INCLUDE
//--------------------------------------------------- Interfaces utilisées
#include "Config.h"

//------------------------------------------------------------------ Types

//Structure comprenant le pid d'un voiturier en train de se garer ainsi que les infos de la voiture
typedef struct
{
	int id; //Obligatoire
	Voiture infoVoiture; //Information a envoyé
} VoitureEnMouvement;

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques

void BarriereEntree(TypeBarriere barr, unsigned int semMP, unsigned int semSync, unsigned int semCompteur, 
					unsigned int semPark, unsigned int boiteAL,unsigned int MPReq, unsigned int MPPark, unsigned int MPCompteur);
// Mode d'emploi : 
//
// Contrat : 
//

#endif // BARRIEREENTREE_H

