/**
 * \file grille.c
 * \brief 
 * \version 1.0
 *
 * Contient les fonctions de génération et d'affichage de la grille. 
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include "../ressources/structureDeDonnee.h"
#include "../headers/grille.h"

/** Initialisation d'un premier huitième de la grille */
void bonusPartiel (Grille grille) {

	int k,l,c;

	// Initialisation des cases
	for (l=0;l<=7;l++) {
		for (c=0;c<=l;c++){
			grille[l][c].multiMot=1;
			grille[l][c].multiLettre=1;
			grille[l][c].jeton.lettre='/';
			grille[l][c].jeton.valeur=0;
		}
	}

	// Mot compte triple
	grille[0][0].multiMot=3;
	grille[7][0].multiMot=3;

	// Mot compte double
	grille[7][7].multiMot=2;
	for (k=1;k<=4;k++)
		grille[k][k].multiMot=2;

	// Lettre compte double
	grille[3][0].multiLettre=2;
	grille[6][2].multiLettre=2;
	grille[6][6].multiLettre=2;
	grille[7][3].multiLettre=2;

	// Lettre compte triple
	grille[5][1].multiLettre=3;
	grille[5][5].multiLettre=2;
}


/** Obtention de la grille entière par symétries */
void symetries (Grille grille) {

	int l,c;
	// Triangle inférieur NO => Carré NO
	for (l=1;l<=7;l++) {
		for (c=0;c<l;c++) {
			grille[c][l]=grille[l][c];
		}
	}
	// Carré SO => Rectangle Sud
	for (l=0;l<=7;l++) {
		for (c=0;c<=6;c++) {
			grille[l][14-c]=grille[l][c];
		}
	}
	// Rectangle Sud => Grille entière
	for (l=0;l<=6;l++) {
		for (c=0;c<=14;c++) {
			grille[14-l][c]=grille[l][c];
		}
	}

}


/** génère la grille (combinaison des deux fonctions précédentes */
void genererGrille(Grille *pGrille) {

	bonusPartiel(*pGrille); // Bonus remplis sur le triangle inférieur Nord Ouest
	symetries(*pGrille); // Obtentien de la grille entière par symétries

}


/** affichage de la grille sur le terminal */
void afficherGrille(Grille grille) {

	int l,c;
	printf ("     1   2   3   4   5   6   7   8   9   10  11  12  13  14  15 \n"); // affichage des numéros des colonnes
	for (l=0;l<15;l++) {
	    if (l<9) {
	        printf (" ");
	    }
		printf(" %d ", l+1); // affichage des numéros des lignes
		for (c=0;c<15;c++) {
		    if (grille[l][c].jeton.lettre=='/') { // S'il n'y a pas de jeton posé sur la case
    			if (grille[l][c].multiMot != 1) // affichage du multiplicateur de mot
    				printf("[\033[35;1mm%d\033[37;22m]",grille[l][c].multiMot); // on met en couleur
    				//printf("[m%d]",grille[l][c].multiMot);
    			else if (grille[l][c].multiLettre != 1) // affichage du multiplicateur de lettre
    				printf("[\033[36;1ml%d\033[37;22m]",grille[l][c].multiLettre); // on met en couleur
    				//printf("[l%d]",grille[l][c].multiLettre);
    			else
    				printf("[  ]");  // Si aucun bonus on n'affiche rien
		    }
		    else  // si une lettre est présente, on l'affiche
		        //printf("[%c ]",grille[l][c].jeton.lettre);
		        printf("[\033[31m%c\033[37m ]",grille[l][c].jeton.lettre); // on met en couleur
		}
		printf("\n");
	}

}
