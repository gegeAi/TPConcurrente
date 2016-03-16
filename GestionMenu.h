/*************************************************************************
                           GestionMenu  -  description
                             -------------------
    d�but                : 16/03/16
    copyright            : (C) GestionMenu par stoko
    e-mail               : samuel.toko@insa-lyon.fr
*************************************************************************/

//---------- Interface de la t�che <GestionMenu> (fichier GestionMenu.h) -------
#if ! defined ( GESTIONMENU_H )
#define GESTIONMENU_H

//------------------------------------------------------------------------
// R�le de la t�che <GestionMenu>
//
//
//------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////  INCLUDE
//--------------------------------------------------- Interfaces utilis�es

//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques
void Commande( char code, unsigned int valeur);
// Mode d'emploi :
//
// Contrat :
//

void GestionMenu(int noSig);
// Mode d'emploi :
//
// Contrat :
//



#endif // GESTIONMENU_H
