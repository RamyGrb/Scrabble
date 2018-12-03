/**
 * \file testEstDansDico.c
 * \brief 
 * \version 1.0
 *
 * Programme de test de vérification de présence d'un mot dans le dictionnaire. 
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "../ressources/structureDeDonnee.h"
#include "../headers/estDansDico.h"


int main ()
{
	char mot[30];
	
	printf("Veuillez entrer le mot à rechercher (Utilisez le caractère '_' pour simuler un jocker)\n");
	scanf("%s", mot);
	
	if (estDansDico (mot))
		printf ("Ce mot est dans le dictionnaire\n");
	else
		printf ("Ce mot n'est pas dans le dictionnaire\n");
	
	return EXIT_SUCCESS;
}