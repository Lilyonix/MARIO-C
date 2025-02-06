#ifndef AFFICHAGE_H
#define AFFICHAGE_H

#include "main.h"


char** chargerCarte(const char* fichier, int* largeur, int* hauteur, Joueur* joueur);

void afficherGameOverMenu(SDL_Renderer *renderer);
void libererCarte(char** carte, int hauteur);

void afficherCarte(SDL_Renderer* renderer, char** carte, int largeur, int hauteur, int offsetX, int offsetY);

void afficherJoueur(SDL_Renderer* renderer, Joueur* joueur, int offsetX, int offsetY);

void afficherInterface(SDL_Renderer* renderer, SDL_Texture* textureVie, SDL_Texture* textureAnneau, TTF_Font* font, Joueur* joueur);

void afficherEnnemis(SDL_Renderer* renderer, int offsetX, int offsetY);

void afficherCadrillage(SDL_Renderer* renderer, int largeur, int hauteur);

void afficherTexte(SDL_Renderer* renderer, TTF_Font* font, const char* texte, int x, int y, SDL_Color couleur);

#endif //AFFICHAGE_H
