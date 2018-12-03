/**
 * \file distribution.c
 * \brief 
 * \version 1.0
 *
 * Contient les fonctions qui gèrent les jetons (génération, distribution, empilement). 
 *
 */


#include <stdlib.h>
#include <stdio.h>
#include "../ressources/structureDeDonnee.h"
#include "../headers/distribution.h"
#include "../headers/pile.h"


/** Construction du tableau des jetons dans l'ordre alphabétique */
void generationEnsembleDesJetons (Jeton ensembleDesJetons[NOMBREDEJETON]) {
  const char ensembleDesLettres[] = "AAAAAAAAABBCCDDDEEEEEEEEEEEEEEEFFGGHHIIIIIIIIJKLLLLLMMMNNNNNNOOOOOOPPQRRRRRRSSSSSSTTTTTTUUUUUUVVWXYZ??";
  //const char ensembleDesLettres[] = "??????????????????????????????????????????????????????????????????????????????????????????????????????";
  const int ensembleDesValeurs[] = {1,1,1,1,1,1,1,1,1,3,3,3,3,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,4,4,2,2,4,4,1,1,1,1,1,1,1,1,8,10,1,1,1,1,1,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,3,3,8,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,4,4,10,10,10,10,0,0};
  int i;
  for (i=0;i<NOMBREDEJETON;i++) {
    ensembleDesJetons[i].lettre = ensembleDesLettres[i];
    ensembleDesJetons[i].valeur = ensembleDesValeurs[i];
  }
}

/** Les jetons du tableau sont empilés aléatoirement dans la pioche */
void empilageAleatoireJeton (Jeton ensembleDesJetons[NOMBREDEJETON],int nbJetonsRestants, Pile *pPioche) {

  int indice;
  Jeton jetonTire;
  if (nbJetonsRestants > 1) {
    indice = rand() % nbJetonsRestants; // On tire un nombre au hasard indiquant un jeton de la réserve
    jetonTire = ensembleDesJetons [indice];
    empiler (pPioche,jetonTire); // on le met dans la pioche
    ensembleDesJetons [indice] = ensembleDesJetons [nbJetonsRestants-1]; // on le remplace par le dernier jeton de la pioche
    empilageAleatoireJeton (ensembleDesJetons,nbJetonsRestants-1,pPioche); // appels récursifs
  }
  if (nbJetonsRestants == 1)
    empiler (pPioche,ensembleDesJetons [0]);
}


/** On distribue le nombre de jetons nécessaires afin de compléter le chevalet du joueur */
void distributionJetons (Pile *pPioche,Chevalet chevalet) {
  int i;
  for (i=0;i<7;i++) {
    if (chevalet [i].lettre == '/') // on ne pioche que si l'emplacement n'a pas de chevalet
      chevalet[i] = depiler (pPioche);
  }
}


/** Intitialisation des chevalets des joueurs (appel aux 3 fonctions précédentes) */
void distributionInitiale(Jeton ensembleDesJetons[NOMBREDEJETON], Pile *pPioche, Chevalet ensembleDesChevalets [], int nbJoueurs ) {
    int j,i;
    for (j=0;j < nbJoueurs; j++) {
        for (i=0;i<7;i++) {
            ensembleDesChevalets [j] [i].lettre = '/';
            ensembleDesChevalets [j] [i].valeur = 0;
        }
    }
    generationEnsembleDesJetons (ensembleDesJetons);//On crée un tableau de Jetons.
    empilageAleatoireJeton (ensembleDesJetons, NOMBREDEJETON, pPioche);//On les empile aléatoirement dans la pioche

    for (j=0;j < nbJoueurs; j++)//Puis on distribue sept jetons à chaque joueur
        distributionJetons (pPioche,ensembleDesChevalets [j]);
}

