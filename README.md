# MARIO-C
BIENVENUE MONSIEUR TRANCHO

Un jeu de plateforme inspiré de Mario, développé en C avec la bibliothèque SDL2.

\ Fonctionnalités /

Déplacements du joueur (gauche, droite, saut)

Système de collision avec le sol et les obstacles

Collecte d'anneaux

Ennemis avec différents comportements

Checkpoints pour sauvegarder la progression

Éditeur de niveaux intégré

Sélection de personnages

\ Dependances / 

Avant de compiler, assurez-vous d'avoir installé :

SDL2 : libsdl2-dev

SDL_2ttf : libsdl2-ttf-dev

SDL_2mixer : libsdl2-mixer-dev

\ Compilation & Execution / 

aller dans cmake-build-debug puis 

 gcc -o PROJETFINAL.exe ../main.c ../jouer.c ../texture.c ../affichage.c ../menu.c ../creationdeniveau.c ../deplacement.c ../afficheranimation.c ../audio.c -IC:/msys64/mingw64/include -IC:/msys64/mingw64/include/SDL2 -LC:/msys64/mingw64/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_mixer -mconsole

./PROJETFINAL




