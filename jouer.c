#include "jouer.h"

int frameActuellePiece = 0;

void mettreAJourFramePiece() {
    static int compteur = 0; // Compteur pour gérer la vitesse de l'animation
    compteur++;
    if (compteur >= 10) { // Change de frame toutes les 10 itérations
        frameActuellePiece = (frameActuellePiece + 1) % 7; // 7 frames dans la sprite sheet
        compteur = 0;
    }
}


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


void jouer(SDL_Renderer* renderer, SDL_Texture* texturefond, Joueur* joueur, char** carte, int largeur, int hauteur, EtatJeu* etatActuel)

{

    TTF_Font* font = TTF_OpenFont("arial.ttf", 24);
    if (!font) {
        printf("Erreur : impossible de charger la police.\n");
        exit(1);
    }

    int enCours = 1; // permet de maintenir la boucle du jeu
    int offsetY = hauteur * TAILLE_TILE - HAUTEUR_ECRAN;

    // animations du joueur
    joueur->animMarche = creerAnimation(texturejoueur, 16, 32, 4, 10); // Animation de marche (4 frames)
    joueur->animSaut = creerAnimation(texturejoueur, 16, 32, 2, 10);   // Animation de saut (2 frames)
    joueur->direction = 1; // Le joueur regarde à droite par défaut
    joueur->gagner = 0;

    while (enCours) {
        mettreAJourFramePiece();
        if (joueur->vies <= 0) {
            *etatActuel = GAMEOVER;
            return;
        }
        else if (joueur->gagner == 1) {
            *etatActuel = MENU;
            return;
        }
        // Mettre à jour les animations des ennemis
        for (int i = 0; i < nbEnnemis; i++) {
            mettreAJourAnimation(&ennemis[i].anim);
        }

        // Mettre à jour l'animation du joueur en fonction de l'état
        if (joueur->vx != 0 && !joueur->estSautant) { // Si le joueur marche
            mettreAJourAnimation(&joueur->animMarche);
        }
        if (joueur->estSautant) { // Si le joueur saute
            mettreAJourAnimation(&joueur->animSaut);
        }

        // Gérer les événements (déplacement, saut, etc.)
        gererEvenements(joueur, &enCours);

        // Appliquer la gravité et les déplacements
        joueur->vy += GRAVITE; // Applique la gravité
        joueur->x += joueur->vx; // Déplacement horizontal
        joueur->y += joueur->vy; // Déplacement vertical

        // Gestion des collisions
        gererCollisionsAvecEnnemis(joueur);
        gererCollisions(joueur, carte, largeur, hauteur);

        // Déplacer les ennemis
        deplacerEnnemis();

        // Calculer l'offset pour centrer le joueur à l'écran
        int offsetX = joueur->x - LARGEUR_ECRAN / 2;
        if (offsetX < 0) offsetX = 0;

        // Effacer l'écran et afficher le fond
        SDL_RenderClear(renderer);
        if (texturefond) {
            SDL_RenderCopy(renderer, texturefond, NULL, NULL); // Affiche le fond en plein écran
        } else {
            printf("Avertissement : texture de fond manquante.\n");
        }

        // Afficher la carte, les ennemis et le joueur
        afficherCarte(renderer, carte, largeur, hauteur, offsetX, offsetY);
        afficherEnnemis(renderer, offsetX, offsetY);
        afficherJoueur(renderer, joueur, offsetX, offsetY); // Fonction pour gérer l'affichage du joueur
        afficherInterface(renderer, texturemariohead, textureAnneau, font, joueur);


        // Rafraîchir l'écran
        SDL_RenderPresent(renderer);

        // Attente pour limiter à 60 FPS
        SDL_Delay(16);
    }

    // Libérer la police utilisée pour l'interface
    TTF_CloseFont(font);
}

void jouer2(SDL_Renderer* renderer,
            SDL_Texture* texturefond,
            Joueur* joueur,
            Joueur* joueur2,
            char** carte,
            int largeur,
            int hauteur,
            EtatJeu* etatActuel)
{
    TTF_Font* font = TTF_OpenFont("arial.ttf", 24);
    if (!font) {
        printf("Erreur : impossible de charger la police.\n");
        exit(1);
    }

    int enCours = 1;
    int offsetY = hauteur * TAILLE_TILE - HAUTEUR_ECRAN;

    // animations Joueur 1
    joueur->animMarche = creerAnimation(texturejoueur1, 16, 32, 4, 10);
    joueur->animSaut   = creerAnimation(texturejoueur1, 16, 32, 2, 10);
    joueur->direction  = 1;

    // animations Joueur 2
    joueur2->animMarche = creerAnimation(texturejoueur2, 16, 32, 4, 10);
    joueur2->animSaut   = creerAnimation(texturejoueur2, 16, 32, 2, 10);
    joueur2->direction  = 1;

    while (enCours)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            // Appel de la fonction de contrôle de Joueur 1
            gererEvenementJoueur1(joueur, &e, &enCours);
            // Appel de la fonction de contrôle de Joueur 2
            gererEvenementJoueur2(joueur2, &e, &enCours);
        }

        // Vérifier si l'un meurt
        if (joueur->vies == 0 || joueur2->vies == 0) {
            *etatActuel = MENU;
            enCours = 0;
        }



        // Animer les ennemis, joueurs, etc
        mettreAJourFramePiece();
        for (int i = 0; i < nbEnnemis; i++) {
            mettreAJourAnimation(&ennemis[i].anim);
        }

        // Joueur 1 animations
        if (joueur->vx != 0 && !joueur->estSautant) {
            mettreAJourAnimation(&joueur->animMarche);
        }
        if (joueur->estSautant) {
            mettreAJourAnimation(&joueur->animSaut);
        }

        // Joueur 2 animations
        if (joueur2->vx != 0 && !joueur2->estSautant) {
            mettreAJourAnimation(&joueur2->animMarche);
        }
        if (joueur2->estSautant) {
            mettreAJourAnimation(&joueur2->animSaut);
        }

        // Appliquer la gravité et déplacements
        joueur->vy  += GRAVITE;
        joueur->x   += joueur->vx;
        joueur->y   += joueur->vy;

        joueur2->vy += GRAVITE;
        joueur2->x  += joueur2->vx;
        joueur2->y  += joueur2->vy;

        // Collisions
        gererCollisionsAvecEnnemis(joueur);
        gererCollisions(joueur, carte, largeur, hauteur);

        gererCollisionsAvecEnnemis(joueur2);
        gererCollisions2(joueur2,joueur, carte, largeur, hauteur);

        deplacerEnnemis(); // Mouvements ennemis

        // Calcul offset
        int offsetX = joueur->x - LARGEUR_ECRAN / 2;
        if (offsetX < 0) offsetX = 0;

        // Rendu
        SDL_RenderClear(renderer);

        // Fond
        if (texturefond) {
            SDL_RenderCopy(renderer, texturefond, NULL, NULL);
        }

        // Carte, ennemis
        afficherCarte(renderer, carte, largeur, hauteur, offsetX, offsetY);
        afficherEnnemis(renderer, offsetX, offsetY);

        // Joueurs
        afficherJoueur(renderer,  joueur,  offsetX, offsetY);
        afficherJoueur(renderer,  joueur2, offsetX, offsetY);

        // HUD
        afficherInterface(renderer, texturevie, textureAnneau, font, joueur);

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }

    TTF_CloseFont(font);
}