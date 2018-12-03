/**
 * \file pile.c
 * \brief 
 * \version 1.0
 *
 * Contient des fonctions de gestion de la pile. 
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "../ressources/structureDeDonnee.h"
#include "../headers/pile.h"

Pile nouvellePile (void) {
  return NULL;
}

void empiler (Pile *ppile, Jeton jet) {
  Pile pTmp;
  if ( (pTmp = malloc (sizeof (Element)) ) == NULL ) {
		printf ( "Erreur allocation memoire" );
		exit ( EXIT_FAILURE );
	}
  pTmp->jeton = jet;
  pTmp->lien=*ppile;
  *ppile=pTmp;
}

Jeton depiler (Pile *ppile) {
	Jeton jet;
	if (*ppile==NULL) {
		jet.valeur=0;
		jet.lettre='/';
	}
	else {
		jet = (*ppile)->jeton;
		*ppile = (*ppile)->lien;
	}
	return jet;
}
