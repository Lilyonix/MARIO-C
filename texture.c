#include "texture.h"


SDL_Texture* textureDefaut = NULL;


SDL_Texture* textureBlocSolide = NULL;
SDL_Texture* texturechampignon = NULL;
SDL_Texture* texturemariohead = NULL;
SDL_Texture* texturePic = NULL;
SDL_Texture* textureAnneau = NULL;
SDL_Texture* textureCheckpoint = NULL;
SDL_Texture* textureMort = NULL;
SDL_Texture* textureFin = NULL;
SDL_Texture* textureEnnemi = NULL;
SDL_Texture* texturegrass = NULL;
SDL_Texture* texturedirt = NULL;
SDL_Texture* textureplateforme = NULL;
SDL_Texture* textureenemiecarapace = NULL;
SDL_Texture* texturevie = NULL;
SDL_Texture* texturepoteau = NULL;
SDL_Texture* texturefinpoteau = NULL;
SDL_Texture* texturejoueur = NULL;
SDL_Texture* textureplant = NULL;
SDL_Texture* texturefond = NULL;
SDL_Texture* texturelucky = NULL;
SDL_Texture* texturemascas = NULL;
SDL_Texture* texturecarapace = NULL;
SDL_Texture* textureboss = NULL;
SDL_Texture* texturejoueur1 = NULL;
SDL_Texture* texturejoueur2 = NULL;

// Ajout d'un tableau pour stocker les textures des personnages
SDL_Texture* texturesPersonnages[4] = {NULL, NULL, NULL, NULL};
const char* nomsPersonnages[4] = {"Mario", "Luigi", "Peach", "Toad"};
const char* fichiersPersonnages[4] = {
    "textures/mariotest.bmp",
    "textures/luigi.bmp",
    "textures/peach.bmp",
    "textures/toad.bmp"
};



SDL_Texture* chargerTexture(SDL_Renderer* renderer, const char* chemin) {
    SDL_Surface* surface = SDL_LoadBMP(chemin);
    if (!surface) {
        printf("Erreur de chargement de l'image %s : %s\n", chemin, SDL_GetError());
        return NULL;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        printf("Erreur de création de la texture pour %s : %s\n", chemin, SDL_GetError());
    } else {
        printf("Texture chargée avec succès : %s\n", chemin);
    }

    return texture;
}

void chargerToutesLesTextures(SDL_Renderer* renderer) {
    printf("Chargement des textures...\n");

    textureDefaut = chargerTexture(renderer, "textures/defaut.bmp");
    if (!textureDefaut) {
        printf("Erreur : textureDefaut non chargée.\n");
    }
    texturemascas = chargerTexture(renderer, "textures/mascas.bmp");
    textureAnneau = chargerTexture(renderer, "textures/piece.bmp");
    texturecarapace = chargerTexture(renderer, "textures/carapace.bmp");
    textureboss = chargerTexture(renderer, "textures/boss.bmp");
    if (!textureAnneau) {
        printf("Erreur : texture des pièces non trouvée. Utilisation de la texture par défaut.\n");
        textureAnneau = textureDefaut;
    } else {
        printf("Texture des pièces chargée avec succès.\n");
    }

    textureBlocSolide = chargerTexture(renderer, "textures/blocs.bmp");
    if (!textureBlocSolide) {
        printf("Erreur : textureBlocSolide non chargée.\n");
        textureBlocSolide = textureDefaut;
    }

    texturemariohead = chargerTexture(renderer, "textures/mariohead.bmp");

    texturelucky = chargerTexture(renderer, "textures/lucky.bmp");

    texturePic = chargerTexture(renderer, "textures/pike.bmp");
    if (!texturePic) {
        printf("Erreur : texturePic non chargée.\n");
        texturePic = textureDefaut;
    }

    textureCheckpoint = chargerTexture(renderer, "textures/flag.bmp");
    if (!textureCheckpoint) {
        printf("Erreur : textureCheckpoint non chargée.\n");
        textureCheckpoint = textureDefaut;
    }

    texturefond = chargerTexture(renderer, "textures/grass.bmp");
    if (!texturefond) {
        printf("Erreur : texture de fond non chargée. Utilisation de la texture par défaut.\n");
        texturefond = textureDefaut;
    }

    textureMort = chargerTexture(renderer, "textures/lava.bmp");
    if (!textureMort) {
        printf("Erreur : textureMort non chargée.\n");
        textureMort = textureDefaut;
    }

    texturegrass = chargerTexture(renderer, "textures/grass.bmp");
    if (!texturegrass) {
        printf("Erreur : texturegrass non chargée.\n");
        texturegrass = textureDefaut;
    }

    texturedirt = chargerTexture(renderer, "textures/dirt.bmp");
    if (!texturedirt) {
        printf("Erreur : texturedirt non chargée.\n");
        texturedirt = textureDefaut;
    }

    textureplateforme = chargerTexture(renderer, "textures/plateforme.bmp");
    if (!textureplateforme) {
        printf("Erreur : textureplateforme non chargée.\n");
        textureplateforme = textureDefaut;
    }

    textureEnnemi = chargerTexture(renderer, "textures/ennemie.bmp");
    if (!textureEnnemi) {
        printf("Erreur : textureEnnemi non chargée.\n");
        textureEnnemi = textureDefaut;
    }


    textureenemiecarapace = chargerTexture(renderer, "textures/ennemiecarapace.bmp");
    if (!textureenemiecarapace) {
        printf("Erreur : textureenemiecarapace non chargée.\n");
        textureenemiecarapace = textureDefaut;
    }

    texturevie = chargerTexture(renderer, "textures/vie.bmp");
    if (!texturevie) {
        printf("Erreur : texturevie non chargée.\n");
        texturevie = textureDefaut;
    }

    texturefinpoteau = chargerTexture(renderer, "textures/finpoteau.bmp");
    if (!texturefinpoteau) {
        printf("Erreur : texturefinpoteau non chargée.\n");
        texturefinpoteau = textureDefaut;
    }

    texturechampignon = chargerTexture(renderer, "textures/vie.bmp");
    if (!texturechampignon) {
        printf("Erreur : texturefinpoteau non chargée.\n");
        texturefinpoteau = textureDefaut;
    }

    texturepoteau = chargerTexture(renderer, "textures/poteau.bmp");
    if (!texturepoteau) {
        printf("Erreur : texturepoteau non chargée.\n");
        texturepoteau = textureDefaut;
    }

    textureplant = chargerTexture(renderer, "textures/plant.bmp");
    if (!textureplant) {
        printf("Erreur : textureplant non chargée.\n");
        textureplant = textureDefaut;
    }

    printf("Chargement des textures terminé.\n");
}

// Fonction pour charger les textures des personnages
void chargerTexturesPersonnages(SDL_Renderer* renderer) {
    for (int i = 0; i < 4; i++) {
        texturesPersonnages[i] = chargerTexture(renderer, fichiersPersonnages[i]);
        if (!texturesPersonnages[i]) {
            printf("Erreur : impossible de charger %s\n", fichiersPersonnages[i]);
        }
    }
}

void libererToutesLesTextures() {
    SDL_DestroyTexture(textureBlocSolide);
    SDL_DestroyTexture(texturePic);
    SDL_DestroyTexture(textureAnneau);
    SDL_DestroyTexture(textureCheckpoint);
    SDL_DestroyTexture(textureMort);
    SDL_DestroyTexture(textureFin);
    SDL_DestroyTexture(textureEnnemi);
    SDL_DestroyTexture(texturejoueur);
    SDL_DestroyTexture(textureDefaut);
}