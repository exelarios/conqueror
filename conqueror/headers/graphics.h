#ifndef graphics_h
#define graphics_h

#include <iostream>
#include "SDL2/SDL.h"
#include <SDL2_image/SDL_image.h>
#include <stdio.h>
#include <string>

class Graphics {
    
public:
    
    static const int SCREEN_WIDTH = 1500;
    static const int SCREEN_HEIGHT = 1000;

private:

    static Graphics * sInstance;
    static bool sInitalized;

    SDL_Window* Window;
    SDL_Surface* backBuffer;

public:
    static Graphics* Instance();
    static void Release();
    static bool Initalized();
    
    void render();

private:
    Graphics();
    ~Graphics();
    
    bool init();
};

#endif
