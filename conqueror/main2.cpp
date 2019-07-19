#include <iostream>
#include "SDL2/SDL.h"
#include <SDL2_image/SDL_image.h>
#include <stdio.h>
#include <string>
#include <SDL2_ttf/SDL_ttf.h>
#include <fstream>

#include "LTexture.h"

using namespace std;

const int WIDTH = 1500, HEIGHT = 1000;

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

TTF_Font *gFont = NULL;


void close(SDL_Window* window ){
    IMG_Quit();
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(){
    
    SDL_Surface* gameLogo = NULL;
    SDL_Surface* windowSurface = NULL;
    
    // Initalzing Window
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0 ) {
        cout << "SDL Couldn't be initialized. SDL Error!" << SDL_GetError() << endl;
    }
    SDL_Window* window = SDL_CreateWindow("Conqueror", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
    
    SDL_Surface* favicon = NULL;
    
    IMG_Init(IMG_INIT_PNG);
    
    windowSurface = SDL_GetWindowSurface(window);
    
    if (NULL == window){
        cout << "Couldn't render window." << SDL_GetError() << endl;
        return EXIT_FAILURE;
    }
    
    //intialzing PNG Loading
    SDL_Rect imagePosition;
    gameLogo = IMG_Load("mainLogo.png");
    favicon = IMG_Load("icon.png");
    
    
    imagePosition.x = 250;
    imagePosition.y = 100;
    
    
    if ( NULL == gameLogo){
        cout << "SDL couldn't load image! SDL Error: " << SDL_GetError() << endl;
    }
    
    SDL_SetWindowIcon(window, favicon);
    
    SDL_Event windowEvent;
    
    
    // Running the Window; Needs to loop in order for the window to be opened.
    while(true){
        if (SDL_PollEvent(&windowEvent)){
            if (SDL_QUIT == windowEvent.type){
                break;
            }
        }
        
        SDL_BlitSurface( gameLogo, NULL, windowSurface, &imagePosition);
        
        SDL_UpdateWindowSurface(window);
    }
    
    SDL_FreeSurface(gameLogo);
    SDL_FreeSurface(windowSurface);
    
    gameLogo = NULL;
    windowSurface = NULL;
    
    close( window );
    
    return EXIT_SUCCESS;
}
