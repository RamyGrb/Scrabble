/**
 * \file testDefausse.c
 * \brief 
 * \version 1.0
 *
 * Programme de test de defausse. 
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //pour getpid
#include "../ressources/structureDeDonnee.h"
#include "../headers/distribution.h"
#include "../headers/defausse.h"


void afficherChevalet (Chevalet chevalet) {
    
    int i;
    printf("Votre chevalet : ");
    for (i=0;i<7;i++) {
        printf("[%c]",chevalet[i].lettre);
    }
    printf("\n");
       
}

int main (void) {

    int i;
    
    Jeton ensembleDesJetons[NOMBREDEJETON];
    Pile *pPioche;
    Chevalet ensembleDesChevalets [2];
    pPioche=malloc(sizeof(Pile));
    
    srand ( getpid() ); // on initialise le rand
    
    distributionInitiale(ensembleDesJetons, pPioche, ensembleDesChevalets, 2);
    afficherChevalet (ensembleDesChevalets[0]);
    
    int masqueDefausse[7];
    
    for (i=0;i<7;i++) {
        printf("Se défausser du jeton n°%d ? (1 pour oui, 0 pour non)\n", i+1);
        scanf("%d",&(masqueDefausse[i]));
        while (getchar () != '\n');
    }
    
    defausseParIndice (ensembleDesChevalets[0], pPioche, masqueDefausse);
    afficherChevalet (ensembleDesChevalets[0]);
    
    free(pPioche);
    
    return EXIT_SUCCESS;
}