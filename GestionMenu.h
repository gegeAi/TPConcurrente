/*************************************************************************
                           GestionMenu  -  Interface GestionMenu
                             -------------------
    début                : 16/03/16
    copyright            : (C) GestionMenu par stoko
    e-mail               : samuel.toko@insa-lyon.fr
*************************************************************************/

//---------- Interface de la tâche <GestionMenu> (fichier GestionMenu.h) -------
#if ! defined ( GESTIONMENU_H )
#define GESTIONMENU_H

//------------------------------------------------------------------------
// Rôle de la tâche <GestionMenu>
//
//
//------------------------------------------------------------------------


//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques
void Commande( char code, unsigned int valeur);
// Mode d'emploi :
//
// Contrat :
//

void GestionMenu(unsigned int balAutres, unsigned int balProfs, unsigned int balGB, unsigned int balSortie);
// Mode d'emploi :
//
// Contrat :
//



#endif // GESTIONMENU_H
