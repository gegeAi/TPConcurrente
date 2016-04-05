/*************************************************************************
                           BarriereEntree  -  Interface BarriereEntree
                             -------------------
    d�but                : 16/03/16
    copyright            : (C) BarriereEntree par stoko
    e-mail               : samuel.toko@insa-lyon.fr
*************************************************************************/

//---------- Interface de la t�che <BarriereEntree> (fichier BarriereEntree.h) -------
#if ! defined ( BARRIEREENTREE_H )
#define BARRIEREENTREE_H

//------------------------------------------------------------------------
// R�le de la t�che <BarriereEntree>
// 
// Cette t�che permet de g�rer l'arriv�e de voiture aux barri�res du parking
//------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////  INCLUDE
//--------------------------------------------------- Interfaces utilis�es
#include "Config.h"

//------------------------------------------------------------------ Types

//Structure comprenant le pid d'un voiturier en train de se garer ainsi que les infos de la voiture
typedef struct
{
	int id; //Obligatoire
	Voiture infoVoiture; //Information a envoy�
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

