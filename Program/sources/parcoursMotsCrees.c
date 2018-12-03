/**
 * \file parcoursMotsCrees.c
 * \brief 
 * \version 1.0
 *
 * Contient les fonctions de calcul de score et de vérification de mot. 
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "../ressources/structureDeDonnee.h"
#include "../headers/estDansDico.h"
#include "../headers/parcoursMotsCrees.h"

/** à partir des coordonnées d'une suite de lettre, détermine si le mot existe et, si tel est le cas, le nombre de point qu'il rapporte */
void calculerScore (int i1, int j1, int i2, int j2, Grille grille, Booleen *pExistence,int *pScore)
{
    char mot[16];    // permet de récupérer le mot a verifier
    int multiMot = 1;   //mutiplicateur sur mot du score
    int l,c;      // variables utilisées pour le parcours du tableau (for)
    if (i1==i2)  // mot horizontal
    {
        for (c=j1;c<=j2;c++)
        {
            mot[c-j1] = (grille [i1][c].jeton.lettre)+32;       //on met la suite de lettre posée dans "mot" (+32 pour obtenir des minuscules)
            *pScore += (grille [i1][c].jeton.valeur)*(grille [i1][c].multiLettre);     //recupere le score de la lettre (avec les multiplicateurs de lettre)
            multiMot *= grille [i1][c].multiMot;
        }
        mot[j2-j1+1]='\0'; // on marque la fin du mot
    }
    else if (j1==j2)        // mot vertical, idem
    {
        for (l=i1;l<=i2;l++)
        {
            mot[l-i1] = (grille [l][j1].jeton.lettre)+32;
            *pScore += (grille [l][j1].jeton.valeur)*(grille [l][j1].multiLettre);
            multiMot *= grille [l][j1].multiMot;
        }
        mot[i2-i1+1]='\0';
    }
    else{
	    printf("Erreur : mot en diagonale !\n");
    }

    *pExistence = estDansDico (mot);            //verifie l'existence du mot
    *pScore *= multiMot;                        //calcul le score du mot en multipliant par le multiplicateur de mot
}

/** recherche tous les mots créés par la dépose des lettres du jouer + détermination de la validité du tour + le cas échéant, du score remporté */
void parcoursDesMotsCrees (Grille grille, char sens, int l0, int c0, int n, Booleen *pTourValide,int *pScoreTour,int masqueGrille[15][15], int *pTouche,char message[200]) {
    int touche=0; // vérifie que le mot n'est pas isolé
	int deb,fin; // contiendra les indices de début et de fin des mots créés
	int score=0; // contiendra les scores temporaires (ceux de chaque mot)
	Booleen existence; // VRAI si le mot existe
    int l,c;

    sprintf(message,"ERREUR - ");

    if (masqueGrille[7][7]==1) touche=1; // si on pose une lettre sur la case centrale, c'est que c'est le premier tour donc le mot n'a pas besoin d'en toucher un autre

    // 3 cas. Ici, mot avec une seule lettre.
    if (n==1) {
        // cas d'une seule lettre isolée
        if (grille [l0-1] [c0].jeton.lettre == '/' && grille [l0+1] [c0].jeton.lettre == '/' && grille [l0] [c0-1].jeton.lettre == '/' && grille [l0] [c0+1].jeton.lettre == '/'){
            *pTourValide = FAUX;
            sprintf(message, "ERREUR - Vous ne pouvez pas poser une lettre isolée !");
            return;
        }

        // Parcours vertical
        deb=l0;
        fin=l0;
        //On cherche où commence le mot.
        while ( grille [deb] [c0].jeton.lettre != '/') {
            deb--;
            if(deb<0) {break;}
        }
        deb++;

		//On cherche où finit le mot.
		while ( grille [fin] [c0].jeton.lettre != '/' ) {
            fin++;
            if(fin>14) {break;}
        }
		fin--;

		if (fin>deb) {
		    touche=1;
			calculerScore (deb,c0,fin,c0,grille,&existence,&score);
			//La variable existence valide le mot entré (le mot existe bien), on peut donc augmenter pScoreTour qui contient le score total du tour
			if (existence) {*pScoreTour += score; score=0;}
            else {
                *pTourValide = FAUX;
                for(l=deb;l<=fin;l++)
                    sprintf(message,"%s%c",message,grille[l][c0].jeton.lettre);
                sprintf(message,"%s n'existe pas !", message);
                return;
            }
		}

        // Parcours horizontal, idem
		deb=c0;
        fin=c0;
        while ( grille [l0] [deb].jeton.lettre != '/') {
            deb--;
            if(deb<0) {break;}
        }
        deb++;

		while ( grille [l0] [fin].jeton.lettre != '/' ) {
            fin++;
            if(fin>14) {break;}
        }
		fin--;

		if (fin>deb) {
		    touche=1;
			calculerScore (l0,deb,l0,fin,grille,&existence,&score);
			if (existence) {*pScoreTour += score; score=0;}
            else {
                *pTourValide = FAUX;
                for(c=deb;c<=fin;c++)
                    sprintf(message,"%s%c",message,grille[l0][c].jeton.lettre);
                sprintf(message,"%s n'existe pas !", message);
                return;
            }
		}
    }

    // 2ème cas, le mot est vertical (et composé d'au moins deux lettres)
	else if (sens == 'V') {

        // Parcours vertical (mot formé en rallongeant la suite de lettre posée)
	    deb=l0;
        fin=l0+n-1;
        //On cherche où commence le mot.
        while ( grille [deb] [c0].jeton.lettre != '/') {
            deb--;
            if(deb<0) {break;}
        }
        deb++;
		//On cherche où finit le mot.
		while ( grille [fin] [c0].jeton.lettre != '/') {
            fin++;
            if(fin>14) {break;}
        }
		fin--;

		if(fin-deb>=n)
		    touche=1; // si le mot créé est plus grand que le nombre de lettre posé, c'est que ça touche

		if (fin>deb) {
			calculerScore (deb,c0,fin,c0,grille,&existence,&score);
			//La variable existence valide le mot entré (le mot existe bien), on peut donc augmenter pScoreTour
			if (existence) {*pScoreTour += score; score=0;}
            else {
                *pTourValide = FAUX;
                for(l=deb;l<=fin;l++)
                    sprintf(message,"%s%c",message,grille[l][c0].jeton.lettre);
                sprintf(message,"%s n'existe pas !", message);
                return;
            }
		}

        // parcours horizontaux (mots formés perpendiculairement à la suite le lettres posées)
		for (l=l0;l<l0+n;l++) {
		    if (masqueGrille[l][c0]==1) {
    		    deb=c0;
                fin=c0;
                while ( grille [l] [deb].jeton.lettre != '/') {
                    deb--;
                    if(deb<0) {break;}
                }
                deb++;
        		while ( grille [l] [fin].jeton.lettre != '/') {
                    fin++;
                    if(fin>14) {break;}
                }
        		fin--;

    		    if (fin-deb > 0) {
    		        touche=1;
                    calculerScore (l,deb,l,fin,grille,&existence,&score);
                    if (existence) {*pScoreTour += score; score=0;}
                    else {
                        *pTourValide = FAUX;
                        for(c=deb;c<=fin;c++)
                            sprintf(message,"%s%c",message,grille[l][c].jeton.lettre);
                        sprintf(message,"%s n'existe pas !", message);
                        return;
                    }
                }
		    }
		}
	}

	else if (sens == 'H') {    // 2ème cas, le mot est horizontal (et composé d'au moins deux lettres), idem

	    deb=c0;
        fin=c0+n-1;
        while ( grille [l0] [deb].jeton.lettre != '/') {
            deb--;
            if(deb<0) {break;}
        }
        deb++;
		while ( grille [l0] [fin].jeton.lettre != '/' ){
            fin++;
            if(fin>14) {break;}
        }
		fin--;

		if(fin-deb>=n)
		    touche=1;

		if (fin>deb) {
			calculerScore (l0,deb,l0,fin,grille,&existence,&score);
			if (existence) {*pScoreTour += score; score=0;}
            else {
                *pTourValide = FAUX;
                for(c=deb;c<=fin;c++)
                    sprintf(message,"%s%c",message,grille[l0][c].jeton.lettre);
                sprintf(message,"%s n'existe pas !", message);
                return;
            }
		}

		for (c=c0;c<c0+n;c++) {
		    if (masqueGrille[l0][c]==1) {
    		    deb=l0;
                fin=l0;
                while ( grille [deb] [c].jeton.lettre != '/'){
                    deb--;
                    if(deb<0) {break;}
                }
                deb++;
        		while ( grille [fin] [c].jeton.lettre != '/') {
                    fin++;
                    if(fin>14) {break;}
                }
        		fin--;

    		    if (fin-deb> 0) {
    		        touche=1;
                    calculerScore (deb,c,fin,c,grille,&existence,&score);
                    if (existence) {*pScoreTour += score; score=0;}
                    else {
                        *pTourValide = FAUX;
                        for(l=deb;l<=fin;l++)
                            sprintf(message,"%s%c",message,grille[l][c].jeton.lettre);
                        sprintf(message,"%s n'existe pas !", message);
                        return;
                    }
                }
		    }
		}
	}

	else {
          *pTourValide = FAUX;
	  sprintf(message,"ERREUR - mot en diagonale !");
	  return;
	}

    if (*pTouche) // si vérifMasqueGrille avait déjà vu que ça touchait, c'est que ça touche
        touche=1;

    if (!touche) { // si ça touche pas
        *pTourValide=FAUX;
        sprintf(message,"ERREUR - Votre mot doit toucher une lettre déjà présente !");
    }

}
