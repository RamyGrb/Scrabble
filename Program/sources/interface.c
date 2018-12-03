/**
 * \file interface.c
 * \brief
 * \version 2.0
 *
 * Contient le programme principal (main) et gère l'interface graphique
 *
 */


#include "../headers/interface.h"



//Definitions des variables globales////////////////////////////////////////////////////////////////////////////

/**fenetre utilisee pour le choix du nombre de joueur et pour le jeu*/
GtkWidget *pWindow;
/**fenetre utilisee pour l'affichage du vainqueur*/
GtkWidget *pWindow2;
/**table principale de la fenetre de jeu */
GtkWidget *pTable;
/**cadre contenant la zone d'affichage du score */
GtkWidget *pZoneScore;
/**vbox contenant les label affichant le score*/
GtkWidget *pScoreBox;
/**table contenant les boutons constituant la grille */
GtkWidget *pGrille;
/**tableau de widget*, il contient les boutons de la grille */
ImageCliquable pQuadrillage[15][15];
/**table contenant les widgets de la zone joueur*/
GtkWidget *pZoneJoueur;
/**vbox contenant les boutons pAnnuler et pDefausser*/
GtkWidget *pActionsBox;
/**bouton permettant d'annuler sa pose de mot*/
GtkWidget *pAnnuler;
/**bouton permettant de lancer la validation du mot pose*/
GtkWidget *pOK;
/**bouton permettant de lancer la procedure de defausse*/
GtkWidget *pDefausser;
/**tableau de widget*, il contient les boutons du chevalet*/
ImageCliquable pChevalet[7];
/**label contenant le numero du joueur dont c'est le tour*/
GtkWidget *pJoueur;
/**label permettant d'afficher le score du joueur*/
GtkWidget *pScoreJoueurs[4];
/**bouton utilise lors de la transition entre 2 joueurs*/
GtkWidget *pPret;
/**Le nombre de joueurs*/
int nbJoueurs;
/**Le numero du joueur en cours*/
int noJoueur=1;
/**Le nombre de lettres posees*/
int nbLettresPosees=0;
/**Le tableau des 4 joueurs (ou moins)*/
int score[4];
/**la grille telle que le terminal la "voit" (et non pas ce que l'utilisateur voir sur l'interface graphique)*/
Grille *pGrilleVirtuelle;
/**idem avec les chevalets*/
Chevalet ensembleDesChevaletsVirtuels [4];
/**La pioche*/
Pile *pPioche;
/**emplacement pour sauvegarder les chevalets*/
Chevalet ancienEnsembleDesChevaletsVirtuels[4] ;
/**emplacement pour sauvegarder la grille*/
Grille ancienneGrilleVirtuelle  ;
/**indice du jeton choisi par le joueur sur son chevalet*/
int indiceLettreChoisie;
/**vaut 1 si la lettre correspondante du chevalet du joueur actuelle doit etre defaussee*/
int masqueDefausse[7];
/**vaut 1 si la case correspondante sur la grille doit recevoir une lettre*/
int masqueGrille[15][15];
/**identifiants des g_signal_connect des boutons du chevalets*/
gulong signauxChevalet[7]={0,0,0,0,0,0,0};
/**identifiant du g_signal_connect du bouton OK*/
gulong signalOK;
/**identifiant du g_signal_connect de la fermeture de la fenetre de fin de partie*/
gulong signalFermerFenetre2;
/**Pour chaque joueur : vaut 1 si le joueur a passe son tour (pour la fin de la partie)*/
int passeurs[4];
/**contient d'eventuels messages d'erreurs (ex : "ERREUR - Votre mot doit passer par la case centrale")*/
char message[200];


int main (int argc, char **argv)
{
    srand ( getpid() ); // on initialise le rand
	gtk_init(&argc, &argv);
	choisirNbJoueurs();
	return EXIT_SUCCESS;
}


/** Affichage de la fenetre permettant de choisir combien de joueurs vont jouer : on attend le choix du joueur */
void choisirNbJoueurs ()
{
	GtkWidget *PNbPlayer[4];
	GtkWidget *pTableAccueil;


//Mise en place de la fenetre
	if (pWindow!=0)
	{
		gtk_widget_destroy(pWindow);
	}
	pWindow = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size (GTK_WINDOW(pWindow), 400, 200);
	gtk_window_set_title (GTK_WINDOW(pWindow), "SCRABBLE");

//Mise en place de la table
	pTableAccueil = gtk_table_new (5 , 4 , TRUE );
	gtk_container_add (GTK_CONTAINER(pWindow) , GTK_WIDGET(pTableAccueil));

//Mise en place des boutons
	//PNbPlayer[0]=gtk_button_new_with_label ("1 Joueur");
	PNbPlayer[1]=gtk_button_new_with_label ("2 Joueurs");
	PNbPlayer[2]=gtk_button_new_with_label ("3 Joueurs");
	PNbPlayer[3]=gtk_button_new_with_label ("4 Joueurs");
	//gtk_table_attach_defaults (GTK_TABLE(pTableAccueil), PNbPlayer[0]  , 1 , 2 , 1 , 2);
	gtk_table_attach_defaults (GTK_TABLE(pTableAccueil), PNbPlayer[1]  , 1 , 3 , 1 , 2);
	gtk_table_attach_defaults (GTK_TABLE(pTableAccueil), PNbPlayer[2]  , 1 , 3 , 2 , 3);
	gtk_table_attach_defaults (GTK_TABLE(pTableAccueil), PNbPlayer[3]  , 1 , 3 , 3 , 4);

//affichage de la fenetre
	g_signal_connect(G_OBJECT(pWindow), "destroy", G_CALLBACK(onDestroy), NULL);
	//g_signal_connect(G_OBJECT(PNbPlayer[0]), "clicked" , G_CALLBACK(nouvellePartie), GINT_TO_POINTER(1));
	g_signal_connect(G_OBJECT(PNbPlayer[1]), "clicked" , G_CALLBACK(nouvellePartie), GINT_TO_POINTER(2));
	g_signal_connect(G_OBJECT(PNbPlayer[2]), "clicked" , G_CALLBACK(nouvellePartie), GINT_TO_POINTER(3));
	g_signal_connect(G_OBJECT(PNbPlayer[3]), "clicked" , G_CALLBACK(nouvellePartie), GINT_TO_POINTER(4));
	gtk_widget_show_all(pWindow);
	gtk_main();
}


/** Fermeture de la fenetre en cours */
void onDestroy(GtkWidget *pWidget, gpointer pData)
{
//Arret de la boucle evenementielle
	gtk_main_quit();
}


/** Fermeture de toutes les fenetres */
void toutFermer (GtkWidget *pWidget, gpointer pData)
{
    g_signal_handler_disconnect (G_OBJECT(pWindow), signalFermerFenetre2);
//Arret de la boucle evenementielle
	gtk_main_quit();
}


/** Fermeture de toutes les fenetres + Reouverture de celle du choix du nombre de joueur  */
void rejouer (GtkWidget *pWidget, gpointer pData)
{
	gtk_main_quit();
    gtk_widget_destroy (pWindow2);

	free(pGrilleVirtuelle);
	free(pPioche);
	int i;
    for (i=0;i<7;i++) {
        signauxChevalet[i]=0;
    }
	choisirNbJoueurs();
}


/** Fermeture de la fenetre de jeu et retour au choix du nombre de joueur */
void onDestroy2(GtkWidget *pWidget, gpointer pData)
{
	gtk_main_quit();
	free(pGrilleVirtuelle);
	free(pPioche);
	int i;
    for (i=0;i<7;i++) {
        signauxChevalet[i]=0;
    }
	choisirNbJoueurs();
}


/** Mise a jour des textes affiches sur les boutons de l'interface graphique au niveau du chevalet affiche */
void miseAJourChevalet( ImageCliquable pChevalet[7], Chevalet chevaletVirtuel) {
    int i;
    char label[40];
    for (i=0;i<7;i++) {
        if (chevaletVirtuel[i].lettre == '/')
            gtk_imagecliquable_set_image_from_file ( &(pChevalet[i]) , NULL);
        else if (chevaletVirtuel[i].lettre == '?'){
            sprintf(label,"ressources/jetons/jocker.png");
            gtk_imagecliquable_set_image_from_file ( &(pChevalet[i]) , label);
        }
        else {
            sprintf(label,"ressources/jetons/%c.png", chevaletVirtuel[i].lettre);
            gtk_imagecliquable_set_image_from_file ( &(pChevalet[i]) , label);
        }

    }
}


/** Mise a jour des textes affiches sur les boutons de l'interface graphique au niveau de la grille */
void miseAJourGrille(ImageCliquable pQuadrillage[15][15], Grille grille) {

	int l,c;
	char label[40];

	for (l=0;l<15;l++) {
		for (c=0;c<15;c++) {
		    if (grille[l][c].jeton.lettre=='/') {
                if (grille[l][c].multiMot != 1)
                        sprintf(label,"ressources/grille/m%d.png",grille[l][c].multiMot);
                else if (grille[l][c].multiLettre != 1)
                    sprintf(label,"ressources/grille/l%d.png",grille[l][c].multiLettre);
                else
                    sprintf(label,"ressources/grille/vide.png");
                }
		    else if (grille[l][c].jeton.lettre=='?')
		        sprintf(label,"ressources/jetons/jocker.png");
            else
		        sprintf(label,"ressources/jetons/%c.png",grille[l][c].jeton.lettre);

        gtk_imagecliquable_set_image_from_file (&(pQuadrillage[l][c]) , label);
		}
	}
	if (grille[7][7].jeton.lettre=='/') {
	gtk_imagecliquable_set_image_from_file (&(pQuadrillage[7][7]) , "ressources/grille/centre.png");
	}
}

/** Demarrage d'une nouvelle partie : creation des fenetres, de tous les boutons. On attend que le 1er joueur soit pret a commencer */
void nouvellePartie (GtkWidget *pWidget, gpointer pNbJoueurs)
{
//suppression de la fenetre choisirNbJoueurs et recuperation du nombre de joueurs
	gtk_main_quit();
	gtk_widget_destroy (pWindow);
	nbJoueurs = GPOINTER_TO_INT(pNbJoueurs);

//initialisation de la fenetre (pWindow)
	pWindow = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size (GTK_WINDOW(pWindow), 880, 800);
	gtk_window_set_title (GTK_WINDOW(pWindow), "SCRABBLE");
	gtk_window_set_modal(GTK_WINDOW(pWindow), TRUE);

//mise en place de la table
	pTable  = gtk_table_new (10 , 11 , TRUE);
	//gtk_table_set_col_spacing (GTK_TABLE(pTable) , 3 , 100);
	gtk_container_add (GTK_CONTAINER(pWindow) , GTK_WIDGET(pTable));

//mise en place du score
	int i;
    for(i=0;i<4;i++) {
        score[i]=0;
    }
	pZoneScore = gtk_frame_new ("Score:");
	gtk_frame_set_shadow_type (GTK_FRAME(pZoneScore) , GTK_SHADOW_ETCHED_OUT );
	pScoreBox = gtk_vbox_new ( FALSE , 0 );
	gtk_container_add (GTK_CONTAINER(pZoneScore) , pScoreBox );
	char label[40];
	for (i=0;i<=nbJoueurs-1;i++)
	{
		sprintf ( label , "Joueur %d : %d" , i+1, score[i] );
		pScoreJoueurs[i] = gtk_label_new ( label );
		gtk_box_pack_start (GTK_BOX(pScoreBox), pScoreJoueurs[i], FALSE, FALSE, 0);
	}

	gtk_table_attach_defaults (GTK_TABLE(pTable) , pZoneScore , 0 , 2 , 0 , 9 );

    //mise en place de la grille
	pGrille = gtk_table_new ( 15 , 15 , TRUE );
	//gtk_table_set_col_spacings (GTK_TABLE(pGrille) , 0);
	//gtk_table_set_row_spacings (GTK_TABLE(pGrille) , 0);

	pGrilleVirtuelle = malloc(sizeof(Grille));
    genererGrille(pGrilleVirtuelle);

	int l;
	int c;
	for ( l=0 ; l<=14 ; l++ )
	{
		for ( c=0 ; c<=14 ; c++ )
		{
			gtk_imagecliquable_new (&(pQuadrillage[l][c])); // Init label
			gtk_table_attach_defaults (GTK_TABLE(pGrille), pQuadrillage[l][c].pButton  , c , c+1 , l , l+1);
		}
	}
	miseAJourGrille(pQuadrillage, *pGrilleVirtuelle);

	gtk_table_attach_defaults (GTK_TABLE(pTable) , pGrille , 2 ,11 , 0 , 9);
// allocation de la pioche
    pPioche=malloc(sizeof(Pile));
    *pPioche=nouvellePile();
    Jeton ensembleDesJetons[NOMBREDEJETON];
    distributionInitiale(ensembleDesJetons, pPioche, ensembleDesChevaletsVirtuels, nbJoueurs);

//mise en place du chevalet
	pZoneJoueur = gtk_table_new (10 , 2 , FALSE);

	pActionsBox = gtk_vbox_new (FALSE , 0);
	pAnnuler = gtk_button_new_with_label ("ANNULER");
	pDefausser = gtk_button_new_with_label ("DEFAUSSER");
	gtk_box_pack_start (GTK_BOX(pActionsBox) , pDefausser , FALSE , FALSE , 0);
	gtk_box_pack_start (GTK_BOX(pActionsBox) , pAnnuler , FALSE , FALSE , 0 );
	gtk_table_attach_defaults (GTK_TABLE(pZoneJoueur) , pActionsBox , 0 , 2 , 0 , 2);

	pOK = gtk_button_new_with_label ("OK");
	gtk_table_attach_defaults (GTK_TABLE(pZoneJoueur) , pOK , 9 , 10 , 1 , 2);

	pPret = gtk_button_new_with_label ("Pret !");
	gtk_table_attach_defaults (GTK_TABLE(pZoneJoueur) , pPret , 2 , 9 , 1 , 2 );
    noJoueur=1;

    char label2[40];
    sprintf(label2,"Joueur %d\n",noJoueur);
	pJoueur = gtk_label_new (label2);

	gtk_table_attach_defaults (GTK_TABLE(pZoneJoueur) , pJoueur , 2 ,10 , 0 , 1);

	gtk_table_attach_defaults (GTK_TABLE(pTable) , pZoneJoueur , 0 , 11 , 9 , 10 );

//affichage de la fenetre
	signalFermerFenetre2=g_signal_connect (G_OBJECT(pWindow), "destroy", G_CALLBACK(onDestroy2), NULL);
	g_signal_connect (G_OBJECT(pPret), "clicked", G_CALLBACK(afficherChevaletJoueur), NULL);

	for(l=0;l<=14;l++)
	{
		for (c=0;c<=14;c++)
		{
			g_signal_connect(G_OBJECT(pQuadrillage[l][c].pButton),"clicked",G_CALLBACK(poserLettreChoisie), NULL);
			gtk_widget_set_sensitive(pQuadrillage[l][c].pButton,FALSE);
		}
	}
	signalOK=g_signal_connect(G_OBJECT(pOK),"clicked",G_CALLBACK(validerPose),NULL);
	gtk_widget_set_sensitive(pOK,FALSE);
	g_signal_connect(G_OBJECT(pAnnuler),"clicked",G_CALLBACK(annulerMot),NULL);
	gtk_widget_set_sensitive(pAnnuler,FALSE);
	g_signal_connect(G_OBJECT(pDefausser),"clicked",G_CALLBACK(defausser),NULL);
	gtk_widget_set_sensitive(pDefausser,FALSE);

	gtk_widget_show_all(pWindow);
	gtk_main();

}


/** On sauvegarde la grille et le chevalet au cas ou le joueur voudrait annuler son depot de lettre (ou si son mot n'existe pas etc) */
void sauvegardeGrilleChevalets(void){
    int l,c;
	for (l=0 ; l<7 ; l++)
        ancienEnsembleDesChevaletsVirtuels[noJoueur-1][l] = ensembleDesChevaletsVirtuels[noJoueur-1][l];

    for (l=0 ; l<15 ; l++)
        for (c=0 ; c<15 ; c++)
            ancienneGrilleVirtuelle[l][c] = (*pGrilleVirtuelle)[l][c] ;
}


/** Le joueur est pret, on cree et affiche son chevalet : on attend qu'il choisisse un lettre a poser ou qu'il choisisse de se defausser */
void afficherChevaletJoueur ( GtkWidget *pWidget, gpointer pData )
{
	gtk_widget_destroy(pPret);

	int l,c;
	for ( l=0 ; l<7 ; l++ )
    {
        gtk_imagecliquable_new (&(pChevalet[l]));
        gtk_table_attach_defaults (GTK_TABLE(pZoneJoueur) , pChevalet[l].pButton , 2+l , 3+l , 1 , 2);
        signauxChevalet[l]=g_signal_connect (G_OBJECT(pChevalet[l].pButton), "clicked",  G_CALLBACK(selectionnerLettreChoisie), NULL);
        if (ensembleDesChevaletsVirtuels[noJoueur-1][l].lettre != '/')
            gtk_widget_set_sensitive(pChevalet[l].pButton,TRUE);
        else
            gtk_widget_set_sensitive(pChevalet[l].pButton,FALSE);
    }
    for (l=0;l<15;l++)
        for(c=0;c<15;c++)
            masqueGrille[l][c]=0;

	gtk_widget_set_sensitive(pDefausser,TRUE);
    gtk_widget_show_all(pWindow);
    sauvegardeGrilleChevalets();
	miseAJourChevalet(pChevalet,ensembleDesChevaletsVirtuels [noJoueur-1]);


	if(*pPioche==NULL){
        gtk_button_set_label ( GTK_BUTTON (pDefausser) , "PASSER (Pioche vide)");
        g_signal_connect (G_OBJECT(pDefausser), "clicked",  G_CALLBACK(passerTour), NULL);
	}
}


/** Le joueur a clique sur la lettre qu'il veut poser : on attend qu'il choisisse ou la poser (ou qu'il annule) */
void selectionnerLettreChoisie ( GtkWidget *pLettreChoisie, gpointer pData )
{
	int l;
	int c;
	int indiceLettreSelectionee;
	for(l=0;l<=6;l++)
	{
		if(pLettreChoisie==pChevalet[l].pButton)
		{
			indiceLettreSelectionee=l;
		}
	}
	if (ensembleDesChevaletsVirtuels[noJoueur-1][indiceLettreSelectionee].lettre != '/')
	{
		for(l=0;l<=6;l++)
		{


			if (pChevalet[l].pButton!=pLettreChoisie)
			{
				gtk_widget_set_sensitive (pChevalet[l].pButton, FALSE);

			}
			else
			{
			indiceLettreChoisie = l;
			}
		}
		gtk_widget_show_all(pWindow);
		g_signal_handler_disconnect (G_OBJECT(pChevalet[indiceLettreChoisie].pButton), signauxChevalet[indiceLettreChoisie]);
		signauxChevalet[indiceLettreChoisie]=g_signal_connect(G_OBJECT(pLettreChoisie), "clicked" , G_CALLBACK(deselectionnerLettreChoisie), NULL);
		for(l=0;l<=14;l++)
		{
			for (c=0;c<=14;c++)
			{
				if((*pGrilleVirtuelle)[l][c].jeton.lettre == '/')
				{
					gtk_widget_set_sensitive(pQuadrillage[l][c].pButton,TRUE);

				}
			}
		}
		gtk_widget_set_sensitive (pDefausser, FALSE);
		gtk_widget_set_sensitive (pAnnuler, FALSE);
		gtk_widget_set_sensitive (pOK, FALSE);
	}
}

/** Le joueur ne veut plus poser cette lettre : on attend qu'il en choisisse une autre */
void deselectionnerLettreChoisie ( GtkWidget *pWidget, gpointer pData )
{
	int l;
	int c;


	for (l=0;l<=6;l++)
	{
		gtk_widget_set_sensitive(pChevalet[l].pButton,TRUE);

		g_signal_handler_disconnect (G_OBJECT(pChevalet[l].pButton), signauxChevalet[l]);
		signauxChevalet[l]=g_signal_connect (G_OBJECT(pChevalet[l].pButton), "clicked",  G_CALLBACK(selectionnerLettreChoisie), NULL);
	}
	for(l=0;l<=14;l++)
	{
		for (c=0;c<=14;c++)
		{
			gtk_widget_set_sensitive(pQuadrillage[l][c].pButton,FALSE);

		}
	}
	if (nbLettresPosees==0)
	{
		gtk_widget_set_sensitive (pDefausser, TRUE);
	}
	else
	{
		gtk_widget_set_sensitive (pAnnuler, TRUE);
		gtk_widget_set_sensitive (pOK, TRUE);
	}

}


/** Le joueur a choisi la case sur laquelle poser la lettre desiree : on attend qu'il en choisisse une autre ou qu'il valide son mot (ou qu'il annule) */
void poserLettreChoisie ( GtkWidget *pWidget, gpointer pData )
{
	nbLettresPosees++;
	int l;
	int c;
	int lChoisi=0;
	int cChoisi=0;

	for (l=0;l<=6;l++)
	{
	  if(ensembleDesChevaletsVirtuels[noJoueur-1][l].lettre != '/' )
		gtk_widget_set_sensitive(pChevalet[l].pButton,TRUE);
	  else
		gtk_widget_set_sensitive(pChevalet[l].pButton,FALSE);
	}

	for(l=0;l<=14;l++)
	{
		for (c=0;c<=14;c++)
		{
		  if (pWidget == pQuadrillage[l][c].pButton)
		    {
		      lChoisi=l;
		      cChoisi=c;
		    }
		  gtk_widget_set_sensitive(pQuadrillage[l][c].pButton,FALSE);


		}
	}

	masqueGrille[lChoisi][cChoisi]=1;

	if (nbLettresPosees==0)
	{
		gtk_widget_set_sensitive (pDefausser, TRUE);
	}
	else
	{
		gtk_widget_set_sensitive (pAnnuler, TRUE);
		gtk_widget_set_sensitive (pOK, TRUE);
	}

    (*pGrilleVirtuelle)[lChoisi][cChoisi].jeton = ensembleDesChevaletsVirtuels[noJoueur-1][indiceLettreChoisie] ;

    ensembleDesChevaletsVirtuels[noJoueur-1][indiceLettreChoisie].lettre = '/' ;
    ensembleDesChevaletsVirtuels[noJoueur-1][indiceLettreChoisie].valeur = 0 ;


    gtk_widget_set_sensitive(pChevalet[indiceLettreChoisie].pButton,FALSE);


    char label[50];
    if ((*pGrilleVirtuelle)[lChoisi][cChoisi].jeton.lettre=='?')
        sprintf(label,"ressources/jetons/jocker.png");
    else
        sprintf(label,"ressources/jetons/%c.png",(*pGrilleVirtuelle)[lChoisi][cChoisi].jeton.lettre);

    gtk_imagecliquable_set_image_from_file(&(pQuadrillage[lChoisi][cChoisi]),label);

    gtk_imagecliquable_set_image_from_file(&(pChevalet[indiceLettreChoisie]),NULL);
}


/** En cours de depose, le joueur ne veut plus poser son mot : on retablit la grille et le chevalet aux valeurs sauvegardees */
void annulerMot ( GtkWidget *pWidget, gpointer pData )
{
	nbLettresPosees=0;
	int l,c;

	for (l=0;l<7;l++)
	{
		gtk_widget_set_sensitive(pChevalet[l].pButton,TRUE);

		g_signal_handler_disconnect (G_OBJECT(pChevalet[l].pButton), signauxChevalet[l]);
		signauxChevalet[l]=g_signal_connect (G_OBJECT(pChevalet[l].pButton), "clicked",  G_CALLBACK(selectionnerLettreChoisie), NULL);
		ensembleDesChevaletsVirtuels[noJoueur-1][l] = ancienEnsembleDesChevaletsVirtuels[noJoueur-1][l] ; 	// On rétablit aux valeurs sauvegardée


	}
	for(l=0;l<15;l++)
	{
		for (c=0;c<15;c++)
		{
            masqueGrille[l][c]=0;
			gtk_widget_set_sensitive(pQuadrillage[l][c].pButton,FALSE);

			(*pGrilleVirtuelle)[l][c] = ancienneGrilleVirtuelle[l][c] ;	// On rétablit aux valeurs sauvegardée

		}
	}


    miseAJourChevalet(pChevalet,ensembleDesChevaletsVirtuels[noJoueur-1]);
    miseAJourGrille(pQuadrillage, *pGrilleVirtuelle);

    gtk_widget_set_sensitive (pDefausser, TRUE);
    gtk_widget_set_sensitive (pAnnuler, FALSE);
    gtk_widget_set_sensitive (pOK, FALSE);
}


/** Le joueur valide son mot : on verifie que tout est bon sinon on restaure au chevalet et a la grille sauvegardee */
void validerPose ( GtkWidget *pWidget, gpointer pData )
{
    int i,j;
    char *pSens;
    int *pC0;
    int *pL0;
    int *pN;
    int *pScoreTour;
    Booleen *pTourValide ;
    int *pTouche;

    pSens=malloc(sizeof(char));
    pC0=malloc(sizeof(int));
    pL0=malloc(sizeof(int));
    pN=malloc(sizeof(int));
    pTourValide = malloc (sizeof (Booleen) );
    pScoreTour = malloc (sizeof (int) );
    pTouche=malloc(sizeof(int));

    *pSens='/';
    *pC0=-1;
    *pL0=-1;
    *pN=0;
    *pTourValide= VRAI;
    *pScoreTour=0;
    *pTouche=0;

    int testMasque = verifMasqueGrille(*pGrilleVirtuelle, pSens, pL0, pC0, pN,masqueGrille,pTouche,message);

    if ( !testMasque ){
        char label[220];
        sprintf(label,"Joueur %d\n%s",noJoueur,message);
        gtk_label_set_text(GTK_LABEL(pJoueur),label);

        annulerMot(pWidget,NULL);
    }
    else {
        parcoursDesMotsCrees (*pGrilleVirtuelle, *pSens, *pL0, *pC0, *pN, pTourValide, pScoreTour,masqueGrille, pTouche,message) ;
        if (*pTourValide==FAUX){
            char label[220];
            sprintf(label,"Joueur %d\n%s",noJoueur,message);
            gtk_label_set_text(GTK_LABEL(pJoueur),label);

            annulerMot(pWidget,NULL);
        }
        else {
            for (i=0;i<15;i++)
                for (j=0;j<15;j++)
                    if (masqueGrille[i][j]==1) {
                        (*pGrilleVirtuelle)[i][j].multiLettre = 1;
                        (*pGrilleVirtuelle)[i][j].multiMot = 1;
                    }
            score[noJoueur-1] += *pScoreTour;
	    if (testMasque==2)
	      score[noJoueur-1]+=50;
            char label[40];
            sprintf ( label , "Joueur %d : %d" , noJoueur, score[noJoueur-1] );
            gtk_label_set_text(GTK_LABEL(pScoreJoueurs[noJoueur-1]),label);

            distributionJetons (pPioche,ensembleDesChevaletsVirtuels[noJoueur-1]); // on distribue au joueur ses nouveaux jetons

            if (*pPioche==NULL) {
                int n;
                n=0;
                for(i=0;i<7;i++) {
                    if (ensembleDesChevaletsVirtuels[noJoueur-1][i].lettre != '/') {
                        n++;
                    }
                }
                if (n==0){
                    finDePartie();
                    return;
                }
            }

            // Joueur suivant
            noJoueur++;
            if (noJoueur>nbJoueurs)
            {
                noJoueur=1;
            }
            nbLettresPosees=0;
            int l;

            for (l=0;l<7;l++)
            {
                gtk_widget_destroy(pChevalet[l].pButton);
            }

            gtk_widget_set_sensitive(pOK, FALSE);
            gtk_widget_set_sensitive(pAnnuler, FALSE);
            gtk_widget_set_sensitive(pDefausser, FALSE);

            pPret = gtk_button_new_with_label ("Pret !");
            gtk_table_attach_defaults (GTK_TABLE(pZoneJoueur) , pPret , 2 , 9 , 1 , 2 );
            g_signal_connect (G_OBJECT(pPret), "clicked", G_CALLBACK(afficherChevaletJoueur), NULL);

            char s[40];
            sprintf (s, "Joueur %d\n" , noJoueur);
            gtk_label_set_text(GTK_LABEL(pJoueur), (gchar*)s);
            gtk_widget_show_all(pWindow);

        }
    }

    free(pSens);
    free(pC0);
    free(pL0);
    free(pN);
    free(pTourValide);
    free(pScoreTour);
    free(pTouche);


}


/** Le joueur souhaite se defausser d'un certain nombre de ses jetons */
void defausser ( GtkWidget *pWidget, gpointer pData )
{
    nbLettresPosees=0;
    gtk_widget_set_sensitive (pDefausser, FALSE);
    gtk_widget_set_sensitive (pAnnuler, TRUE);
	g_signal_connect(G_OBJECT(pAnnuler), "clicked" , G_CALLBACK(deselectionnerLettreChoisie), NULL);
    g_signal_handler_disconnect(G_OBJECT(pOK),signalOK);
	signalOK=g_signal_connect(G_OBJECT(pOK),"clicked",G_CALLBACK(validerDefausse),NULL);
    int l;
	for ( l=0 ; l<7 ; l++ )
    {
        masqueDefausse[l]=0;
        g_signal_handler_disconnect (G_OBJECT(pChevalet[l].pButton), signauxChevalet[l]);
        signauxChevalet[l]=g_signal_connect (G_OBJECT(pChevalet[l].pButton), "clicked",  G_CALLBACK(defausserLettreChoisie), GINT_TO_POINTER(l));
    }
}


/** Le joueur a choisit une lettre a defausser */
void defausserLettreChoisie ( GtkWidget *pWidget, gpointer pData )
{
    char label[40];
    int l=GPOINTER_TO_INT(pData);
    nbLettresPosees++;

    g_signal_handler_disconnect (G_OBJECT(pChevalet[l].pButton), signauxChevalet[l]);
    signauxChevalet[l]=g_signal_connect (G_OBJECT(pChevalet[l].pButton), "clicked",  G_CALLBACK(annulerDefausserLettreChoisie), pData);

    if (ensembleDesChevaletsVirtuels[noJoueur-1][l].lettre == '?')
        sprintf(label,"ressources/jetonsD/(jocker).png");
    else
        sprintf(label,"ressources/jetonsD/(%c).png", ensembleDesChevaletsVirtuels[noJoueur-1][l].lettre);

    gtk_imagecliquable_set_image_from_file(&(pChevalet[l]),label);

    masqueDefausse[l]=1;

    if (nbLettresPosees != 0) gtk_widget_set_sensitive(pOK, TRUE);
}


/** Le joueur ne veut plus defausser cette lettre */
void annulerDefausserLettreChoisie ( GtkWidget *pWidget, gpointer pData )
{
    char label[40];
    int l=GPOINTER_TO_INT(pData);
    nbLettresPosees--;

    g_signal_handler_disconnect (G_OBJECT(pChevalet[l].pButton), signauxChevalet[l]);
    signauxChevalet[l]=g_signal_connect (G_OBJECT(pChevalet[l].pButton), "clicked",  G_CALLBACK(defausserLettreChoisie), pData);


    if (ensembleDesChevaletsVirtuels[noJoueur-1][l].lettre == '?')
        sprintf(label,"ressources/jetons/jocker.png");
    else
        sprintf(label,"ressources/jetons/%c.png", ensembleDesChevaletsVirtuels[noJoueur-1][l].lettre);

    gtk_imagecliquable_set_image_from_file(&(pChevalet[l]),label);

    masqueDefausse[l]=0;

    if (nbLettresPosees == 0) gtk_widget_set_sensitive(pOK, FALSE);

}


/** Le joueur valide son choix de lettres a defausser, on lui en distribue d'autres et on passe au joueur suivant */
void validerDefausse ( GtkWidget *pWidget, gpointer pData )
{

    defausseParIndice(ensembleDesChevaletsVirtuels[noJoueur-1],pPioche,masqueDefausse);

	noJoueur++;
	if (noJoueur>nbJoueurs)
	{
		noJoueur=1;
	}
	nbLettresPosees=0;
	int l;

	for (l=0;l<7;l++)
	{
		gtk_widget_destroy(pChevalet[l].pButton);
		if (signauxChevalet[l]==1)
        masqueDefausse[l]=0;
	}

    gtk_widget_set_sensitive(pOK, FALSE);
    gtk_widget_set_sensitive(pAnnuler, FALSE);
    gtk_widget_set_sensitive(pDefausser, FALSE);

	pPret = gtk_button_new_with_label ("Pret !");
	gtk_table_attach_defaults (GTK_TABLE(pZoneJoueur) , pPret , 2 , 9 , 1 , 2 );
	g_signal_connect (G_OBJECT(pPret), "clicked", G_CALLBACK(afficherChevaletJoueur), NULL);

    g_signal_handler_disconnect(G_OBJECT(pOK),signalOK);
	signalOK=g_signal_connect(G_OBJECT(pOK),"clicked",G_CALLBACK(validerPose),NULL);

	char s[40];
	sprintf (s, "Joueur %d\n" , noJoueur);
	gtk_label_set_text(GTK_LABEL(pJoueur), (gchar*)s);
	gtk_widget_show_all(pWindow);
}


/** Le joueur ne peut rien faire, il passe son tour, on passe au joueur suivant */
void passerTour ( GtkWidget *pWidget, gpointer pData )
{
	int l,n;
	if (*pPioche==NULL) {
        passeurs[noJoueur-1] = 1;

        n=0;
        for (l=0;l<nbJoueurs;l++){
            if (passeurs[l]==1)
                n++;
        }
        if (n==nbJoueurs) {
            finDePartie();
            return;
        }
	}

	noJoueur++;
	if (noJoueur>nbJoueurs)
	{
		noJoueur=1;
	}

	if (*pPioche==NULL)
        passeurs[noJoueur-1] = 0;

	nbLettresPosees=0;


	for (l=0;l<7;l++)
	{
		gtk_widget_destroy(pChevalet[l].pButton);
		if (signauxChevalet[l]==1)
        masqueDefausse[l]=0;
	}

    gtk_widget_set_sensitive(pOK, FALSE);
    gtk_widget_set_sensitive(pAnnuler, FALSE);
    gtk_widget_set_sensitive(pDefausser, FALSE);

	pPret = gtk_button_new_with_label ("Pret !");
	gtk_table_attach_defaults (GTK_TABLE(pZoneJoueur) , pPret , 2 , 9 , 1 , 2 );
	g_signal_connect (G_OBJECT(pPret), "clicked", G_CALLBACK(afficherChevaletJoueur), NULL);

    g_signal_handler_disconnect(G_OBJECT(pOK),signalOK);
	signalOK=g_signal_connect(G_OBJECT(pOK),"clicked",G_CALLBACK(validerPose),NULL);

	char s[40];
	sprintf (s, "Joueur %d" , noJoueur);
	gtk_label_set_text(GTK_LABEL(pJoueur), (gchar*)s);
	gtk_widget_show_all(pWindow);
}


/** Le fin de la partie a ete declenchee : on affiche dans une nouvelle fenetre le score ainsi que le numero du gagnant */
void finDePartie () {

	GtkWidget *pTableFinDePartie;
	GtkWidget *pZoneScore2;
    GtkWidget *pScoreBox2;
    GtkWidget *pScoreJoueurs2[4];
    GtkWidget *PFermer;
    GtkWidget *PNouvellePartie;
    GtkWidget *pFinDeLaPartie;
    GtkWidget *pJoueurGagnant;

//initialisation de la fenetre (pWindow)
	pWindow2 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size (GTK_WINDOW(pWindow2), 400, 200);
	gtk_window_set_title (GTK_WINDOW(pWindow2), "Scrabble");
	gtk_window_set_modal(GTK_WINDOW(pWindow2), TRUE);


	//Mise en place de la table
	pTableFinDePartie = gtk_table_new (4 , 4 , TRUE );
	gtk_container_add (GTK_CONTAINER(pWindow2) , GTK_WIDGET(pTableFinDePartie));

    pFinDeLaPartie = gtk_label_new ( "Fin de la partie" );
    gtk_table_attach_defaults (GTK_TABLE(pTableFinDePartie) , pFinDeLaPartie,0,4 , 0,1 );

	pZoneScore2 = gtk_frame_new ("Score:");
	gtk_frame_set_shadow_type (GTK_FRAME(pZoneScore2) , GTK_SHADOW_ETCHED_OUT );
	pScoreBox2 = gtk_vbox_new ( FALSE , 0 );
	gtk_container_add (GTK_CONTAINER(pZoneScore2) , pScoreBox2 );



	char label[50];
	int i,j;
	for (i=0;i<=nbJoueurs-1;i++)
	{
	    for (j=0;j<7;j++)
            score[i]-=ensembleDesChevaletsVirtuels[i][j].valeur; // On enlève la valeur de chaque jeton (un '/' valant 0 de toute façon)

		sprintf ( label , "Joueur %d : %d" , i+1, score[i] );
		pScoreJoueurs2[i] = gtk_label_new ( label );
		gtk_box_pack_start (GTK_BOX(pScoreBox2), pScoreJoueurs2[i], FALSE, FALSE, 0);
	}
	gtk_table_attach_defaults (GTK_TABLE(pTableFinDePartie) , pZoneScore2,0,4 , 1,2);

	int gagnant = 1;
	for (i=1;i<nbJoueurs;i++)
        if (score[gagnant-1]<score[i])
            gagnant=i+1;

    sprintf(label,"Le joueur %d gagne la partie !!!", gagnant);
    pJoueurGagnant = gtk_label_new ( label);
    gtk_table_attach_defaults (GTK_TABLE(pTableFinDePartie) , pJoueurGagnant,0,4 , 2,3 );

//Mise en place des boutons

	//PNbPlayer[0]=gtk_button_new_with_label ("1 Joueur");
	PNouvellePartie=gtk_button_new_with_label ("Rejouer !");
	PFermer=gtk_button_new_with_label ("Quitter");

	gtk_table_attach_defaults (GTK_TABLE(pTableFinDePartie), PNouvellePartie , 1 , 2  , 3 , 4);
	gtk_table_attach_defaults (GTK_TABLE(pTableFinDePartie), PFermer, 2 , 3  , 3 , 4 );


//affichage de la fenetre

	g_signal_connect(G_OBJECT(pWindow2), "destroy", G_CALLBACK(toutFermer), NULL);
	g_signal_connect(G_OBJECT(PNouvellePartie), "clicked" , G_CALLBACK(rejouer), NULL);
	g_signal_connect(G_OBJECT(PFermer), "clicked" , G_CALLBACK(toutFermer), NULL);
	gtk_widget_show_all(pWindow2);
}

/**initialisation d'une image cliquable*/
void gtk_imagecliquable_new(ImageCliquable *pImageCliquable)
{
	pImageCliquable->pButton = gtk_button_new();
	pImageCliquable->pImage = gtk_image_new();
	gtk_container_add(GTK_CONTAINER(pImageCliquable->pButton), pImageCliquable->pImage);
}

/**charger une image dans l'imagecliquable*/
void gtk_imagecliquable_set_image_from_file(ImageCliquable *pImageCliquable, char* file)
{
	gtk_image_set_from_file(GTK_IMAGE(pImageCliquable->pImage),file);
}
