/**
 * \file verifMasqueGrille.c
 * \brief 
 * \version 1.0
 *
 * contient une des fonction de vérification de validité du mot posé. 
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include "../ressources/structureDeDonnee.h"


/** Verifie grossièrement si l'emplacement des lettres posees n'est pas incoherent */
int verifMasqueGrille(Grille grille, char *pSens,int *pL0, int *pC0, int *pN,int masqueGrille[15][15], int *pTouche, char message[200]) {
    int touche=0;
    int l,c;
    int scrabble=0;

// On vérifie que ça passe bien par le centre au tour 1
    if (grille[7][7].jeton.lettre=='/' && masqueGrille[7][7]==0) {
        sprintf(message,"ERREUR - Votre mot doit passer par la case centrale !");
        return 0;
    }

// On vérifie que le mot ne soit pas en diagonale et on récupère le sens et le nombre de lettre
    for(l=0;l<15;l++)
        for(c=0;c<15;c++)
            if(masqueGrille[l][c]==1){
                if (*pN==0) { //premier passage
                    *pL0=l; // on retient les coordonnées le la lettre la plus en haut à droite
                    *pC0=c; // on retient les coordonnées le la lettre la plus en haut à droite
                    *pSens='/';
                    (*pN)++; // on incrémente le compteur de lettre
                }
                else if (l==*pL0) { // horizontal
                    *pSens='H';
                    (*pN)++;

                }
                else if (c==*pC0) { // vertical
                    *pSens='V';
                    (*pN)++;
                }
                else if (l!=*pL0 && c!=*pC0) { //on est en train de lire un mot en diagonale
                    sprintf(message,"ERREUR - Votre mot n'est ni horizontal, ni vertical !");
                    return 0;
                }
            }

    if (*pN==7)
      scrabble=1;

// On vérifie que le mot ne comporte pas de 'trous' / que ça touche
    if (*pSens =='H')
        for (c=*pC0 ; c < *pC0 + *pN ; c++) {
            if(masqueGrille[*pL0][c]==0 && grille[*pL0][c].jeton.lettre == '/') { // entre la première lette et la dernière lettre posée il y a un trou
                sprintf(message,"ERREUR - Votre mot comporte des trous !");
                return 0;
            }
            else if (masqueGrille[*pL0][c]==0 && grille[*pL0][c].jeton.lettre != '/'){ // entre la première lette et la dernière lettre posée il y a une lettre (non posée ce tour ci)
                (*pN)++;
                touche=1;
            }
        }
    else if (*pSens =='V') // idem en vertical
        for (l=*pL0 ; l < *pL0 + *pN ; l++) {
            if(masqueGrille[l][*pC0]==0 && grille[l][*pC0].jeton.lettre == '/') {
                sprintf(message,"ERREUR - Votre mot comporte des trous !");
                return 0;
            }
            else if (masqueGrille[l][*pC0]==0 && grille[l][*pC0].jeton.lettre != '/'){
                (*pN)++;
                touche=1;
            }
        }
    *pTouche=touche;
 // Tout est OK
    if (scrabble)
      return 2;
    else
      return 1;
}
