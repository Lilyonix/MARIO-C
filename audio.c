#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>

// Variables globales pour les sons préchargés
Mix_Chunk *coin   = NULL;
Mix_Chunk *jump   = NULL;
Mix_Chunk *mort   = NULL;
Mix_Chunk *casse  = NULL;
Mix_Chunk *life   = NULL;
Mix_Music *musique = NULL;  // Variable pour la musique

void audio_init(void) {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "Erreur d'initialisation de SDL (audio) : %s\n", SDL_GetError());
        return;
    }

    // Ouverture de l'appareil audio (44100 Hz, format par défaut, 2 canaux, buffer de 2048 octets)
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fprintf(stderr, "Erreur d'ouverture de l'appareil audio : %s\n", Mix_GetError());
        SDL_Quit();
        return;
    }

    // Chargement des effets sonores
    coin  = Mix_LoadWAV("audio/coin.wav");
    casse = Mix_LoadWAV("audio/break.wav");
    jump  = Mix_LoadWAV("audio/jump.wav");
    life  = Mix_LoadWAV("audio/life.wav");
    mort  = Mix_LoadWAV("audio/death.wav");

    // Chargement de la musique (modifiez le chemin et l'extension si nécessaire)
    musique = Mix_LoadMUS("audio/music.mp3");
    if (!musique) {
        fprintf(stderr, "Erreur lors du chargement de la musique : %s\n", Mix_GetError());
    }
}

void play_sound(Mix_Chunk *sound) {
    if (sound != NULL) {
        if (Mix_PlayChannel(-1, sound, 0) == -1) {
            fprintf(stderr, "Erreur lors de la lecture du son : %s\n", Mix_GetError());
        }
    } else {
        fprintf(stderr, "Le son demandé n'est pas chargé.\n");
    }
}

// Fonction pour jouer la musique en boucle
void play_music(void) {
    if (musique != NULL) {
        // -1 indique une boucle infinie
        if (Mix_PlayMusic(musique, -1) == -1) {
            fprintf(stderr, "Erreur lors de la lecture de la musique : %s\n", Mix_GetError());
        }
    } else {
        fprintf(stderr, "La musique n'est pas chargée.\n");
    }
}

void audio_cleanup(void) {
    if (coin) {
        Mix_FreeChunk(coin);
        coin = NULL;
    }
    if (life) {
        Mix_FreeChunk(life);
        life = NULL;
    }
    if (jump) {
        Mix_FreeChunk(jump);
        jump = NULL;
    }
    if (mort) {
        Mix_FreeChunk(mort);
        mort = NULL;
    }
    if (casse) {
        Mix_FreeChunk(casse);
        casse = NULL;
    }
    if (musique) {
        Mix_FreeMusic(musique);
        musique = NULL;
    }
    Mix_CloseAudio();
    SDL_Quit();
}
