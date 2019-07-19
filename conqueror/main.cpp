#include <iostream>
#include "SDL2/SDL.h"
#include <SDL2_image/SDL_image.h>
#include <stdio.h>
#include <string>
#include <SDL2_ttf/SDL_ttf.h>
#include <fstream>

using namespace std;

const int WIDTH = 1500, HEIGHT = 1000;

bool init();
bool loadMedia();
void close();

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Surface* imageSurface = NULL;
SDL_Surface* windowSurface = NULL;
//LTexture gTextTexture;

bool init(){
    
    bool success = true;
    
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0 ) {
        cout << "SDL Couldn't be initialized. SDL Error!" << SDL_GetError() << endl;
    }
    gWindow = SDL_CreateWindow("Conqueror", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
    windowSurface = SDL_GetWindowSurface(gWindow);
    if (NULL == gWindow){
        cout << "Couldn't render window." << SDL_GetError() << endl;
        success = false;
    }
    
    return success;
}

bool loadMedia(){
    
    bool success = true;
    
    return success;
}

void close(){
    
    SDL_FreeSurface(imageSurface);
    SDL_FreeSurface(windowSurface);
    
    imageSurface = NULL;
    windowSurface = NULL;
    
    SDL_DestroyWindow(gWindow);
    IMG_Quit();
    SDL_Quit();
}


int main(){
    
    bool success = true;
    
    // Initalzing Window
    
    IMG_Init(IMG_INIT_PNG);
    
    
    //intialzing PNG Loading
    SDL_Rect imagePosition;
    imageSurface = IMG_Load("mainLogo.png");
    
    imagePosition.x = 250;
    imagePosition.y = 100;
    
    
    if ( NULL == imageSurface){
        cout << "SDL couldn't load image! SDL Error: " << SDL_GetError() << endl;
        success = false;
    }
    
    //Initalzing TTF_Font
    int imgFlags = IMG_INIT_PNG;
    if (!( IMG_Init(imgFlags) * imgFlags)){
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
        success = false;
    }
    
    if( TTF_Init() == -1){
        printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    
    SDL_Event windowEvent;
    
    
    // Running the Window; Needs to loop in order for the window to be opened.
    if(!init()){
        printf( "Failed to initialize!\n" );
    } else {
        if ( !loadMedia()) {
            printf( "Failed to load media!\n" );
        } else {
            SDL_BlitSurface( imageSurface, NULL, windowSurface, &imagePosition);
        }
    }
    while(true){
        if (SDL_PollEvent(&windowEvent)){
            if (SDL_QUIT == windowEvent.type){
                break;
            }
        }
        SDL_UpdateWindowSurface(gWindow);
    }
    
    close();
    
    return 0;
}
