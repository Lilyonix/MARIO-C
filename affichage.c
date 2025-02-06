#include "affichage.h"

char** chargerCarte(const char* fichier, int* largeur, int* hauteur, Joueur* joueur) {
    FILE* file = fopen(fichier, "r");
    if (!file) {
        printf("Erreur : impossible de charger la carte.\n");
        exit(1);
    }

    *largeur = 0;
    *hauteur = 0;
    char ligne[1024];

    // Lire la carte pour déterminer sa taille
    while (fgets(ligne, sizeof(ligne), file)) {
        if (*largeur == 0) {
            *largeur = strlen(ligne) - 1; // Retirer le saut de ligne
        }
        (*hauteur)++;
    }
    rewind(file);

    // Allocation dynamique de la carte
    char** carte = malloc(*hauteur * sizeof(char*));
    for (int i = 0; i < *hauteur; i++) {
        carte[i] = malloc(*largeur * sizeof(char));
        fgets(ligne, sizeof(ligne), file);
        strncpy(carte[i], ligne, *largeur);

        // Parcourir la ligne pour détecter les éléments spéciaux
        for (int j = 0; j < *largeur; j++) {
            switch (carte[i][j]) {
                case '7':  // Point de spawn du joueur
                    joueur->x = j * TAILLE_TILE;
                    joueur->y = i * TAILLE_TILE;
                    joueur->checkpointX = joueur->x;
                    joueur->checkpointY = joueur->y;
                    carte[i][j] = '0'; // Remplace le point de spawn par du vide
                    break;

                case 'E':  // Ennemi normal (déplacement gauche/droite)
                    if (nbEnnemis < MAX_ENNEMIS) {
                        ennemis[nbEnnemis] = (Ennemi){
                            .x = j * TAILLE_TILE,
                            .y = i * TAILLE_TILE,
                            .vx = 2, // Vitesse initiale
                            .limiteGauche = (j - 3) * TAILLE_TILE,
                            .limiteDroite = (j + 3) * TAILLE_TILE,
                            .type = NORMAL,
                            .anim = creerAnimation(textureEnnemi, 16, 16, 2, 10),
                            .direction = 1 // Par défaut, va à droite
                        };
                        nbEnnemis++;
                        carte[i][j] = '0';
                    }
                    break;
                case 'V':  // Ennemi normal (déplacement gauche/droite)
                    if (nbEnnemis < MAX_ENNEMIS) {
                        ennemis[nbEnnemis] = (Ennemi){
                            .x = j * TAILLE_TILE,
                            .y = i * TAILLE_TILE,
                            .vx = 4, // Vitesse initiale
                            .limiteGauche = (j - 5) * TAILLE_TILE,
                            .limiteDroite = (j + 5) * TAILLE_TILE,
                            .type = CARAPACEVITE,
                            .anim = creerAnimation(texturecarapace, 16, 16, 2, 10),
                            .direction = 1 // Par défaut, va à droite
                        };
                        nbEnnemis++;
                        carte[i][j] = '0';
                    }
                break;

                case 'M':  // Mascas
                    if (nbEnnemis < MAX_ENNEMIS) {
                        ennemis[nbEnnemis] = (Ennemi){
                            .x = j * TAILLE_TILE,
                            .y = i * TAILLE_TILE,
                            .vx = 2, // Vitesse initiale
                            .limiteGauche = (j - 3) * TAILLE_TILE,
                            .limiteDroite = (j + 3) * TAILLE_TILE,
                            .type = MASCAS,
                            .anim = creerAnimation(texturemascas, 16, 16, 2, 10),
                            .direction = 1 // Par défaut, va à droite
                        };
                        nbEnnemis++;
                        carte[i][j] = '0';
                    }
                break;
                case 'B':  // BOSS
                    if (nbEnnemis < MAX_ENNEMIS) {
                        ennemis[nbEnnemis] = (Ennemi){
                            .x = j * TAILLE_TILE,
                            .y = i * TAILLE_TILE,
                            .vx = 2, // Vitesse initiale
                            .limiteGauche = (j - 3) * TAILLE_TILE,
                            .limiteDroite = (j + 3) * TAILLE_TILE,
                            .type = BOSS, // Enum BOSS
                            .anim = creerAnimation(textureboss, 40, 48, 11, 10),
                            .direction = 1 // Par défaut, orienté vers la droite
                        };
                        nbEnnemis++;
                        carte[i][j] = '0'; // Remplacer le 'B' par du vide
                    }
                break;


                case 'C':  // Carapace (se déplace uniquement vers la droite)
                    if (nbEnnemis < MAX_ENNEMIS) {
                        ennemis[nbEnnemis] = (Ennemi){
                            .x = j * TAILLE_TILE,
                            .y = i * TAILLE_TILE,
                            .vx = 2, // Toujours à droite
                            .limiteGauche = j * TAILLE_TILE,
                            .limiteDroite = (j + 10) * TAILLE_TILE,
                            .type = CARAPACE,
                            .anim = creerAnimation(textureenemiecarapace, 16, 16, 2, 10),
                            .direction = 1 // Toujours à droite
                        };
                        nbEnnemis++;
                        carte[i][j] = '0';
                    }
                    break;

                case 'P':  // Plante Piranha (statique)
                    if (nbEnnemis < MAX_ENNEMIS) {
                        ennemis[nbEnnemis] = (Ennemi){
                            .x = j * TAILLE_TILE,
                            .y = i * TAILLE_TILE,
                            .vx = 0, // Ne bouge pas
                            .limiteGauche = j * TAILLE_TILE,
                            .limiteDroite = j * TAILLE_TILE,
                            .type = PIRANHA,
                            .anim = creerAnimation(textureplant, 16, 32, 2, 10),
                            .direction = 1 // Par défaut
                        };
                        nbEnnemis++;
                        carte[i][j] = '0';
                    }
                    break;
            }
        }
    }
    fclose(file);
    return carte;
}

void libererCarte(char** carte, int hauteur) {
    for (int i = 0; i < hauteur; i++) {
        free(carte[i]);
    }
    free(carte);
}

void afficherCarte(SDL_Renderer* renderer, char** carte, int largeur, int hauteur, int offsetX, int offsetY) {
    for (int y = 0; y < hauteur; y++) {
        for (int x = 0; x < largeur; x++) {
            char bloc = carte[y][x];
            SDL_Rect rect = {x * TAILLE_TILE - offsetX, y * TAILLE_TILE - offsetY, TAILLE_TILE, TAILLE_TILE};

            switch (bloc) {
                case '1': // Bloc solide
                    if (textureBlocSolide) {
                        SDL_RenderCopy(renderer, textureBlocSolide, NULL, &rect);
                    } else {
                        printf("Erreur : textureBlocSolide est NULL.\n");
                    }
                break;

                case 'H':
                    if (texturechampignon) {
                        SDL_RenderCopy(renderer, texturechampignon, NULL, &rect);
                    } else {
                        printf("Erreur : textureBlocSolide est NULL.\n");
                    }
                break;

                case '2': { // Pièce avec animation
                    SDL_Rect source = {
                        frameActuellePiece * 16, // Décale la source horizontalement
                        0,                      // Ligne de la sprite sheet
                        16,                     // Largeur d'une frame
                        16                      // Hauteur d'une frame
                    };
                    SDL_RenderCopy(renderer, textureMort, &source, &rect);
                    break;
                }
                case 'L': { // Pièce avec animation
                    SDL_Rect source = {
                        (frameActuellePiece % 3) * 16,
                        0,                      // Ligne de la sprite sheet
                        16,                     // Largeur d'une frame
                        16                      // Hauteur d'une frame
                    };
                    SDL_RenderCopy(renderer, texturelucky, &source, &rect);
                    break;
                }
                case '4': // Pics
                    SDL_RenderCopy(renderer, texturePic, NULL, &rect);
                break;
                case '5': { // Pièce avec animation
                    SDL_Rect source = {
                        frameActuellePiece * 16, // Décale la source horizontalement
                        0,                      // Ligne de la sprite sheet
                        16,                     // Largeur d'une frame
                        16                      // Hauteur d'une frame
                    };
                    SDL_RenderCopy(renderer, textureAnneau, &source, &rect);
                    break;
                }
                case '6': { // Pièce avec animation
                    SDL_Rect source = {
                        (frameActuellePiece % 3) * 16, // Boucle sur 3 frames (0, 1, 2)
                        0,                            // Ligne de la sprite sheet
                        16,                           // Largeur d'une frame
                        16                            // Hauteur d'une frame
                    };
                    SDL_RenderCopy(renderer, textureCheckpoint, &source, &rect);
                    break;
                }

                case '3': // Fin
                    SDL_RenderCopy(renderer, texturepoteau, NULL, &rect);
                break;
                case 'F': // Fin
                    SDL_RenderCopy(renderer, texturefinpoteau, NULL, &rect);
                break;
                case '9': // Fin
                    SDL_RenderCopy(renderer, texturegrass, NULL, &rect);
                break;
                case 'D': // Fin
                    SDL_RenderCopy(renderer, texturedirt, NULL, &rect);
                break;
                case '7': // Fin
                    SDL_RenderCopy(renderer, textureCheckpoint, NULL, &rect);
                break;
                case 'E':{ // Pièce avec animation
                    SDL_Rect source = {
                        frameActuellePiece * 16, // Décale la source horizontalement
                        0,                      // Ligne de la sprite sheet
                        16,                     // Largeur d'une frame
                        16                      // Hauteur d'une frame
                    };
                    SDL_RenderCopy(renderer, textureAnneau, &source, &rect);
                    break;
                }
                default:
                    break;
            }
        }
    }
}

void afficherJoueur(SDL_Renderer* renderer, Joueur* joueur, int offsetX, int offsetY) {
    Animation* animActuelle;
    int frameIndex = 0;

    // Gestion de l'animation en fonction de l'état du joueur
    if (joueur->estSautant) {
        animActuelle = &joueur->animSaut;
        frameIndex = (joueur->vy < 0) ? 3 : 4;  // 3 pour montée, 4 pour descente
    } else if (joueur->vx != 0) {
        animActuelle = &joueur->animMarche;
        frameIndex = (animActuelle->frameActuelle % 3); // Boucle entre 0, 1 et 2
    } else {
        animActuelle = &joueur->animMarche;
        frameIndex = 0; // Frame statique
    }

    // Définition de la zone de découpe de la sprite sheet
    SDL_Rect source = {
        frameIndex * animActuelle->largeurFrame, // Décalage horizontal
        0,                                       // Ligne de la sprite sheet
        animActuelle->largeurFrame,              // Largeur d'une frame
        animActuelle->hauteurFrame               // Hauteur d'une frame
    };

    SDL_Rect destination = {
        joueur->x - offsetX,                    // Position X à l'écran
        joueur->y - offsetY,                    // Position Y à l'écran
        TAILLE_TILE,                            // Largeur d'affichage
        2 * TAILLE_TILE                         // Hauteur d'affichage
    };

    // Affichage avec gestion du flip en fonction de la direction
    if (joueur->direction == -1) {
        SDL_RenderCopyEx(renderer, animActuelle->texture, &source, &destination, 0, NULL, SDL_FLIP_HORIZONTAL);
    } else {
        SDL_RenderCopy(renderer, animActuelle->texture, &source, &destination);
    }
}

void afficherInterface(SDL_Renderer* renderer, SDL_Texture* texturemariohead, SDL_Texture* textureAnneau, TTF_Font* font, Joueur* joueur) {
    char buffer[64];
    SDL_Color noir = {0, 0, 0, 255}; // Couleur noire pour les textes

    // --- Affichage des vies ---
    SDL_Rect rectTextureVie = {10, 10, 32, 32}; // Position et taille de l'icône de vie
    SDL_RenderCopy(renderer, texturemariohead, NULL, &rectTextureVie);

    // Afficher le nombre de vies à côté de l'icône
    snprintf(buffer, sizeof(buffer), "%d", joueur->vies);
    SDL_Surface* surfaceVies = TTF_RenderText_Solid(font, buffer, noir);
    SDL_Texture* textureScoreVies = SDL_CreateTextureFromSurface(renderer, surfaceVies);
    SDL_Rect rectScoreVies = {50, 10, surfaceVies->w, surfaceVies->h};
    SDL_RenderCopy(renderer, textureScoreVies, NULL, &rectScoreVies);
    SDL_FreeSurface(surfaceVies);
    SDL_DestroyTexture(textureScoreVies);

    // --- Affichage des anneaux (score) ---
    SDL_Rect rectTextureAnneau = {10, 50, 32, 32}; // Position et taille de l'icône d'anneau
    SDL_Rect frameAnneau = {96, 0, 16, 16}; // Première frame (à adapter selon votre sprite sheet)
    SDL_RenderCopy(renderer, textureAnneau, &frameAnneau, &rectTextureAnneau);

    // Afficher le nombre d'anneaux à côté de l'icône
    snprintf(buffer, sizeof(buffer), "%d", joueur->score);
    SDL_Surface* surfaceScore = TTF_RenderText_Solid(font, buffer, noir);
    SDL_Texture* textureScoreAnneaux = SDL_CreateTextureFromSurface(renderer, surfaceScore);
    SDL_Rect rectScoreAnneaux = {50, 50, surfaceScore->w, surfaceScore->h};
    SDL_RenderCopy(renderer, textureScoreAnneaux, NULL, &rectScoreAnneaux);
    SDL_FreeSurface(surfaceScore);
    SDL_DestroyTexture(textureScoreAnneaux);

    // --- Ajout du timer ---
    // Utilisation de SDL_GetTicks pour obtenir le temps écoulé depuis le lancement (en millisecondes)
    Uint32 ticks = SDL_GetTicks();
    int totalSeconds = ticks / 1000;
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;
    // Formatage du timer sous forme mm:ss
    snprintf(buffer, sizeof(buffer), "%02d:%02d", minutes, seconds);

    SDL_Surface* surfaceTimer = TTF_RenderText_Solid(font, buffer, noir);
    SDL_Texture* textureTimer = SDL_CreateTextureFromSurface(renderer, surfaceTimer);
    // Position du timer (ici en haut à droite, ajustez en fonction de votre résolution)
    SDL_Rect rectTimer = {650, 10, surfaceTimer->w, surfaceTimer->h};
    SDL_RenderCopy(renderer, textureTimer, NULL, &rectTimer);
    SDL_FreeSurface(surfaceTimer);
    SDL_DestroyTexture(textureTimer);
}

void afficherEnnemis(SDL_Renderer* renderer, int offsetX, int offsetY) {
    for (int i = 0; i < nbEnnemis; i++) {
        // 1) Calcul de la zone source en fonction de la frameActuelle
        SDL_Rect source = {
            ennemis[i].anim.frameActuelle * ennemis[i].anim.largeurFrame, // Décalage horizontal
            0,                                                            // En général 0 si 1 ligne
            ennemis[i].anim.largeurFrame,                                 // Largeur frame
            ennemis[i].anim.hauteurFrame                                  // Hauteur frame
        };

        // 2) Calcul de la destination
        //    Selon le type, tu peux ajuster la taille à l'écran.
        //    Pour un boss 40x48, tu peux afficher 40x48 à l'écran :

        SDL_Rect destination;

        // Exemple : si c'est un boss
        if (ennemis[i].type == BOSS) {
            destination.x = ennemis[i].x - offsetX;
            destination.y = ennemis[i].y - offsetY;
            destination.w = 2.5*TAILLE_TILE; // Affichage "taille réelle"
            destination.h = 3*TAILLE_TILE;
        }
        // Plante Piranha
        else if (ennemis[i].anim.texture == textureplant) {
            destination.x = ennemis[i].x - offsetX;
            destination.y = ennemis[i].y - offsetY - TAILLE_TILE;
            destination.w = TAILLE_TILE;
            destination.h = 2 * TAILLE_TILE;
        }
        // Ennemi normal
        else {
            destination.x = ennemis[i].x - offsetX;
            destination.y = ennemis[i].y - offsetY;
            destination.w = TAILLE_TILE;
            destination.h = TAILLE_TILE;
        }

        //Flip horizontal si direction == -1
        SDL_RendererFlip flip = (ennemis[i].direction == -1) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

        SDL_RenderCopyEx(renderer, ennemis[i].anim.texture, &source, &destination, 0, NULL, flip);
    }
}

void afficherCadrillage(SDL_Renderer* renderer, int largeur, int hauteur) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100);

    for (int x = 0; x < largeur; x += TAILLE_TILE) {
        SDL_RenderDrawLine(renderer, x, 0, x, hauteur);
    }
    for (int y = 0; y < hauteur; y += TAILLE_TILE) {
        SDL_RenderDrawLine(renderer, 0, y, largeur, y);
    }
}

void afficherTexte(SDL_Renderer* renderer, TTF_Font* font, const char* texte, int x, int y, SDL_Color couleur) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, texte, couleur);
    if (!surface) return;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = {x, y, surface->w, surface->h};

    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void afficherGameOverMenu(SDL_Renderer *renderer) {
    // Effacer l'écran avec un fond noir
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Charger la police (assurez-vous que le fichier "arial.ttf" est accessible)
    TTF_Font *font = TTF_OpenFont("arial.ttf", 48);
    if (!font) {
        printf("Erreur lors du chargement de la police: %s\n", TTF_GetError());
        return;
    }

    // Définir la couleur du texte (blanc)
    SDL_Color white = {255, 255, 255, 255};

    // Créer une surface contenant le texte "GAME OVER"
    SDL_Surface *surface = TTF_RenderText_Solid(font, "GAME OVER", white);
    if (!surface) {
        printf("Erreur lors de la création de la surface: %s\n", TTF_GetError());
        TTF_CloseFont(font);
        return;
    }

    // Créer une texture à partir de la surface
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        printf("Erreur lors de la création de la texture: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        TTF_CloseFont(font);
        return;
    }

    // Obtenir la taille de la texture
    int texW = 0, texH = 0;
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);

    // Obtenir la taille de la fenêtre (ou du renderer)
    int windowW, windowH;
    SDL_GetRendererOutputSize(renderer, &windowW, &windowH);

    // Définir un rectangle pour centrer le texte
    SDL_Rect dstRect;
    dstRect.x = (windowW - texW) / 2;
    dstRect.y = (windowH - texH) / 2;
    dstRect.w = texW;
    dstRect.h = texH;

    // Afficher la texture (le texte) à l'écran
    SDL_RenderCopy(renderer, texture, NULL, &dstRect);
    SDL_RenderPresent(renderer);

    // Attendre que l'utilisateur appuie sur une touche (R ou Q)
    int done = 0;
    while (!done) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                done = 1;
                break;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_q) {
                    // Quitter l'application
                    exit(0);
                } else if (event.key.keysym.sym == SDLK_r) {
                    // Redémarrer le jeu
                    done = 1;
                    // Ici, vous pouvez ajouter le code nécessaire pour réinitialiser votre jeu.
                    // Par exemple, remettre à zéro le score, les vies, la position du joueur, etc.
                }
            }
        }
        SDL_Delay(16); // Petite pause pour limiter l'utilisation CPU
    }

    // Nettoyer les ressources utilisées par le menu Game Over
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    TTF_CloseFont(font);
}