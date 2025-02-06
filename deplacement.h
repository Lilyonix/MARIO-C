#ifndef DEPLACEMENT_H
#define DEPLACEMENT_H


#include "main.h"


void gererCollisions(Joueur* joueur, char** carte, int largeur, int hauteur);

void gererCollisionsAvecEnnemis(Joueur* joueur);

void gererEvenementJoueur2(Joueur* joueur2, SDL_Event* e, int* enCours);

void gererEvenements(Joueur* joueur, int* enCours);

void gererCollisions2(Joueur* joueur,Joueur* joueurinitial, char** carte, int largeur, int hauteur);

void gererEvenementJoueur1(Joueur* joueur2, SDL_Event* e, int* enCours);



#endif //DEPLACEMENT_H
