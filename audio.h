#ifndef AUDIO_H
#define AUDIO_H

#include <SDL2/SDL_mixer.h>

extern Mix_Chunk *coin;
extern Mix_Chunk *jump;
extern Mix_Chunk *mort;
extern Mix_Chunk *casse;
extern Mix_Chunk *life;
extern Mix_Chunk *music;

void play_music(void);

void audio_init(void);
void play_sound(Mix_Chunk *sound);
void audio_cleanup(void);

#endif // AUDIO_H
