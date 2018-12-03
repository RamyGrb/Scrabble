#include <unistd.h>  //pour getpid
#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include "../ressources/structureDeDonnee.h"
#include "../headers/grille.h"
#include "../headers/distribution.h"
#include "../headers/defausse.h"
#include "../headers/pile.h"
#include "../headers/parcoursMotsCrees.h"
#include "../headers/verifMasqueGrille.h"

typedef struct { GtkWidget *pButton;
		 GtkWidget *pImage; } ImageCliquable;
		 
void choisirNbJoueurs ();
void onDestroy(GtkWidget *pWidget, gpointer pData);
void toutFermer (GtkWidget *pWidget, gpointer pData);
void rejouer (GtkWidget *pWidget, gpointer pData);
void onDestroy2(GtkWidget *pWidget, gpointer pData);
void nouvellePartie (GtkWidget *pWidget, gpointer pNbJoueurs);
void sauvegardeGrilleChevalets(void);
void afficherChevaletJoueur ( GtkWidget *pWidget, gpointer pData );
void selectionnerLettreChoisie ( GtkWidget *pLettreChoisie, gpointer pData );
void deselectionnerLettreChoisie ( GtkWidget *pWidget, gpointer pData );
void poserLettreChoisie ( GtkWidget *pWidget, gpointer pData );
void annulerMot ( GtkWidget *pWidget, gpointer pData );
void defausser ( GtkWidget *pWidget, gpointer pData );
void defausserLettreChoisie ( GtkWidget *pWidget, gpointer pData );
void annulerDefausserLettreChoisie ( GtkWidget *pWidget, gpointer pData );
void validerPose ( GtkWidget *pWidget, gpointer pData );
void validerDefausse ( GtkWidget *pWidget, gpointer pData );
void passerTour ( GtkWidget *pWidget, gpointer pData );
void finDePartie ();
void miseAJourChevalet(ImageCliquable pChevalet[7], Chevalet chevaletVirtuel);
void miseAJourGrille(ImageCliquable pQuadrillage[15][15], Grille grille);
void gtk_imagecliquable_new(ImageCliquable *pImageCliquable);
void gtk_imagecliquable_set_image_from_file(ImageCliquable *pImageCliquable, char* file);