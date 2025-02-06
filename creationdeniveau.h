#ifndef CREATIONDENIVEAU_H
#define CREATIONDENIVEAU_H

#include "main.h"

int demanderLongueurNiveau();

char** creerCarteVide(int largeur);

void gererSouris(char** carte, int largeur, int hauteur, SDL_Event* e, char elementSelectionne);

int afficherSelection(SDL_Renderer* renderer, SDL_Event* event, int* selectionIndex);

void sauvegarderCarte(char** carte, int largeur, int hauteur);


void sauvegarderNiveau(const char* nom, char** carte, int largeur, int hauteur);

void editeurDeNiveau(SDL_Renderer* renderer, TTF_Font* font, EtatJeu *etatActuel);

#endif //CREATIONDENIVEAU_H
