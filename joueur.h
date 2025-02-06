#ifndef JOUEUR_H
#define JOUEUR_H
#include "main.h"
#include "animation.h"

typedef struct {
    int x, y;          // Position du joueur (en pixels)
    int vx, vy;        // Vitesse du joueur
    int estSautant;    // Indique si le joueur est en saut
    int vies;          // Nombre de vies restantes
    int score;         // Score du joueur
    int checkpointX;   // Coordonnée X du dernier checkpoint
    int checkpointY;   // Coordonnée Y du dernier checkpoint
    Animation animMarche;  // Animation de marche
    Animation animSaut;    // Animation de saut
    int direction;
    int gagner;
} Joueur;


#endif //JOUEUR_H
