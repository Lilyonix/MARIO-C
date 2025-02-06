#ifndef AFFICHERANIMATION_H
#define AFFICHERANIMATION_H

#include "main.h"

extern int frameActuellePiece;

// Déclarations des fonctions
void mettreAJourAnimation(Animation* anim);
Animation creerAnimation(SDL_Texture* texture, int largeur, int hauteur, int nbFrames, int vitesse);

#endif //AFFICHERANIMATION_H
