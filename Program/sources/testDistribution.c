/**
 * \file testDistribution.c
 * \brief 
 * \version 1.0
 *
 * Programme de test de distribution. 
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //pour getpid
#include "../ressources/structureDeDonnee.h"
#include "../headers/distribution.h"

int main (void) {

    Jeton ensembleDesJetons[NOMBREDEJETON];
    int i,j;
    Pile *pPioche;
    pPioche=malloc(sizeof(Pile));
    
    Chevalet ensembleDesChevalets [4];
    
    srand ( getpid() ); // on initialise le rand
    
    distributionInitiale(ensembleDesJetons, pPioche, ensembleDesChevalets, 4);
    
    
    // Affichage des chevalets
    for (j=0;j<4;j++) {
        printf("Joueur %d : ",j+1);
        for (i=0;i<7;i++) {
            printf("[%c]",ensembleDesChevalets[j][i].lettre);
        }
        printf("\n");
    }
    
    // Affichage de la pioche
     printf("Pioche : ");
    Element elt = **pPioche;
    while(elt.lien!=NULL){
        printf("%c,", elt.jeton.lettre);
        elt=*(elt.lien);}
    printf("\n");
    
    free(pPioche);
    
    return EXIT_SUCCESS;
}