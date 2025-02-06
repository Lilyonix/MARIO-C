#include "creationdeniveau.h"

int demanderLongueurNiveau() {
    int longueur = 50; // Valeur par défaut
    printf("Entrez la longueur du niveau (min 10, max 500) : ");
    scanf("%d", &longueur);

    if (longueur < 10) longueur = 10;
    if (longueur > 500) longueur = 500;

    return longueur;
}

char** creerCarteVide(int largeur) {
    int hauteur = 15; // Hauteur fixe du niveau
    char** carte = malloc(hauteur * sizeof(char*));

    for (int y = 0; y < hauteur; y++) {
        carte[y] = malloc(largeur * sizeof(char));
        for (int x = 0; x < largeur; x++) {
            if (y == hauteur - 1) {
                carte[y][x] = 'D'; // Sol
            } else {
                carte[y][x] = '0'; // Vide
            }
        }
    }
    return carte;
}

void gererSouris(char** carte, int largeur, int hauteur, SDL_Event* e, char elementSelectionne) {
    // On traite les événements MOUSEBUTTONDOWN ET MOUSEMOTION
    if (e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEMOTION) {
        // Récupérer la position actuelle de la souris
        int x = e->motion.x / TAILLE_TILE;
        int y = e->motion.y / TAILLE_TILE;

        // Vérifier qu'on est dans la carte
        if (x >= 0 && x < largeur && y >= 0 && y < hauteur) {
            // Vérifier si on appuie sur le bouton gauche
            if (e->motion.state & SDL_BUTTON_LMASK) {
                // Placer un bloc
                carte[y][x] = elementSelectionne;
            }
            // Vérifier si on appuie sur le bouton droit
            else if (e->motion.state & SDL_BUTTON_RMASK) {
                // Effacer un bloc
                carte[y][x] = '0';
            }
        }
    }
}


int afficherSelection(SDL_Renderer* renderer, SDL_Event* event, int* selectionIndex) {
    // Tableau des éléments
    #define NB_ELEMENTS 7
    char elements[NB_ELEMENTS] = {'1', '2', '3', 'L', 'E', 'C', 'P'};
    const char* noms[NB_ELEMENTS] = {"Bloc", "Anneau", "Checkpoint", "Lucky", "Ennemi", "Carapace", "Plante"};

    // Création des cases pour cliquer
    SDL_Rect cases[NB_ELEMENTS];

    for (int i = 0; i < NB_ELEMENTS; i++) {
        // Chaque case fait 50 pixels de large, 50 de haut
        cases[i].x = 10 + i * 60;         // Espacées de 60 px
        cases[i].y = HAUTEUR_ECRAN - 60;  // Barre en bas
        cases[i].w = 50;
        cases[i].h = 50;

        // On dessine un rectangle gris
        SDL_SetRenderDrawColor(renderer, 160, 160, 160, 255);
        SDL_RenderFillRect(renderer, &cases[i]);
    }

    // Dessiner un cadre jaune autour de l'élément sélectionné
    if (*selectionIndex >= 0 && *selectionIndex < NB_ELEMENTS) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_RenderDrawRect(renderer, &cases[*selectionIndex]);
    }

    // Vérifier si on a un clic dans une des cases
    if (event->type == SDL_MOUSEBUTTONDOWN) {
        int mouseX = event->button.x;
        int mouseY = event->button.y;

        // Parcourir les cases pour voir si le clic est dedans
        for (int i = 0; i < NB_ELEMENTS; i++) {
            if (mouseX >= cases[i].x && mouseX < (cases[i].x + cases[i].w) &&
                mouseY >= cases[i].y && mouseY < (cases[i].y + cases[i].h))
            {
                *selectionIndex = i;  // On met à jour l'indice sélectionné
                return i;            // Et on renvoie l'index
            }
        }
    }

    return -1;
}

void sauvegarderCarte(char** carte, int largeur, int hauteur) {
    FILE* fichier = fopen("niveau_cree.txt", "w");
    if (!fichier) {
        printf("Erreur de sauvegarde du niveau.\n");
        return;
    }

    for (int y = 0; y < hauteur; y++) {
        for (int x = 0; x < largeur; x++) {
            fputc(carte[y][x], fichier);
        }
        fputc('\n', fichier);
    }

    fclose(fichier);
    printf("Niveau sauvegardé sous 'niveau_cree.txt'.\n");
}


void sauvegarderNiveau(const char* nom, char** carte, int largeur, int hauteur) {
    char chemin[100];
    snprintf(chemin, sizeof(chemin), "niveaux/%s.txt", nom);
    FILE* fichier = fopen(chemin, "w");
    if (!fichier) {
        printf("Erreur de sauvegarde\n");
        return;
    }
    for (int y = 0; y < hauteur; y++) {
        fwrite(carte[y], sizeof(char), largeur, fichier);
        fputc('\n', fichier);
    }
    fclose(fichier);
    printf("Niveau %s sauvegardé !\n", nom);
}

void editeurDeNiveau(SDL_Renderer* renderer, TTF_Font* font, EtatJeu *etatActuel) {
    int enCours = 1;
    SDL_Event event;

    char nomNiveau[50] = "";
    int longueurNiveau = 30;
    int hauteurNiveau = 15;
    int offsetX = 0;
    int entreeActive = 1;
    int scrollObjet = 0;

    SDL_StartTextInput();

    while (entreeActive) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) return;
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_RETURN) {
                    entreeActive = 0;
                } else if (event.key.keysym.sym == SDLK_BACKSPACE && strlen(nomNiveau) > 0) {
                    nomNiveau[strlen(nomNiveau) - 1] = '\0';
                } else if (event.key.keysym.sym == SDLK_LEFT) {
                    if (longueurNiveau > 10) longueurNiveau -= 5;
                } else if (event.key.keysym.sym == SDLK_RIGHT) {
                    if (longueurNiveau < 500) longueurNiveau += 5;
                }
            } else if (event.type == SDL_TEXTINPUT) {
                if (strlen(nomNiveau) < 49) strcat(nomNiveau, event.text.text);
            }
        }

        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        afficherTexte(renderer, font, "Nom du niveau :", 250, 200, (SDL_Color){255, 255, 255, 255});
        afficherTexte(renderer, font, nomNiveau, 450, 200, (SDL_Color){255, 255, 0, 255});

        afficherTexte(renderer, font, "Taille (gauche/droite) :", 250, 250, (SDL_Color){255, 255, 255, 255});
        char tailleStr[10];
        sprintf(tailleStr, "%d", longueurNiveau);
        afficherTexte(renderer, font, tailleStr, 500, 250, (SDL_Color){255, 255, 0, 255});

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
    SDL_StopTextInput();

    // Création d'une carte vide
    char** carte = malloc(hauteurNiveau * sizeof(char*));
    for (int i = 0; i < hauteurNiveau; i++) {
        carte[i] = malloc(longueurNiveau * sizeof(char));
        // Initialiser toute la ligne à '0'
        memset(carte[i], '0', longueurNiveau);
    }

    // Vérifie qu'on a au moins 2 lignes
    if (hauteurNiveau >= 2) {
        int ligneDerniere = hauteurNiveau - 1;
        int ligneAvantDerniere = hauteurNiveau - 2;

        // Mettre la dernière ligne en 'D'
        for (int j = 0; j < longueurNiveau; j++) {
            carte[ligneDerniere][j] = 'D';
        }

        // Mettre l'avant-dernière ligne en '9'
        for (int j = 0; j < longueurNiveau; j++) {
            carte[ligneAvantDerniere][j] = '9';
        }
    }



    // Éléments sélectionnables avec leurs textures correctes
    char elements[] = {'0','1', '2', 'D', '9', '4', '5', '6', 'E', 'C', 'P', 'L','M'};
    SDL_Texture* textures[] = {
            texturevie,            // Vide
         textureBlocSolide,        // Bloc solide
        textureMort,              // Lave
        texturedirt,              // Terre
        texturegrass,             // Herbe
        texturePic,               // Pics
        textureAnneau,            // Pièce
        textureCheckpoint,        // Checkpoint
        textureEnnemi,            // Ennemi normal (animé)
        textureenemiecarapace,    // Ennemi carapace (animé)
        textureplant,             // Plante Piranha (animée)
        texturelucky,    // Bloc générique
        texturemascas
    };

    int nbElements = sizeof(elements) / sizeof(elements[0]);

    for (int i = 0; i < 5 && (i + scrollObjet) < nbElements; i++) {
        SDL_Rect dest = {i * 50, HAUTEUR_ECRAN - 45, 40, 40};
        SDL_Rect source = {0, 0, 16, 16}; // Prend toujours les 16x16 premiers pixels

        if (textures[i + scrollObjet] != NULL) {
            SDL_RenderCopy(renderer, textures[i + scrollObjet], &source, &dest);
        } else {
            printf("Erreur : Texture %c non chargée !\n", elements[i + scrollObjet]);
        }
    }


    SDL_Rect btnSauvegarde = {LARGEUR_ECRAN - 150, HAUTEUR_ECRAN - 50, 140, 40};

    int selectionIndex = 0;
    while (enCours) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) enCours = 0;
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) enCours = 0;
                if (event.key.keysym.sym == SDLK_LEFT) offsetX -= 40;
                if (event.key.keysym.sym == SDLK_RIGHT) offsetX += 40;
                if (event.key.keysym.sym == SDLK_a && scrollObjet > 0) scrollObjet--;
                if (event.key.keysym.sym == SDLK_z && scrollObjet < nbElements - 5) scrollObjet++;

                if (offsetX < 0) offsetX = 0;
                if (offsetX > (longueurNiveau - 20) * TAILLE_TILE) offsetX = (longueurNiveau - 20) * TAILLE_TILE;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x = (event.button.x + offsetX) / TAILLE_TILE;
                int y = event.button.y / TAILLE_TILE;

                // Vérifier si on clique sur un bloc du panel de sélection
                if (event.button.y >= HAUTEUR_ECRAN - 50) {
                    int choix = (event.button.x / 50) + scrollObjet;
                    if (choix < nbElements) selectionIndex = choix;
                }
                // Sinon, placer l'élément sélectionné sur la carte
                else if (x < longueurNiveau && y < hauteurNiveau) {
                    carte[y][x] = elements[selectionIndex];
                }

                // Vérifier si on clique sur le bouton de sauvegarde
                if (event.button.x >= btnSauvegarde.x && event.button.x <= btnSauvegarde.x + btnSauvegarde.w &&
                    event.button.y >= btnSauvegarde.y && event.button.y <= btnSauvegarde.y + btnSauvegarde.h) {
                    sauvegarderNiveau(nomNiveau, carte, longueurNiveau, hauteurNiveau);
                    *etatActuel = MENU;
                    enCours = 0;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);
        SDL_RenderClear(renderer);

        afficherCarte(renderer, carte, longueurNiveau, hauteurNiveau, offsetX, 0);

        // Affichage du panel de sélection
        SDL_Rect panel = {0, HAUTEUR_ECRAN - 50, LARGEUR_ECRAN, 50};
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_RenderFillRect(renderer, &panel);

        for (int i = 0; i < 7 && (i + scrollObjet) < nbElements; i++) {
            SDL_Rect dest = {i * 50, HAUTEUR_ECRAN - 45, 40, 40};
            SDL_Rect source = {0, 0, 16, 16}; // Prendre les 16x16 premiers pixels

            if (textures[i + scrollObjet] != NULL) { // Vérifier que la texture existe
                SDL_RenderCopy(renderer, textures[i + scrollObjet], &source, &dest);
            } else {
                printf("Erreur : Texture %c non chargée !\n", elements[i + scrollObjet]);
            }
        }

        SDL_Rect cadre = {(selectionIndex - scrollObjet) * 50, HAUTEUR_ECRAN - 45, 40, 40};
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_RenderDrawRect(renderer, &cadre);

        // bouton de save
        SDL_Rect btnSauvegardeRect = {LARGEUR_ECRAN - 150, HAUTEUR_ECRAN - 50, 140, 40};
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &btnSauvegardeRect);
        afficherTexte(renderer, font, "Sauvegarder", btnSauvegardeRect.x + 10, btnSauvegardeRect.y + 10,
                      (SDL_Color){0, 0, 0, 255});

        // maj de l'écran
        SDL_RenderPresent(renderer);
        SDL_Delay(16);


        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    for (int i = 0; i < hauteurNiveau; i++) {
        free(carte[i]);
    }
    free(carte);
}