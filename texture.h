#ifndef TEXTURE_H
#define TEXTURE_H

#include "jouer.h"

extern SDL_Texture* textureDefaut;
extern SDL_Texture* textureBlocSolide;
extern SDL_Texture* texturePic;
extern SDL_Texture* textureAnneau;
extern SDL_Texture* textureCheckpoint;
extern SDL_Texture* textureMort;
extern SDL_Texture* textureFin;
extern SDL_Texture* textureEnnemi;
extern SDL_Texture* texturegrass;
extern SDL_Texture* texturedirt;
extern SDL_Texture* textureplateforme;
extern SDL_Texture* textureenemiecarapace;
extern SDL_Texture* texturevie;
extern SDL_Texture* texturepoteau;
extern SDL_Texture* texturefinpoteau;
extern SDL_Texture* texturechampignon;
extern SDL_Texture* texturemariohead;
extern SDL_Texture* texturejoueur;
extern SDL_Texture* textureplant;
extern SDL_Texture* texturefond;
extern SDL_Texture* texturelucky;
extern SDL_Texture* texturemascas;
extern SDL_Texture* texturecarapace;
extern SDL_Texture* textureboss;
extern SDL_Texture* texturejoueur1;
extern SDL_Texture* texturejoueur2;

// Ajout d'un tableau pour stocker les textures des personnages
extern SDL_Texture* texturesPersonnages[4];
extern const char* nomsPersonnages[4];
extern const char* fichiersPersonnages[4];



SDL_Texture* chargerTexture(SDL_Renderer* renderer, const char* chemin);
void chargerToutesLesTextures(SDL_Renderer* renderer);
void chargerTexturesPersonnages(SDL_Renderer* renderer);
void libererToutesLesTextures();

#endif //TEXTURE_H
