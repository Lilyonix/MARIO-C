cmake_minimum_required(VERSION 3.29)
project(PROJETFINAL C)

set(CMAKE_C_STANDARD 11)

# Chemins des bibliothèques SDL2
include_directories(C:/msys64/mingw64/include)
include_directories(C:/msys64/mingw64/include/SDL2)
link_directories(C:/msys64/mingw64/lib)
include_directories(${CMAKE_SOURCE_DIR}/)


# Ajoutez tous les fichiers source
set(SOURCES
        main.c
        jouer.c
        texture.c
        affichage.c
        menu.c
        creationdeniveau.c
        deplacement.c
        afficheranimation.c
        audio.c
        audio.h
)

# Ajoutez l'exécutable
add_executable(PROJETFINAL ${SOURCES})
target_compile_definitions(PROJETFINAL PRIVATE SDL_MAIN_HANDLED)

# Lier les bibliothèques SDL2 et SDL2_mixer
target_link_libraries(PROJETFINAL SDL2 SDL2_ttf SDL2_mixer -mconsole)
