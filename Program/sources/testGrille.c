/**
 * \file testGrille.c
 * \brief 
 * \version 1.0
 *
 * Programme de test de génération de grille. 
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "../ressources/structureDeDonnee.h"
#include "../headers/grille.h"


int main(void) {
    
    Grille *pGrille;
    pGrille = malloc(sizeof(Grille));
    
    genererGrille(pGrille);
    afficherGrille(*pGrille);
    
    return EXIT_SUCCESS;
    
}