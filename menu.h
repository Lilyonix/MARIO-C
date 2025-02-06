#ifndef MENU_H
#define MENU_H

#include "jouer.h"
#include "niveau.h"
#include <dirent.h>
#include "texture.h"

char* afficherMenu(SDL_Renderer* renderer, TTF_Font* font, EtatJeu* etatActuel);
char* selectionnerNiveau(SDL_Renderer* renderer, TTF_Font* font);
int listerNiveaux(Niveau niveaux[], int maxNiveaux);
int selectionnerPersonnage(SDL_Renderer* renderer, TTF_Font* font);
void menuPrincipal(SDL_Renderer* renderer, EtatJeu* etatActuel);
void afficherGameOverScreen(SDL_Renderer* renderer);

#endif //MENU_H
