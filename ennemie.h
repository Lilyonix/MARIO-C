#ifndef ENNEMIE_H
#define ENNEMIE_H
#include "main.h"

typedef enum {
    NORMAL,    // Ennemi classique
    PIRANHA,   // Plante piranha (On meurt si on la touche pas le haut)
    CARAPACE,   // Carapace (mortelle par le haut aussi)
    MASCAS,
    CARAPACEVITE,
    BOSS,
} TypeEnnemi;


typedef struct {
    int x, y;         // Position de l'ennemi
    int vx;           // Vitesse horizontale de l'ennemi
    int limiteGauche; // Limite gauche du mouvement
    int limiteDroite; // Limite droite du mouvement
    TypeEnnemi type;  // Type de l'ennemi (NORMAL, PIRANHA, CARAPACE)
    Animation anim;
    int direction; // Animation de l'ennemi
} Ennemi;

extern int Direction;
extern Ennemi ennemis[];
extern int nbEnnemis;


void deplacerEnnemis();


#endif //ENNEMIE_H
