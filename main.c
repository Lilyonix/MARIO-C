#include "main.h"

int main(int argc, char* argv[]) {
    audio_init();
    play_music();

    // Initialisation de SDL et SDL_ttf
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        printf("Erreur SDL_Init: %s\n", SDL_GetError());
        return 1;
    }
    if (TTF_Init() == -1) {
        printf("Erreur TTF_Init: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    // Création de la fenêtre et du renderer
    SDL_Window* fenetre = SDL_CreateWindow(
        "Plateforme", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        LARGEUR_ECRAN, HAUTEUR_ECRAN, SDL_WINDOW_SHOWN
    );
    if (!fenetre) {
        printf("Erreur lors de la création de la fenêtre: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(fenetre, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Erreur : Impossible de créer le renderer (%s)\n", SDL_GetError());
        SDL_DestroyWindow(fenetre);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Chargement des textures et des polices
    chargerToutesLesTextures(renderer);
    chargerTexturesPersonnages(renderer); // Charge les textures des personnages

    TTF_Font* font = TTF_OpenFont("arial.ttf", 24);
    if (!font) {
        printf("Erreur : Impossible de charger la police Arial.ttf\n");
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(fenetre);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Texture* texturefond = chargerTexture(renderer, "textures/fond.bmp");
    if (!texturefond) {
        printf("Avertissement : texture de fond manquante.\n");
    }

    // Initialisation des variables de jeu
    EtatJeu etatActuel = MENU;
    Joueur joueur = {0, 0, 0, 0, 0, 3, 0, 0, 0};
    Joueur joueur2 = {0, 0, 0, 0, 0, 3, 0, 0, 0};
    joueur2.checkpointX = joueur2.x * TAILLE_TILE;
    joueur2.checkpointY = joueur2.y * TAILLE_TILE;
    char** carte = NULL;
    int largeur = 0, hauteur = 0;
    char* niveauSelectionne = NULL;

    while (etatActuel != QUITTER) {
        if (etatActuel == MENU) {
            niveauSelectionne = afficherMenu(renderer, font, &etatActuel);
        }

        if (etatActuel == JEU) {
            // Sélection du personnage AVANT de choisir le niveau
            int choixPersonnage = selectionnerPersonnage(renderer, font);
            if (choixPersonnage == -1) { // Si l'utilisateur quitte
                etatActuel = MENU;
                continue;
            }

            // Assignation directe de la texture du joueur
            texturejoueur = texturesPersonnages[choixPersonnage];
            if (!texturejoueur) {
                printf("Erreur : Texture du personnage non chargée !\n");
                etatActuel = MENU;
                continue;
            }

            // Sélection du niveau après le choix du personnage
            if (!niveauSelectionne) {
                niveauSelectionne = selectionnerNiveau(renderer, font);
            }
            if (niveauSelectionne) {
                carte = chargerCarte(niveauSelectionne, &largeur, &hauteur, &joueur);
                jouer(renderer, texturefond, &joueur, carte, largeur, hauteur, &etatActuel);

                // Si dans la fonction jouer le joueur perd toutes ses vies,
                // on suppose que la fonction affecte etatActuel = GAMEOVER.
                if (etatActuel == GAMEOVER) {
                    // Affichage de l'écran Game Over
                    afficherGameOverScreen(renderer);
                    // Après Game Over, retour au menu (ou autre logique de redémarrage)
                    etatActuel = MENU;
                }

                libererCarte(carte, hauteur);
                free(niveauSelectionne);
                niveauSelectionne = NULL;
                continue;
            }
        }

        if (etatActuel == CREER) {
            editeurDeNiveau(renderer, font, &etatActuel);
        }

        if (etatActuel == JOUERDEUX) {
            // Sélection du personnage AVANT de choisir le niveau
            int choixPersonnage1 = selectionnerPersonnage(renderer, font);
            if (choixPersonnage1 == -1) { // Si l'utilisateur quitte
                etatActuel = MENU;
                continue;
            }

            int choixPersonnage2 = selectionnerPersonnage(renderer, font);
            if (choixPersonnage2 == -1) { // Si l'utilisateur quitte
                etatActuel = MENU;
                continue;
            }

            // Assignation directe de la texture du joueur
            texturejoueur1 = texturesPersonnages[choixPersonnage1];
            texturejoueur2 = texturesPersonnages[choixPersonnage2];
            if (!texturejoueur1) {
                printf("Erreur : Texture du personnage non chargée !\n");
                etatActuel = MENU;
                continue;
            }

            // Sélection du niveau après le choix du personnage
            if (!niveauSelectionne) {
                niveauSelectionne = selectionnerNiveau(renderer, font);
            }
            if (niveauSelectionne) {
                carte = chargerCarte(niveauSelectionne, &largeur, &hauteur, &joueur);
                joueur2.checkpointX = joueur2.x * TAILLE_TILE;
                joueur2.checkpointY = joueur2.y * TAILLE_TILE;
                jouer2(renderer, texturefond, &joueur, &joueur2, carte, largeur, hauteur, &etatActuel);
                if (etatActuel == GAMEOVER) {
                    afficherGameOverScreen(renderer);
                    etatActuel = MENU;
                }
                if (etatActuel == GAGNER) {
                    etatActuel = MENU;
                }
                libererCarte(carte, hauteur);
                free(niveauSelectionne);
                niveauSelectionne = NULL;
            }
        }
    }

    // Libération des ressources
    SDL_DestroyTexture(texturefond);
    for (int i = 0; i < 4; i++) {
        SDL_DestroyTexture(texturesPersonnages[i]);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(fenetre);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();

    return 0;
}


