#include "headers/graphics.h"

Graphics* Graphics::sInstance = NULL;
bool Graphics::sInitalized = false;

Graphics* Graphics::Instance() {
    
    if (sInstance == NULL){
        sInstance = new Graphics();
    }
    return sInstance;
}

void Graphics::Release() {
    delete sInstance;
    sInstance = NULL;
    
    sInitalized = false;
}

bool Graphics::Initalized(){
    
    return sInitalized;
}

Graphics::Graphics(){
    backBuffer = NULL;
    
    sInitalized = init();
}

Graphics::~Graphics(){
    
    SDL_DestroyWindow(Window);
    Window = NULL;
    
    SDL_Quit();
    
}

bool Graphics::init(){
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("SDL Initalization Error: %s\n", SDL_GetError());
        return false;
    }
    
    Window = SDL_CreateWindow("Conqueror", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    
    if (Window == NULL){
        printf("Window Creation Error: %s\n", SDL_GetError());
        return false;
    }
    
    backBuffer = SDL_GetWindowSurface(Window);
    
    return true;
}

void Graphics::render() {
    
    SDL_UpdateWindowSurface(Window);
}
