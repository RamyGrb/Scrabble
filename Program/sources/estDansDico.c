/**
 * \file estDansDico.c
 * \brief 
 * \version 1.0
 *
 * Programme pour voir si un mot appartient au dictionnaire. 
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include "../ressources/structureDeDonnee.h"
#include "../headers/estDansDico.h"

/** retourne VRAI si les deux mots sont identiques */
Booleen comparer ( char *mot1 , char *mot2 ) {
    // tant que mot1 n'est pas fini et que les lettres sont identiques (*mot1 et *mot2), '_' permet de prendre en compte le joker ('?'+32 ='_')
	while ( (*mot1 != '\0') && ((*mot2==*mot1)||(*mot1=='_')) )
	{
	    // on passe a la lettre suivante
		mot1++;
		mot2++;
	}

	if (*mot1 == '\0' && *mot2 == '\n')	 // vrai si les deux mots sont finis (dans dico.txt, les mots se terminent par un saut de ligne)
		return VRAI;
	else
		return FAUX;
}


/** retourne VRAI si le mot se trouve dans le dictionnaire */
Booleen estDansDico ( char *mot ) {
    char motDico[16];
    FILE *f;
    Booleen existence = FAUX;

	if ( ( f = fopen ( "ressources/dico.txt" , "r") ) == NULL )	{
		printf("erreur lors de l'ouverture du dictionnaire !\n");
		return FAUX;
	}

    while ( ( !existence ) &&( fgets ( motDico, 16, f ) != NULL ) )   // on va comparer mot a mot tous les mots du dico
        existence = comparer (mot, motDico);

    fclose (f);
    return existence;
}
