 #ifndef JOUER_H
#define JOUER_H

#include "ennemie.h"
#include "main.h"


void jouer(SDL_Renderer* renderer, SDL_Texture* texturefond, Joueur* joueur, char** carte, int largeur, int hauteur, EtatJeu* etatActuel);
void jouer2(SDL_Renderer* renderer, SDL_Texture* texturefond, Joueur* joueur, Joueur* joueur2, char** carte, int largeur, int hauteur, EtatJeu* etatActuel);


#endif //JOUER_H
