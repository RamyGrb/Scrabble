#define NOMBREDEJETON 102
#define COTEGRILLE 15

typedef enum{FAUX,VRAI} Booleen;

typedef struct Jeton  {  char lettre; //lettre ('A' à 'Z') ou Jocker ('?') ou rien ('/')
                        int valeur; //valeur de la lettre (0 si pas de lettre, 1 à 10 si lettre)
                    } Jeton;

typedef struct	{	struct Jeton jeton; // Jeton éventuellement présent
					int multiMot; // multiplicateur de mot (1 à 3)
					int multiLettre; // multiplicateur de lettre (1 à 3)
				} Case;

typedef struct element {    Jeton jeton;
                            struct element* lien; } Element;

typedef Element *Pile;

typedef Jeton Chevalet[7];

typedef Case Grille[COTEGRILLE][COTEGRILLE];



