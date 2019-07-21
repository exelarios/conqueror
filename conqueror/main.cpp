#include <iostream>
#include "SDL2/SDL.h"
#include <SDL2_image/SDL_image.h>
#include <stdio.h>
#include <string>
#include <SDL2_ttf/SDL_ttf.h>
#include <fstream>
#include <chrono>
#include <stdlib.h>

#include "headers/LTexture.h"

using namespace std;

const int WIDTH = 1500, HEIGHT = 1000;

bool init();
bool loadMedia();
void close();

SDL_Texture* loadTexture( std::string path );
SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
SDL_Texture* gTexture = nullptr;
SDL_Surface* favicon = NULL;
TTF_Font *gFont = nullptr;


//Initalize Font
SDL_Surface* textSurface = nullptr;
SDL_Texture* text = nullptr;
SDL_Rect textRect;
SDL_Color textColor = { 255, 255, 255, 255};

SDL_Texture *LoadTexture(std::string filePath, SDL_Renderer *renderTarget) {
    SDL_Texture *texture = nullptr;
    SDL_Surface *surface = IMG_Load(filePath.c_str());
    if(surface == NULL)
        std::cout << "Error" << std::endl;
    else
    {
        texture = SDL_CreateTextureFromSurface(renderTarget, surface);
        if(texture == NULL)
            std::cout << "Error" << std::endl;
    }
    
    SDL_FreeSurface(surface);
    
    return texture;
}

void printText(const std::string &Message, int xPos, int yPos){
    
    SDL_Surface* renderText = TTF_RenderText_Blended(gFont, Message.c_str(), textColor);
    if (!renderText){
        cout << "Failed to load CreateText surface.\n";
    }
    
    SDL_Texture* textureText = SDL_CreateTextureFromSurface(gRenderer, renderText);
    if (!text){
        cout << "Failed to load texture surface.\n";
    }
    
    SDL_Rect TextBlock;
    TextBlock.x = xPos;
    TextBlock.y = yPos;
    
    SDL_QueryTexture(textureText, NULL, NULL, &TextBlock.w, &TextBlock.h);
    SDL_FreeSurface(renderText);
    
    renderText = nullptr;
    
    SDL_RenderCopy(gRenderer, textureText, NULL, &TextBlock);
}

void printText(const int &Message, int xPos, int yPos){
    
    SDL_Surface* renderText = TTF_RenderText_Blended(gFont, to_string(Message).c_str(), textColor);
    if (!renderText){
        cout << "Failed to load CreateText surface.\n";
    }
    
    SDL_Texture* textureText = SDL_CreateTextureFromSurface(gRenderer, renderText);
    if (!text){
        cout << "Failed to load texture surface.\n";
    }
    
    SDL_Rect TextBlock;
    TextBlock.x = xPos;
    TextBlock.y = yPos;
    
    SDL_QueryTexture(textureText, NULL, NULL, &TextBlock.w, &TextBlock.h);
    SDL_FreeSurface(renderText);
    
    renderText = nullptr;
    
    SDL_RenderCopy(gRenderer, textureText, NULL, &TextBlock);
}

bool init(){
    
    bool success = true;
    
    SDL_Init(SDL_INIT_VIDEO);
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0 ) {
        cout << "SDL Couldn't be initialized. SDL Error!" << SDL_GetError() << endl;
    }
    
    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if( IMG_Init(imgFlags) != imgFlags ) {
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
        success = false;
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
    
    if( TTF_Init() < 0){
        printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    
    gFont = TTF_OpenFont("OpenSans-Regular.ttf", 100);
    if (gFont == NULL){
        printf( "Failed to load OpenSans font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    
    SDL_Color textColor = { 255, 255, 255, 255 };
    
    textSurface = TTF_RenderText_Blended(gFont, "TIMER:", textColor);
    if (!textSurface){
        cout << "Failed to load texture surface.\n";
    }
    
    text = SDL_CreateTextureFromSurface(gRenderer, textSurface);
    if (!text){
        cout << "Failed to load texture surface.\n";
    }
    
    textRect.x = textRect.y = 100;
    
    SDL_QueryTexture(text, NULL, NULL, &textRect.w, &textRect.h);
    
    SDL_FreeSurface(textSurface);
    textSurface = nullptr;
    
    
    return success;
}

int main(){
    bool success = true;

    SDL_Event windowEvent;
    
    // Running the Window; Needs to loop in order for the window to be opened.
    if(!init()){
        printf( "Failed to initialize!\n" );
    }
    if ( !loadMedia()) {
        printf( "Failed to load media!\n" );
    }
    
    SDL_Rect block;
    block.w = 100;
    block.h = 100;
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
        
        SDL_RenderCopy(gRenderer, text, NULL, &textRect);
        
        
        printText("Score", 500, 100);
        
        printText("Lives", 1000, 100);
        
        printText(500, 300, 500);
        
        SDL_RenderPresent(gRenderer);
    }
    
    return success;
}

bool loadMedia(){
    
    bool success = true;
    
    
    
    SDL_Rect imagePosition;
    favicon = IMG_Load("icon.png");
    
    SDL_SetWindowIcon(gWindow, favicon);
    SDL_FreeSurface(favicon);
    
    imagePosition.x = 250;
    imagePosition.y = 100;
    
    return success;
}


void close(){
    
    SDL_DestroyTexture( gTexture);
    SDL_DestroyTexture(text);
    gTexture = NULL;
    
    
    
    TTF_CloseFont( gFont );
    gFont = NULL;
    
    SDL_RenderClear(gRenderer);
    SDL_DestroyWindow(gWindow);
    
    gWindow = NULL;
    gRenderer = NULL;
    
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
