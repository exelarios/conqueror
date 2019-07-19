#include <iostream>
#include "SDL2/SDL.h"
#include <SDL2_image/SDL_image.h>
#include <stdio.h>
#include <string>
#include <SDL2_ttf/SDL_ttf.h>
#include <fstream>

using namespace std;

const int WIDTH = 1500, HEIGHT = 1000;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
TTF_Font* gFont = NULL;

void gameMenu(){
    
}

class LTexture {
    
public:
    LTexture();
    
    ~LTexture();
    
    bool loadFromTitle( std::string path);
    
    bool loadFromRenderedText (std::string textureText, SDL_Color textColor);
    
    void free();
    
    void setColor (Uint8 red, Uint8 green, Uint8 blue);
    
    void setBlendMode (SDL_BlendMode blending);
    
    void setAlpha (Uint8 alpha);
    
    void render (int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
    
    int getWidth();
    int getHeight();

private:
    
    SDL_Texture* mTexture;
    int mWidth;
    int mHeight;
    
};

bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor){
    free();
    
    SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor);
    
    if( textSurface == NULL){
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    } else{
        
        mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
        if (mTexture == NULL){
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        } else{
            mWidth = textSurface->w;
            mHeight = textSurface->h;
        }
        
        SDL_FreeSurface( textSurface );
    }
    return mTexture != NULL;
}

int main(){
    
    SDL_Surface* imageSurface = NULL;
    SDL_Surface* windowSurface = NULL;
    LTexture gTextTexture;

    
    
    
    // Initalzing Window
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0 ) {
        cout << "SDL Couldn't be initialized. SDL Error!" << SDL_GetError() << endl;
    }
    IMG_Init(IMG_INIT_PNG);
    
    SDL_Window* window = SDL_CreateWindow("Conqueror", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
    
    
    windowSurface = SDL_GetWindowSurface(window);
    
    if (NULL == window){
        cout << "Couldn't render window." << SDL_GetError() << endl;
        return EXIT_FAILURE;
    }
    
    //intialzing PNG Loading
    SDL_Rect imagePosition;
    imageSurface = IMG_Load("mainLogo.png");
    
    imagePosition.x = 250;
    imagePosition.y = 100;
    
    
    if ( NULL == imageSurface){
        cout << "SDL couldn't load image! SDL Error: " << SDL_GetError() << endl;
    }
    
    //Initalzing TTF_Font
    int imgFlags = IMG_INIT_PNG;
    if (!( IMG_Init(imgFlags) * imgFlags)){
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
        return EXIT_FAILURE;
    }
    
    if( TTF_Init() == -1){
        printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
        return EXIT_FAILURE;
    }
    
    SDL_Event windowEvent;
    
    
    // Running the Window; Needs to loop in order for the window to be opened.
    while(true){
        if (SDL_PollEvent(&windowEvent)){
            if (SDL_QUIT == windowEvent.type){
                break;
            }
        }
        
        SDL_BlitSurface( imageSurface, NULL, windowSurface, &imagePosition);
        
        SDL_UpdateWindowSurface(window);
    }
    
    SDL_FreeSurface(imageSurface);
    SDL_FreeSurface(windowSurface);
    
    imageSurface = NULL;
    windowSurface = NULL;
    
    IMG_Quit();
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return EXIT_SUCCESS;
}
