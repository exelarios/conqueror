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
SDL_Surface* windowSurface = NULL;
SDL_Color textColor = { 255, 255, 255 };

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
    
    return success;
}


SDL_Surface* SurfaceMessage = TTF_RenderText_Solid(gFont, "put your text here", textColor);
SDL_Texture* Message = SDL_CreateTextureFromSurface(gRenderer, SurfaceMessage);

SDL_Texture* loadTexture (std::string path){
    SDL_Texture* newTexture = NULL;
    
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if(loadedSurface == NULL){
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    } else{
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL){
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}

/*
bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
    //Get rid of preexisting texture
    free();
    
    //Render text surface
    SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
    if( textSurface == NULL )
    {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    }
    else
    {
        //Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
        if( mTexture == NULL )
        {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        }
        else
        {
            //Get image dimensions
            mWidth = textSurface->w;
            mHeight = textSurface->h;
        }
        
        //Get rid of old surface
        SDL_FreeSurface( textSurface );
    }
    
    //Return success
    return mTexture != NULL;
}
 
*/

bool loadMedia(){
    

    
    bool success = true;
    
    gFont = TTF_OpenFont("SFNS.ttf", 28);
    if (gFont == NULL){
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    

    
    return success;
}

void close(){
    
    SDL_DestroyTexture( gTexture);
    gTexture = NULL;
    
    
    SDL_FreeSurface(imageSurface);
    SDL_FreeSurface(windowSurface);
    
    TTF_CloseFont( gFont );
    gFont = NULL;
    
    imageSurface = NULL;
    windowSurface = NULL;
    
    SDL_RenderClear(gRenderer);
    SDL_DestroyWindow(gWindow);
    
    gWindow = NULL;
    gRenderer = NULL;
    
    TTF_Quit();
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
    
    if( TTF_Init() == -1){
        printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    
    SDL_Event windowEvent;
    
    SDL_Rect Message_rect; //create a rect
    Message_rect.x = 0;  //controls the rect's x coordinate
    Message_rect.y = 0; // controls the rect's y coordinte
    Message_rect.w = 100; // controls the width of the rect
    Message_rect.h = 100; // controls the height of the rect
    
    
    // Running the Window; Needs to loop in order for the window to be opened.
    if(!init()){
        printf( "Failed to initialize!\n" );
    } else {
        if ( !loadMedia()) {
            printf( "Failed to load media!\n" );
        }
    }
    
    while(true){
        if (SDL_PollEvent(&windowEvent)){
            if (SDL_QUIT == windowEvent.type){
                break;
            }
        }
        SDL_SetRenderDrawColor( gRenderer, 0, 0, 200, 255 );
        SDL_RenderClear(gRenderer);
        SDL_RenderPresent(gRenderer);
        
        SDL_BlitSurface( imageSurface, NULL, windowSurface, &imagePosition);
        //SDL_RenderCopy(gRenderer, Message, NULL, &Message_rect);
        
        SDL_UpdateWindowSurface(gWindow);
    }
    
    close();
    
    return 0;
}
