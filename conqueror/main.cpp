#include <iostream>
#include "SDL2/SDL.h"
#include <SDL2_image/SDL_image.h>
#include <stdio.h>
#include <string>
#include <SDL2_ttf/SDL_ttf.h>
#include <fstream>

#include "headers/LTexture.h"

using namespace std;

const int WIDTH = 1500, HEIGHT = 1000;

bool init();
bool loadMedia();
void close();

SDL_Texture* loadTexture( std::string path );
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Texture* gTexture = NULL;
TTF_Font* gFont = NULL;
SDL_Surface* imageSurface = NULL;

bool init(){
    
    bool success = true;
    
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0 ) {
        cout << "SDL Couldn't be initialized. SDL Error!" << SDL_GetError() << endl;
    }
    gWindow = SDL_CreateWindow("Conqueror", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
    if (NULL == gWindow){
        cout << "Couldn't render window." << SDL_GetError() << endl;
        success = false;
    }
    
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    
    if (gRenderer == NULL){
        printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    
    //Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) )
    {
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
        success = false;
    }
    
    if ( NULL == imageSurface){
        cout << "SDL couldn't load image! SDL Error: " << SDL_GetError() << endl;
        success = false;
    }
    
    //Initalzing TTF_Font
    
    if( TTF_Init() == -1){
        printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    
    
    return success;
}

int main(){
    bool success = true;
    
    gFont = TTF_OpenFont("OpenSans-Regular.ttf", 28);
    if (gFont == NULL){
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    
    SDL_Color textColor = { 255, 255, 255 };
    
    SDL_Surface* SurfaceMessage = TTF_RenderText_Solid(gFont, "PLEASE WORK", textColor);
    if (!SurfaceMessage){
        cout << "Failed to load texture surface.\n";
    }
    SDL_Texture* Message = SDL_CreateTextureFromSurface(gRenderer, SurfaceMessage);
    if (!Message){
        cout << "Failed to load texture surface.\n";
    }
    SDL_DestroyTexture(Message);
    SDL_FreeSurface(SurfaceMessage);
    

    

    
    SDL_Rect Message_rect; //create a rect
    Message_rect.x = 0;  //controls the rect's x coordinate
    Message_rect.y = 0; // controls the rect's y coordinte
    Message_rect.w = 100; // controls the width of the rect
    Message_rect.h = 100; // controls the height of the rect
    
    SDL_Event windowEvent;
    
    
    // Running the Window; Needs to loop in order for the window to be opened.
    if(!init()){
        printf( "Failed to initialize!\n" );
    } else {
        if ( !loadMedia()) {
            printf( "Failed to load media!\n" );
        }
    }
    
    SDL_Rect block;
    block.w = 200;
    block.h = 200;
    block.x = (WIDTH / 2) - (block.w / 2);
    block.y = (HEIGHT / 2) - (block.h / 2);
    int blockSpeed = 10;
    
    while(true){
        
        if(SDL_PollEvent(&windowEvent)){
            switch (windowEvent.type){
                case SDL_QUIT: {
                    close();
                    break;
                }
                case SDL_KEYDOWN:
                    switch (windowEvent.key.keysym.sym){
                        case SDLK_w:{
                            block.y -= blockSpeed;
                            break;
                        }
                        case SDLK_a:{
                            block.x -= blockSpeed;
                            break;
                        }
                        case SDLK_s:{
                            block.y += blockSpeed;
                            break;
                        }
                        case SDLK_d:{
                            block.x += blockSpeed;
                            break;
                        }
                        case SDLK_ESCAPE:{
                            close();
                            break;
                        }
                    }
                default:
                    break;
            }
        }
        
        
        SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
        SDL_RenderClear(gRenderer);
        
        

        SDL_SetRenderDrawColor(gRenderer, 200, 0, 255, 255);
        SDL_RenderFillRect(gRenderer, &block);
        
        
        SDL_RenderCopy(gRenderer, Message, NULL, NULL);
        
        SDL_RenderPresent(gRenderer);
        //SDL_BlitSurface( imageSurface, NULL, gRenderer, &imagePosition);
        //SDL_UpdateWindowSurface(gWindow);
        //SDL_RenderCopy(gRenderer, Message, NULL, &Message_rect);
    }
    
    return success;
}

bool loadMedia(){
    
    bool success = true;
    
    SDL_Rect imagePosition;
    imageSurface = IMG_Load("mainLogo.png");
    
    imagePosition.x = 250;
    imagePosition.y = 100;

    return success;
}


void close(){
    
    SDL_DestroyTexture( gTexture);
    gTexture = NULL;
    
    
    SDL_FreeSurface(imageSurface);
    
    TTF_CloseFont( gFont );
    gFont = NULL;
    
    imageSurface = NULL;
    
    SDL_RenderClear(gRenderer);
    SDL_DestroyWindow(gWindow);
    
    gWindow = NULL;
    gRenderer = NULL;
    
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
