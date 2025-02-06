#include "deplacement.h"

void gererCollisionsAvecEnnemis(Joueur* joueur) {
    static int bossHits = 0; // Nombre de coups reçus par le boss

    for (int i = 0; i < nbEnnemis; i++) {
        SDL_Rect rectJoueur = {joueur->x, joueur->y, TAILLE_TILE, 2 * TAILLE_TILE}; // Taille du joueur
        SDL_Rect rectEnnemi;

        if (ennemis[i].type == PIRANHA) {
            rectEnnemi = (SDL_Rect){
                ennemis[i].x, ennemis[i].y - TAILLE_TILE, TAILLE_TILE, 2 * TAILLE_TILE
            }; // Plante piranha
        }
        else if (ennemis[i].type == BOSS) {
            rectEnnemi = (SDL_Rect){
                ennemis[i].x, ennemis[i].y, 2 * TAILLE_TILE, 2 * TAILLE_TILE
            }; // Boss 2x2
        }
        else {
            rectEnnemi = (SDL_Rect){
                ennemis[i].x, ennemis[i].y, TAILLE_TILE, TAILLE_TILE
            }; // Ennemis de base 1x1
        }

        // Vérifier la collision entre le joueur et l'ennemi
        if (SDL_HasIntersection(&rectJoueur, &rectEnnemi)) {
            if (ennemis[i].type == BOSS) {
                // Si le joueur touche le boss sur le côté ou en-dessous, il meurt
                if (!(joueur->vy > 0 && joueur->y + 2 * TAILLE_TILE <= ennemis[i].y + 10)) {
                    printf("Le joueur a touché le boss et meurt instantanément !\n");
                    printf("GAME OVER\n");
                    exit(0);
                }

                // Si le joueur saute sur la tête du boss
                bossHits++;
                printf("Boss touché ! %d/5 \n", bossHits);
                if (bossHits >= 5) {
                    printf("BOSS vaincu ! \n");

                    // Supprimer le boss
                    for (int j = i; j < nbEnnemis - 1; j++) {
                        ennemis[j] = ennemis[j + 1]; // Décale les ennemis
                    }
                    nbEnnemis--;
                    bossHits = 0; // Réinitialisation après la mort du boss
                }

                // Rebond du joueur après un coup réussi
                joueur->vy = FORCE_SAUT / 1.5;
            }
            else if (ennemis[i].type == PIRANHA || ennemis[i].type == CARAPACE) {
                // Peu importe comment le joueur touche ces ennemis il meurt
                printf("Touché par une plante piranha ou une carapace !\n");
                joueur->vies--;
                joueur->x = joueur->checkpointX;
                joueur->y = joueur->checkpointY;
                if (joueur->vies <= 0) {
                    printf("GAME OVER \n");
                    exit(0);
                }
            }
            else {
                // Si le joueur saute sur un ennemi NORMAL
                if (joueur->vy > 0 && joueur->y + 2 * TAILLE_TILE <= ennemis[i].y + 10) {
                    printf("Ennemi écrasé !\n");

                    // Supprimer l'ennemi
                    for (int j = i; j < nbEnnemis - 1; j++) {
                        ennemis[j] = ennemis[j + 1]; // Décale les ennemis
                    }
                    nbEnnemis--;

                    // Rebond du joueur
                    joueur->vy = FORCE_SAUT / 1.5;
                }
                else {
                    // collision normale = le joueur perd une vie
                    printf("Touché par un ennemi !\n");
                    joueur->vies--;
                    joueur->x = joueur->checkpointX;
                    joueur->y = joueur->checkpointY;
                    if (joueur->vies <= 0) {
                        printf("GAME OVER \n");
                        exit(0);
                    }
                }
            }
        }
    }
}

void gererCollisions(Joueur* joueur, char** carte, int largeur, int hauteur) {
    // Dimensions de la hitbox du joueur
    int joueurWidth  = TAILLE_TILE;
    int joueurHeight = 2 * TAILLE_TILE;

    // Calcul initial de la bounding box du joueur (en pixels)
    int pLeft   = joueur->x;
    int pRight  = joueur->x + joueurWidth;
    int pTop    = joueur->y;
    int pBottom = joueur->y + joueurHeight;

    // Déterminer la zone de la carte à vérifier (en tuiles)
    int tileLeft   = pLeft / TAILLE_TILE;
    int tileRight  = pRight / TAILLE_TILE;
    int tileTop    = pTop / TAILLE_TILE;
    int tileBottom = pBottom / TAILLE_TILE;

    // Parcourir toutes les cases potentielles de collision
    for (int ty = tileTop; ty <= tileBottom; ty++) {
        for (int tx = tileLeft; tx <= tileRight; tx++) {
            if (tx < 0 || tx >= largeur || ty < 0 || ty >= hauteur)
                continue;

            char bloc = carte[ty][tx];
            // Coordonnées du bloc en pixels
            int bLeft   = tx * TAILLE_TILE;
            int bRight  = bLeft + TAILLE_TILE;
            int bTop    = ty * TAILLE_TILE;
            int bBottom = bTop + TAILLE_TILE;

            // Traiter d'abord les cases déclencheuses (actions immédiates)
            if (bloc == '2') {  // Lave ou autre bloc mortel
                joueur->vies--;
                play_sound(mort);
                joueur->x = joueur->checkpointX;
                joueur->y = joueur->checkpointY;
                continue;
            }
            else if (bloc == '6') {  // Checkpoint
                joueur->checkpointX = tx * TAILLE_TILE;
                joueur->checkpointY = ty * TAILLE_TILE;
                carte[ty][tx] = '0';
                continue;
            }
            else if (bloc == '5') {
                joueur->score++;
                play_sound(coin);
                carte[ty][tx] = '0';
                continue;
            }
            else if (bloc == '4') {  // Pics (dégâts)
                joueur->vies--;
                play_sound(mort);
                joueur->x = joueur->checkpointX;
                joueur->y = joueur->checkpointY;
                continue;
            }
            else if (bloc == 'H'){
                carte[ty][tx] = '0';  // Bonus vie collecté
                joueur->vies++;
                play_sound(life);
                continue;
            }
            else if (bloc == '3') {
                joueur->gagner = 1;
                continue;
            }

            // Pour les blocs de collision (solides et cassables)
            if (bloc == '1' || bloc == '9' || bloc == 'D' || bloc == 'L') {
                // Traitement spécifique pour les blocs cassables par en dessous
                if (bloc == 'L' && joueur->vy < 0 && pTop < bBottom && pBottom > bBottom) {
                    carte[ty][tx] = '0';  // Le bloc cassable est supprimé
                    play_sound(casse);    // Joue le son de casse
                    joueur->vy = 0;

                    // Utilisation d'un compteur statique pour alterner les bonus
                    static int bonusAlternateur = 0;
                    bonusAlternateur++;
                    if (bonusAlternateur % 2 == 0) {
                        // Pour les valeurs paires, on fait apparaître une pièce (code '5')
                        if (ty - 1 >= 0 && carte[ty - 1][tx] == '0') {  // Vérifie que la case au-dessus est libre
                            carte[ty - 1][tx] = '5';
                        }
                    } else {
                        // Pour les valeurs impaires, on fait apparaître une vie (code 'H')
                        if (ty - 1 >= 0 && carte[ty - 1][tx] == '0') {
                            carte[ty - 1][tx] = 'H';
                        }
                    }
                    continue;
                }

                // Calcul de l'intersection entre la hitbox du joueur et celle du bloc
                int interLeft   = (pLeft   > bLeft)   ? pLeft   : bLeft;
                int interRight  = (pRight  < bRight)  ? pRight  : bRight;
                int interTop    = (pTop    > bTop)    ? pTop    : bTop;
                int interBottom = (pBottom < bBottom) ? pBottom : bBottom;

                int interWidth  = interRight - interLeft;
                int interHeight = interBottom - interTop;

                // S'il y a collision
                if (interWidth > 0 && interHeight > 0) {
                    // Résolution dans la direction de la pénétration minimale
                    if (interWidth < interHeight) {
                        // Collision horizontale
                        if (pLeft < bLeft) {
                            // Collision par la gauche : positionner le joueur à gauche du bloc
                            joueur->x = bLeft - joueurWidth;
                        } else {
                            // Collision par la droite : positionner le joueur à droite du bloc
                            joueur->x = bRight;
                        }
                        joueur->vx = 0;
                    } else {
                        // Collision verticale
                        if (pTop < bTop) {
                            // Le joueur tombe sur le bloc
                            joueur->y = bTop - joueurHeight;
                            joueur->vy = 0;
                            joueur->estSautant = 0;
                        } else {
                            // Le joueur frappe le bloc par le dessous
                            joueur->y = bBottom;
                            joueur->vy = 0;
                        }
                    }
                    // Mise à jour de la bounding box du joueur après ajustement
                    pLeft   = joueur->x;
                    pRight  = joueur->x + joueurWidth;
                    pTop    = joueur->y;
                    pBottom = joueur->y + joueurHeight;
                }
            } // Fin traitement bloc solide
        }
    }

    // Empêcher le joueur de sortir de l'écran à gauche ou à droite
    // Ici, on suppose que la largeur totale de la carte en pixels est largeur * TAILLE_TILE
    if (joueur->x < 0)
        joueur->x = 0;
    if (joueur->x > (largeur * TAILLE_TILE) - joueurWidth)
        joueur->x = (largeur * TAILLE_TILE) - joueurWidth;
}


void gererEvenementJoueur1(Joueur* joueur, SDL_Event* e, int* enCours)
{
    if (e->type == SDL_QUIT) {
        *enCours = 0;
    }
    else if (e->type == SDL_KEYDOWN) {
        switch (e->key.keysym.sym) {
            case SDLK_LEFT:
                joueur->vx = -5;
                joueur->direction = -1;
                break;
            case SDLK_RIGHT:
                joueur->vx = 5;
                joueur->direction = 1;
                break;
            case SDLK_SPACE:
                if (!joueur->estSautant) {
                    joueur->vy = FORCE_SAUT;
                    joueur->estSautant = 1;
                }
                break;
            default:
                break;
        }
    }
    else if (e->type == SDL_KEYUP) {
        switch (e->key.keysym.sym) {
            case SDLK_LEFT:
            case SDLK_RIGHT:
                joueur->vx = 0;
                break;
            default:
                break;
        }
    }
}

void gererEvenementJoueur2(Joueur* joueur2, SDL_Event* e, int* enCours)
{
    if (e->type == SDL_QUIT) {
        *enCours = 0;
    }
    else if (e->type == SDL_KEYDOWN) {
        switch (e->key.keysym.sym) {
            case SDLK_q:
                joueur2->vx = -5;
                joueur2->direction = -1;
                break;
            case SDLK_d:
                joueur2->vx = 5;
                joueur2->direction = 1;
                break;
            case SDLK_z:
                if (!joueur2->estSautant) {
                    joueur2->vy = FORCE_SAUT;
                    joueur2->estSautant = 1;
                }
                break;
            default:
                break;
        }
    }
    else if (e->type == SDL_KEYUP) {
        switch (e->key.keysym.sym) {
            case SDLK_q:
            case SDLK_d:
                joueur2->vx = 0;
                break;
            default:
                break;
        }
    }
}


void gererCollisions2(Joueur* joueur,Joueur* joueurinitial, char** carte, int largeur, int hauteur) {
    // Calculer les positions dans la grille
    int coinGaucheX = (joueur->x + 5) / TAILLE_TILE; // +5 pour ajuster les petits décollages
    int coinDroitX = (joueur->x + TAILLE_TILE - 5) / TAILLE_TILE;
    int basY = (joueur->y + 2 * TAILLE_TILE - 1) / TAILLE_TILE; // Bas du joueur
    int hautY = (joueur->y + 5) / TAILLE_TILE; // +5 pour éviter les erreurs de plafond

    // Vérifier les collisions avec les blocs
    for (int y = hautY; y <= basY; y++) {
        for (int x = coinGaucheX; x <= coinDroitX; x++) {
            if (y >= 0 && y < hauteur && x >= 0 && x < largeur) {
                char bloc = carte[y][x];

                switch (bloc) {
                    case '1': // Bloc solide
                    case '9': // Bloc indestructible
                    case 'D': // Bloc décoratif
                        // Collision sol
                        if (joueur->vy > 0 && joueur->y + 2 * TAILLE_TILE >= y * TAILLE_TILE) {
                            joueur->y = y * TAILLE_TILE - 2 * TAILLE_TILE; // Bloque au sol
                            joueur->vy = 0;
                            joueur->estSautant = 0;
                        }
                        // Collision plafond
                        else if (joueur->vy < 0 && joueur->y <= (y + 1) * TAILLE_TILE) {
                            joueur->y = (y + 1) * TAILLE_TILE; // Bloque en dessous
                            joueur->vy = 0;
                        }
                        break;

                    case '2': // Lave
                        joueur->vies--;
                        joueur->x = joueur->checkpointX;
                        joueur->y = joueur->checkpointY;
                        break;

                    case '6': // Checkpoint
                        joueur->checkpointX = joueurinitial->checkpointX * TAILLE_TILE;
                        joueur->checkpointY = joueurinitial->checkpointY * TAILLE_TILE;
                        carte[y][x] = '0'; // Supprime le checkpoint après activation
                        break;

                    case '5': // Pièce
                        joueur->score++;
                        carte[y][x] = '0'; // Supprime la pièce
                        printf("Anneaux collectés : %d\n", joueur->score);
                        break;

                    case '4': // Pics (dégâts)
                        joueur->vies--;
                        joueur->x = joueur->checkpointX;
                        joueur->y = joueur->checkpointY;
                        printf("Touché par un piège ! Vies restantes : %d\n", joueur->vies);
                        if (joueur->vies <= 0) {
                            printf("Game Over\n");
                            exit(0);
                        }
                        break;

                    case 'L': // Bloc cassable par en dessous
                        // Collision sol
                        if (joueur->vy > 0 && joueur->y + 2 * TAILLE_TILE >= y * TAILLE_TILE) {
                            joueur->y = y * TAILLE_TILE - 2 * TAILLE_TILE;
                            joueur->vy = 0;
                            joueur->estSautant = 0;
                        }
                        // Casse le bloc si le joueur saute en dessous
                        else if (joueur->vy < 0 && joueur->y <= (y + 1) * TAILLE_TILE) {
                            carte[y][x] = '0'; // Supprime le bloc
                            joueur->vy = 0;
                            printf("Bloc cassé !\n");
                        }
                        break;
                }
            }
        }
    }

    // Gérer les collisions latérales pour ne pas bloquer les mouvements
    coinGaucheX = (joueur->x) / TAILLE_TILE;
    coinDroitX = (joueur->x + TAILLE_TILE - 1) / TAILLE_TILE;
    basY = (joueur->y + 2 * TAILLE_TILE - 5) / TAILLE_TILE; // -5 pour éviter les erreurs de colision sur les coins

    for (int y = hautY; y <= basY; y++) {
        for (int x = coinGaucheX; x <= coinDroitX; x++) {
            if (y >= 0 && y < hauteur && x >= 0 && x < largeur) {
                char bloc = carte[y][x];

                if (bloc == '1' || bloc == '9' || bloc == 'D' || bloc == 'L') {
                    // Collision latérale droite
                    if (joueur->vx > 0 && joueur->x + TAILLE_TILE >= x * TAILLE_TILE) {
                        joueur->x = x * TAILLE_TILE - TAILLE_TILE; // Bloque à droite
                        joueur->vx = 0;
                    }
                    // Collision latérale gauche
                    if (joueur->vx < 0 && joueur->x <= (x + 1) * TAILLE_TILE) {
                        joueur->x = (x + 1) * TAILLE_TILE; // Bloque à gauche
                        joueur->vx = 0;
                    }
                }
            }
        }
    }
}


void gererCollisions1(Joueur* joueur,Joueur* joueurinitial, char** carte, int largeur, int hauteur) {
    // Calculer les positions dans la grille
    int coinGaucheX = (joueur->x + 5) / TAILLE_TILE; // +5 pour ajuster les petits décollages
    int coinDroitX = (joueur->x + TAILLE_TILE - 5) / TAILLE_TILE;
    int basY = (joueur->y + 2 * TAILLE_TILE - 1) / TAILLE_TILE; // Bas du joueur
    int hautY = (joueur->y + 5) / TAILLE_TILE; // +5 pour éviter les erreurs de plafond

    // Vérifier les collisions avec les blocs
    for (int y = hautY; y <= basY; y++) {
        for (int x = coinGaucheX; x <= coinDroitX; x++) {
            if (y >= 0 && y < hauteur && x >= 0 && x < largeur) {
                char bloc = carte[y][x];

                switch (bloc) {
                    case '1': // Bloc solide
                    case '9': // Bloc indestructible
                    case 'D': // Bloc décoratif
                        // Collision sol (empêche de tomber)
                        if (joueur->vy > 0 && joueur->y + 2 * TAILLE_TILE >= y * TAILLE_TILE) {
                            joueur->y = y * TAILLE_TILE - 2 * TAILLE_TILE; // Bloque au sol
                            joueur->vy = 0;
                            joueur->estSautant = 0;
                        }
                        // Collision plafond (empêche de monter)
                        else if (joueur->vy < 0 && joueur->y <= (y + 1) * TAILLE_TILE) {
                            joueur->y = (y + 1) * TAILLE_TILE; // Bloque en dessous
                            joueur->vy = 0;
                        }
                        break;

                    case '2': // Lave
                        joueur->vies--;
                        joueur->x = joueur->checkpointX;
                        joueur->y = joueur->checkpointY;

                        break;

                    case '6': // Checkpoint
                        joueur->checkpointX = joueurinitial->checkpointX * TAILLE_TILE;
                        joueur->checkpointY = joueurinitial->checkpointY * TAILLE_TILE;
                        carte[y][x] = '0'; // Supprime le checkpoint après activation
                        break;

                    case '5': // Pièce
                        joueur->score++;
                        carte[y][x] = '0'; // Supprime la pièce
                        printf("Anneaux collectés : %d\n", joueur->score);
                        break;

                    case '4': // Pics (dégâts)
                        joueur->vies--;
                        joueur->x = joueur->checkpointX;
                        joueur->y = joueur->checkpointY;
                        printf("Touché par un piège ! Vies restantes : %d\n", joueur->vies);
                        if (joueur->vies <= 0) {
                            printf("Game Over\n");
                            exit(0);
                        }
                        break;

                    case 'L': // Bloc cassable par en dessous
                        // Collision sol (empêche de tomber)
                        if (joueur->vy > 0 && joueur->y + 2 * TAILLE_TILE >= y * TAILLE_TILE) {
                            joueur->y = y * TAILLE_TILE - 2 * TAILLE_TILE;
                            joueur->vy = 0;
                            joueur->estSautant = 0;
                        }
                        // Casse le bloc si le joueur saute en dessous
                        else if (joueur->vy < 0 && joueur->y <= (y + 1) * TAILLE_TILE) {
                            carte[y][x] = '0'; // Supprime le bloc
                            joueur->vy = 0;
                            printf("Bloc cassé !\n");
                        }
                        break;
                }
            }
        }
    }

    // Gérer les collisions latérales pour ne pas bloquer les mouvements
    coinGaucheX = (joueur->x) / TAILLE_TILE;
    coinDroitX = (joueur->x + TAILLE_TILE - 1) / TAILLE_TILE;
    basY = (joueur->y + 2 * TAILLE_TILE - 5) / TAILLE_TILE; // -5 pour éviter les erreurs de colision sur les coins

    for (int y = hautY; y <= basY; y++) {
        for (int x = coinGaucheX; x <= coinDroitX; x++) {
            if (y >= 0 && y < hauteur && x >= 0 && x < largeur) {
                char bloc = carte[y][x];

                if (bloc == '1' || bloc == '9' || bloc == 'D' || bloc == 'L') {
                    // Collision latérale droite
                    if (joueur->vx > 0 && joueur->x + TAILLE_TILE >= x * TAILLE_TILE) {
                        joueur->x = x * TAILLE_TILE - TAILLE_TILE; // Bloque à droite
                        joueur->vx = 0;
                    }
                    // Collision latérale gauche
                    if (joueur->vx < 0 && joueur->x <= (x + 1) * TAILLE_TILE) {
                        joueur->x = (x + 1) * TAILLE_TILE; // Bloque à gauche
                        joueur->vx = 0;
                    }
                }
            }
        }
    }
}

void gererEvenements(Joueur* joueur, int* enCours) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            *enCours = 0;
        } else if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_LEFT) {
                joueur->vx = -5;
                joueur->direction = -1; // Regarde à gauche
            }
            if (e.key.keysym.sym == SDLK_RIGHT) {
                joueur->vx = 5;
                joueur->direction = 1; // Regarde à droite
            }
            if (e.key.keysym.sym == SDLK_SPACE && !joueur->estSautant) {
                play_sound(jump);
                joueur->vy = FORCE_SAUT;
                joueur->estSautant = 1;
            }
        } else if (e.type == SDL_KEYUP) {
            if (e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_RIGHT) {
                joueur->vx = 0;
            }
        }
    }
}