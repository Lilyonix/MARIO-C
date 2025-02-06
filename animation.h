#ifndef ANIMATION_H
#define ANIMATION_H

#include <SDL2/SDL.h>


typedef struct {
    SDL_Texture* texture; // Sprite sheet associée
    int largeurFrame;     // Largeur d'une frame
    int hauteurFrame;     // Hauteur d'une frame
    int nbFrames;         // Nombre total de frames
    int frameActuelle;    // Frame actuellement affichée
    int vitesse;          // Vitesse d'animation (nombre d'updates avant de changer de frame)
    int compteur;         // Compteur interne pour gérer la vitesse
} Animation;


#endif
