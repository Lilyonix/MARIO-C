#include "afficheranimation.h"



void mettreAJourAnimation(Animation* anim) {
    anim->compteur++;
    if (anim->compteur >= anim->vitesse) {
        anim->frameActuelle = (anim->frameActuelle + 1) % anim->nbFrames; // Boucle les frames
        anim->compteur = 0;
    }
}



Animation creerAnimation(SDL_Texture* texture, int largeur, int hauteur,
                         int nbFrames, int vitesse) {
    Animation anim;
    anim.texture = texture;
    anim.largeurFrame = largeur;
    anim.hauteurFrame = hauteur;
    anim.nbFrames = nbFrames;
    anim.vitesse = vitesse;         // Pour gérer la vitesse d'animation
    anim.frameActuelle = 0;// Pour incrémenter au fil du temps
    return anim;
}


