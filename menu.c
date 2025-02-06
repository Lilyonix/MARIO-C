#include "menu.h"

char* afficherMenu(SDL_Renderer* renderer, TTF_Font* font, EtatJeu* etatActuel) {
    int enCours = 1;
    SDL_Event event;
    char* niveauSelectionne = NULL;

    SDL_Texture* background     = chargerTexture(renderer, "textures/menubg.bmp");
    SDL_Texture* boutonJouer    = chargerTexture(renderer, "textures/boutonjouer.bmp");
    SDL_Texture* boutonDeux     = chargerTexture(renderer, "textures/boutonjouer2j.bmp");
    SDL_Texture* boutonCreer    = chargerTexture(renderer, "textures/editeurbouton.bmp");

    // Indice du bouton sélectionné (0,1,2)
    // 0 = JEU, 1 = JOUERDEUX, 2 = CREER
    int boutonSelectionne = 0;
    const int NB_BOUTONS  = 3; // Nombre total d'options

    // Définition des rectangles pour chaque bouton
    SDL_Rect destJouer = {50, 300, 250, 40};
    SDL_Rect destDeux  = {50, 360, 380, 40};
    SDL_Rect destCreer = {50, 420, 300, 40};

    while (enCours) {
        while (SDL_PollEvent(&event)) {
            // Quitte
            if (event.type == SDL_QUIT) {
                *etatActuel = QUITTER;
                return NULL;
            }
            // Navigation au clavier
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_DOWN) {
                    // Passe au bouton suivant
                    boutonSelectionne = (boutonSelectionne + 1) % NB_BOUTONS;
                }
                else if (event.key.keysym.sym == SDLK_UP) {
                    // Passe au bouton précédent
                    boutonSelectionne = (boutonSelectionne - 1 + NB_BOUTONS) % NB_BOUTONS;
                }
                else if (event.key.keysym.sym == SDLK_RETURN) {
                    // Validation
                    switch (boutonSelectionne) {
                        case 0:  *etatActuel = JEU;        break; // 1 joueur
                        case 1:  *etatActuel = JOUERDEUX;  break; // 2 joueurs
                        case 2:  *etatActuel = CREER;      break; // Éditeur
                    }
                    return NULL;
                }
            }
            // Clic souris
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x;
                int y = event.button.y;
                // Bouton JOUER (1 joueur)
                if (x >= destJouer.x && x <= destJouer.x + destJouer.w &&
                    y >= destJouer.y && y <= destJouer.y + destJouer.h) {
                    *etatActuel = JEU;
                    return NULL;
                }
                // Bouton JOUER À DEUX
                if (x >= destDeux.x && x <= destDeux.x + destDeux.w &&
                    y >= destDeux.y && y <= destDeux.y + destDeux.h) {
                    *etatActuel = JOUERDEUX;
                    return NULL;
                }
                // Bouton CRÉER
                if (x >= destCreer.x && x <= destCreer.x + destCreer.w &&
                    y >= destCreer.y && y <= destCreer.y + destCreer.h) {
                    *etatActuel = CREER;
                    return NULL;
                }
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, background, NULL, NULL); // Fond d'écran

        // Mise en surbrillance
        if (boutonSelectionne == 0) SDL_SetTextureColorMod(boutonJouer, 255, 200, 200);
        else                        SDL_SetTextureColorMod(boutonJouer, 255, 255, 255);

        if (boutonSelectionne == 1) SDL_SetTextureColorMod(boutonDeux, 255, 200, 200);
        else                        SDL_SetTextureColorMod(boutonDeux, 255, 255, 255);

        if (boutonSelectionne == 2) SDL_SetTextureColorMod(boutonCreer, 255, 200, 200);
        else                        SDL_SetTextureColorMod(boutonCreer, 255, 255, 255);

        // Dessiner les 3 boutons
        SDL_RenderCopy(renderer, boutonJouer, NULL, &destJouer);
        SDL_RenderCopy(renderer, boutonDeux,  NULL, &destDeux);
        SDL_RenderCopy(renderer, boutonCreer, NULL, &destCreer);

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }

    return NULL;
}

char* selectionnerNiveau(SDL_Renderer* renderer, TTF_Font* font) {
    DIR* d;
    struct dirent* dir;
    char niveaux[10][256];
    int nbNiveaux = 0;
    int selection = 0;

    d = opendir("niveaux/");
    if (!d) {
        printf("Erreur : impossible d'ouvrir le dossier des niveaux.\n");
        return NULL;
    }

    while ((dir = readdir(d)) != NULL && nbNiveaux < 10) {
        if (strstr(dir->d_name, ".txt")) {
            strcpy(niveaux[nbNiveaux], dir->d_name);
            nbNiveaux++;
        }
    }
    closedir(d);

    int enCours = 1;
    SDL_Event event;
    SDL_Texture* background = chargerTexture(renderer, "textures/fondselecteur.bmp");

    while (enCours) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return NULL;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_DOWN) selection = (selection + 1) % nbNiveaux;
                if (event.key.keysym.sym == SDLK_UP) selection = (selection - 1 + nbNiveaux) % nbNiveaux;
                if (event.key.keysym.sym == SDLK_RETURN) {
                    char* selectionFinale = malloc(strlen(niveaux[selection]) + 10);
                    sprintf(selectionFinale, "niveaux/%s", niveaux[selection]);
                    return selectionFinale;
                }
            }
        }

        SDL_RenderCopy(renderer, background, NULL, NULL); // Fond d’écran

        // Liste de Niveaux
        SDL_Color blanc = {255, 255, 255, 255};
        SDL_Color noir = {0, 0, 0, 255};  // Couleur pour les noms
        SDL_Color rouge = {255, 50, 50, 255}; // Surbrillance

        afficherTexte(renderer, font, "Liste de Niveaux :", 250, 100, blanc);

        // Affichage des niveaux
        for (int i = 0; i < nbNiveaux; i++) {
            if (i == selection) {
                // Rectangle rouge en arrière-plan pour le niveau sélectionné
                SDL_Rect surbrillance = {290, 150 + i * 40, 300, 35};
                SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255);
                SDL_RenderFillRect(renderer, &surbrillance);
            }

            // Afficher le texte en noir sur le rectangle rouge
            afficherTexte(renderer, font, niveaux[i], 300, 150 + i * 40, noir);
        }

        SDL_RenderPresent(renderer);
    }
    return NULL;
}

int listerNiveaux(Niveau niveaux[], int maxNiveaux) {
    DIR *d;
    struct dirent *dir;
    int count = 0;
    d = opendir("niveaux/");
    if (!d) {
        printf("Erreur : impossible d'ouvrir le dossier niveaux/\n");
        return 0;
    }
    while ((dir = readdir(d)) != NULL && count < maxNiveaux) {
        if (strstr(dir->d_name, ".txt")) { // Filtre les fichiers .txt
            strncpy(niveaux[count].nom, dir->d_name, 255);
            niveaux[count].nom[255] = '\0';
            count++;
        }
    }
    closedir(d);
    return count;
}

int selectionnerPersonnage(SDL_Renderer* renderer, TTF_Font* font) {
    int selection = 0;
    SDL_Event event;
    int enCours = 1;

    while (enCours) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return -1;  // Quitter
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_LEFT) {
                    selection = (selection - 1 + 4) % 4;
                }
                if (event.key.keysym.sym == SDLK_RIGHT) {
                    selection = (selection + 1) % 4;
                }
                if (event.key.keysym.sym == SDLK_RETURN) {
                    return selection;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        afficherTexte(renderer, font, "Selectionnez un personnage", 220, 50, (SDL_Color){255, 255, 255, 255});

        // Affichage des personnages avec surbrillance sur celui sélectionné
        for (int i = 0; i < 4; i++) {
            SDL_Rect dest = {100 + i * 150, 200, 100, 200}; // Position et taille des sprites
            SDL_Rect source = {0, 0, 16, 32}; // Afficher uniquement la première frame

            if (texturesPersonnages[i]) {
                SDL_RenderCopy(renderer, texturesPersonnages[i], &source, &dest);
            }

            // Mettre un cadre autour du personnage sélectionné
            if (i == selection) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                SDL_RenderDrawRect(renderer, &dest);
            }

            afficherTexte(renderer, font, nomsPersonnages[i], 100 + i * 150, 400, (SDL_Color){255, 255, 255, 255});
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    return 0;
}

void menuPrincipal(SDL_Renderer* renderer, EtatJeu* etatActuel) {
    // police pour le texte
    TTF_Font* font = TTF_OpenFont("arial.ttf", 48);
    if (!font) {
        printf("Erreur : impossible de charger la police.\n");
        exit(1);
    }

    int enCours = 1;

    while (enCours) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                *etatActuel = QUITTER;
                enCours = 0;
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_1) {
                    *etatActuel = JEU;
                    enCours = 0;
                } else if (e.key.keysym.sym == SDLK_2) {
                    *etatActuel = MENU;
                    enCours = 0;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Affichage des options
        SDL_Color blanc = {255, 255, 255, 255};
        SDL_Surface* surfaceTitre = TTF_RenderText_Solid(font, "Menu Principal", blanc);
        SDL_Texture* textureTitre = SDL_CreateTextureFromSurface(renderer, surfaceTitre);

        SDL_Surface* surfaceJouer = TTF_RenderText_Solid(font, "1 - Jouer", blanc);
        SDL_Texture* textureJouer = SDL_CreateTextureFromSurface(renderer, surfaceJouer);

        SDL_Surface* surfaceCreer = TTF_RenderText_Solid(font, "2 - Creer un Niveau", blanc);
        SDL_Texture* textureCreer = SDL_CreateTextureFromSurface(renderer, surfaceCreer);

        SDL_Rect rectTitre = {200, 100, surfaceTitre->w, surfaceTitre->h};
        SDL_Rect rectJouer = {200, 200, surfaceJouer->w, surfaceJouer->h};
        SDL_Rect rectCreer = {200, 300, surfaceCreer->w, surfaceCreer->h};

        SDL_RenderCopy(renderer, textureTitre, NULL, &rectTitre);
        SDL_RenderCopy(renderer, textureJouer, NULL, &rectJouer);
        SDL_RenderCopy(renderer, textureCreer, NULL, &rectCreer);

        SDL_RenderPresent(renderer);

        // Libération des surfaces et textures
        SDL_FreeSurface(surfaceTitre);
        SDL_FreeSurface(surfaceJouer);
        SDL_FreeSurface(surfaceCreer);
        SDL_DestroyTexture(textureTitre);
        SDL_DestroyTexture(textureJouer);
        SDL_DestroyTexture(textureCreer);

        SDL_Delay(16);
    }

    TTF_CloseFont(font);
}

void afficherGameOverScreen(SDL_Renderer* renderer) {
    // Charger la police pour le texte "GAME OVER"
    TTF_Font* font = TTF_OpenFont("arial.ttf", 48);
    if (!font) {
        printf("Erreur lors du chargement de la police pour Game Over : %s\n", TTF_GetError());
        return;
    }

    // Effacer l'écran avec une couleur noire
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Définir la couleur du texte (blanc)
    SDL_Color white = {255, 255, 255, 255};

    // Créer une surface contenant le texte "GAME OVER"
    SDL_Surface* surface = TTF_RenderText_Solid(font, "GAME OVER", white);
    if (!surface) {
        printf("Erreur lors de la création de la surface Game Over : %s\n", TTF_GetError());
        TTF_CloseFont(font);
        return;
    }

    // Créer une texture à partir de la surface
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        printf("Erreur lors de la création de la texture Game Over : %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        TTF_CloseFont(font);
        return;
    }

    // Obtenir la taille de la texture
    int texW = 0, texH = 0;
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);

    // Obtenir la taille du renderer (ou de la fenêtre)
    int windowW = 0, windowH = 0;
    SDL_GetRendererOutputSize(renderer, &windowW, &windowH);

    // Définir un rectangle pour centrer le texte Game Over
    SDL_Rect dstRect;
    dstRect.x = (windowW - texW) / 2;
    dstRect.y = (windowH - texH) / 2;
    dstRect.w = texW;
    dstRect.h = texH;

    // Afficher la texture (le texte Game Over) à l'écran
    SDL_RenderCopy(renderer, texture, NULL, &dstRect);
    SDL_RenderPresent(renderer);

    // Boucle d'attente pour que l'utilisateur choisisse une action
    int running = 1;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                exit(0);
            }
            if (event.type == SDL_KEYDOWN) {
                // Appuyer sur Q quitte l'application
                if (event.key.keysym.sym == SDLK_q) {
                    exit(0);
                }
                // Appuyer sur R quitte l'écran Game Over (pour redémarrer)
                if (event.key.keysym.sym == SDLK_r) {
                    running = 0;
                }
            }
        }
        SDL_Delay(16); // Pause pour limiter l'utilisation CPU (~60 FPS)
    }

    // Libérer les ressources utilisées par l'écran Game Over
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    TTF_CloseFont(font);
}