do : scrabble clean
all : test scrabble clean
####################
# Tout tester
test : distribution estDansDico grille defausse clean
####################	



####################
#Programme Principal
scrabble : interface.o verifMasqueGrille.o parcoursMotsCrees.o estDansDico.o grille.o distribution.o pile.o defausse.o
	gcc -Wall -g interface.o verifMasqueGrille.o parcoursMotsCrees.o estDansDico.o grille.o distribution.o pile.o defausse.o `pkg-config --libs --cflags gtk+-2.0` -o scrabble
	
interface.o : headers/interface.h sources/interface.c headers/pile.h  headers/defausse.h headers/grille.h headers/distribution.h headers/verifMasqueGrille.h headers/parcoursMotsCrees.h ressources/structureDeDonnee.h
	gcc -Wall -g `pkg-config --cflags --libs gtk+-2.0` -c sources/interface.c
	
parcoursMotsCrees.o : sources/parcoursMotsCrees.c headers/parcoursMotsCrees.h ressources/structureDeDonnee.h headers/estDansDico.h
	gcc -Wall -g -c `pkg-config --cflags --libs gtk+-2.0` sources/parcoursMotsCrees.c

verifMasqueGrille.o: sources/verifMasqueGrille.c ressources/structureDeDonnee.h
	gcc -Wall -g -c `pkg-config --cflags --libs gtk+-2.0` sources/verifMasqueGrille.c
####################



####################
# Test de défausse
defausse : testDefausse.o distribution.o defausse.o pile.o
	gcc -Wall -g testDefausse.o distribution.o defausse.o pile.o -o testDefausse

testDefausse.o : sources/testDefausse.c  headers/distribution.h headers/defausse.h headers/defausse.h ressources/structureDeDonnee.h
	gcc -Wall -g -c sources/testDefausse.c
	
defausse.o : sources/defausse.c headers/defausse.h headers/pile.h headers/distribution.h ressources/structureDeDonnee.h
	gcc -Wall -g -c `pkg-config --cflags --libs gtk+-2.0` sources/defausse.c
####################

####################
# Test de distribution
distribution : testDistribution.o distribution.o pile.o
	gcc -Wall -g testDistribution.o distribution.o pile.o -o testDistribution

testDistribution.o : sources/testDistribution.c  headers/distribution.h headers/defausse.h ressources/structureDeDonnee.h
	gcc -Wall -g -c sources/testDistribution.c

pile.o : sources/pile.c headers/pile.h ressources/structureDeDonnee.h
	gcc -Wall -g -c `pkg-config --cflags --libs gtk+-2.0` sources/pile.c
	
distribution.o : sources/distribution.c headers/distribution.h headers/pile.h ressources/structureDeDonnee.h
	gcc -Wall -g -c `pkg-config --cflags --libs gtk+-2.0` sources/distribution.c
####################	

####################
# Test de estDansDico
estDansDico : testEstDansDico.o  estDansDico.o
	gcc -Wall -g testEstDansDico.o estDansDico.o -o testEstDansDico

testEstDansDico.o : sources/testEstDansDico.c headers/estDansDico.h ressources/structureDeDonnee.h
	gcc -Wall -g -c sources/testEstDansDico.c
	
estDansDico.o : sources/estDansDico.c headers/estDansDico.h ressources/structureDeDonnee.h
	gcc -Wall -g -c `pkg-config --cflags --libs gtk+-2.0` sources/estDansDico.c
####################

####################
# Test de la génération de la grille
grille : testGrille.o grille.o
	gcc -Wall -g testGrille.o grille.o -o testGrille

testGrille.o : sources/testGrille.c headers/grille.h ressources/structureDeDonnee.h
	gcc -Wall -g -c sources/testGrille.c

grille.o : sources/grille.c headers/grille.h ressources/structureDeDonnee.h
	gcc -Wall -g -c `pkg-config --cflags --libs gtk+-2.0` sources/grille.c

####################
# Nettoyage
clean :
	rm *.o
####################	
