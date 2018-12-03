/**
 * \file defausse.c
 * \brief 
 * \version 1.0
 *
 * Contient la fonction de defausse de lettre pour le programme scrabble
 *
 */


#include <stdlib.h>
#include <stdio.h>
#include "../ressources/structureDeDonnee.h"
#include "../headers/defausse.h"
#include "../headers/pile.h"
#include "../headers/distribution.h"

/**
 * \fn void defausseParIndice (Chevalet chevalet, Pile *pPioche, int masqueDefausse[7])
 * \brief Remet les jetons choisis à un endroit aléatoire de la pioche
 *
 * \param chevalet chevalet dont les lettres sont défaussées
 * \param pPioche pioche
 * \param masqueDefausse 
 */
/* Remet les jetons choisis à un endroit aléatoire de la pioche */
void defausseParIndice (Chevalet chevalet, Pile *pPioche, int masqueDefausse[7])
{

    int i,k=0; // i : indice pour pourcourir le chevalet // k : indice pour compter les jetons vraiment défaussées //
    Jeton tablAux[102]; // tableau qui contiendra les jetons défaussés ainsi que tout ce qu'il reste dans la pioche
    for(i=0;i<7;i++)
    {
        if (masqueDefausse[i])
        {
            tablAux[k]=chevalet[i]; // on met le jeton dans un tableau auxiliaire
            chevalet[i].lettre='/'; // et on RAZ le jeton défaussé
            chevalet[i].valeur=0;
            k++;
        }
    }

    if (k != 0) // si des jetons ont été défaussés
    {
        while(*pPioche != NULL) // on vide la pioche dans le tableau auxiliaire
        {
            tablAux[k]=depiler(pPioche);
            k++;
        }
        empilageAleatoireJeton (tablAux,k, pPioche); // on rempile aléatoirement les jetons du tableau dans la pioche
        distributionJetons (pPioche,chevalet); // on distribue au joueur ses nouveaux jetons
    }
}
