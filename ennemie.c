#include "ennemie.h"

int Direction = 1;  // Définition de Direction
Ennemi ennemis[MAX_ENNEMIS];  // Définition du tableau ennemis
int nbEnnemis = 0;  // Définition de nbEnnemis

void deplacerEnnemis() {
    for (int i = 0; i < nbEnnemis; i++) {
        ennemis[i].x += ennemis[i].vx;

        if (ennemis[i].x <= ennemis[i].limiteGauche || ennemis[i].x >= ennemis[i].limiteDroite) {
            // Inverser la direction
            ennemis[i].direction *= -1;
            ennemis[i].vx = -ennemis[i].vx;
        }
    }
}
