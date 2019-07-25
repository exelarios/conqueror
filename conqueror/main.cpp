#include <iostream>
#include "SDL2/SDL.h"
#include <SDL2_image/SDL_image.h>
#include <stdio.h>
#include <string>
#include <SDL2_ttf/SDL_ttf.h>
#include <fstream>
#include <stdlib.h>
#include <sstream>
#include <chrono>
#include <unistd.h>
#include <time.h>

#include "LTimer.cpp"

using namespace std;

const int WIDTH = 1500, HEIGHT = 1000;

bool init();
bool loadMedia();
void close();

//SDL_Texture* loadTexture( std::string path );


SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
SDL_Texture* gTexture = nullptr;
SDL_Surface* favicon = NULL;


//Initalize Font
TTF_Font* bigFont = nullptr;
TTF_Font* midFont = nullptr;
TTF_Font* smallFont = nullptr;

SDL_Surface* textSurface = nullptr;
SDL_Texture* text = nullptr;

SDL_Rect startRect;

SDL_Color textColor = { 255, 255, 255, 255};

SDL_Rect TextBlock;
SDL_Rect textButton;

SDL_Rect block;
SDL_Rect laser;
SDL_Rect laser2;

//Initalize Timer
auto gStartTime = chrono::steady_clock::now();
bool started = false;
bool gpause = true;

int countdown = 10; // In Seconds
int startTime = (countdown*1000);
std::stringstream timeText;
int cooldownTimer = 0;
std::stringstream cooldownTime;

int runTime = 0;
bool getRandom = false;
bool getRandom2 = false;

//framerates



int countedFrames = 0;

//Game Values
int score = 0;
int rounds = 1;
int lives = 20;
int blockSpeed = 10;
int gtimer = 50;
int timeout = 0;
bool debounce = true;
int cooldownHit = 0;

bool isRan = false;

bool success = true;

//Screens
bool isPlaying = false;
bool inMenu = true;

SDL_Surface* renderText = nullptr;
SDL_Texture* textureText = nullptr;

LTimer timer;
LTimer cooldownTiming;

/*

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
 
*/

bool checkCollision (SDL_Rect a, SDL_Rect b){
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;
    
    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;
    
    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;
    
    if( bottomA <= topB ) {
        return false;
    }
    
    if( topA >= bottomB ) {
        return false;
    }
    
    if( rightA <= leftB ) {
        return false;
    }
    
    if( leftA >= rightB ){
        return false;
    }
    
    return true;
}

void placeLaser(){
    do {
    } while ()
}

void updateLaserPosition() {
    
    if (laser.x >= WIDTH && laser2.y >= HEIGHT){
        placeLaser();
        isRan = true;
    }
}

void createLaser(){
    
    cout << "first laser" << endl;
    
    SDL_SetRenderDrawColor(gRenderer, 100, 0, 255, 255);
    SDL_RenderFillRect(gRenderer, &laser);
    SDL_RenderFillRect(gRenderer, &laser2);
    
    laser.w = WIDTH;
    laser.h = 50;
    laser.x = runTime - laser.w;
    
    laser2.h = HEIGHT;
    laser2.w = 50;
    laser2.y = runTime - laser2.h;
    
    laser.y = rand() % 1000;
    laser2.x = rand() % 1500;

    
    if (checkCollision(block, laser)){
        cout << "hit!" << endl;
        if (debounce == true) {
            if (cooldownHit >= 100){
                lives--;
                cooldownHit = 0;
            }
        }
    }
    
    if (checkCollision(block, laser2)){
        cout << "hit!2" << endl;
        if (debounce == true) {
            if (cooldownHit >= 100){
                lives--;
                cooldownHit = 0;
            }
        }
    }
    
    if (laser.x >= WIDTH){
        runTime = 0;
        isRan = true;
    }
    
    cooldownHit++;
    runTime++;
}

void createLaser2(){
    
    cout << "second laser" << endl;
    SDL_SetRenderDrawColor(gRenderer, 100, 0, 255, 255);
    SDL_RenderFillRect(gRenderer, &laser);
    SDL_RenderFillRect(gRenderer, &laser2);
    
    laser.w = WIDTH;
    laser.h = 50;
    laser.x = runTime - laser.w;
    
    laser2.h = HEIGHT;
    laser2.w = 50;
    laser2.y = runTime - laser2.h;
    
    
    if (checkCollision(block, laser)){
        cout << "hit!" << endl;
        if (debounce == true) {
            if (cooldownHit >= 100){
                lives--;
                cooldownHit = 0;
            }
        }
    }
    
    if (checkCollision(block, laser2)){
        cout << "hit!2" << endl;
        if (debounce == true) {
            if (cooldownHit >= 100){
                lives--;
                cooldownHit = 0;
            }
        }
    }
    
    if (laser.x >= WIDTH){
        runTime = 0;
        isRan = true;
    }
    
    cooldownHit++;
    runTime++;
    
    getRandom = true;

}


void printText(const std::string &Message, TTF_Font* fontType, SDL_Rect CreateRect, int xPos, int yPos){
    
    renderText = TTF_RenderText_Blended(fontType, Message.c_str(), textColor);
    if (!renderText){
        cout << "";
        cout << "Failed to load CreateText surface.\n";
    }
    
    textureText = SDL_CreateTextureFromSurface(gRenderer, renderText);
    if (!textureText){
        cout << "Failed to load texture surface. Big Yikes2\n";
    }
    
    CreateRect.x = xPos;
    CreateRect.y = yPos;
    
    SDL_QueryTexture(textureText, NULL, NULL, &CreateRect.w, &CreateRect.h);
    SDL_FreeSurface(renderText);
    
    renderText = nullptr;
    
    SDL_RenderCopy(gRenderer, textureText, NULL, &CreateRect);
    
    SDL_DestroyTexture(textureText);
}

bool startGame() {
    
    timeText.str("");
    cooldownTime.str("");
    
    cooldownTime << (cooldownTimer + cooldownTiming.getTicks() / 1000);
    
    //timeText << (timer.getTicks() / 1000 );
    
    timeText <<  (startTime - timer.getTicks()) / 1000;
    
    isPlaying = true;

    
    SDL_SetRenderDrawColor(gRenderer, 200, 0, 255, 255);
    SDL_RenderFillRect(gRenderer, &block);
    
    if (isRan == false){
        createLaser();
    } else {
        createLaser2();
    }

    
    printText("Score: " + to_string(score), smallFont, TextBlock, 100, 50);
    
    printText("Lives: " + to_string(lives), smallFont, TextBlock, 100, 100);
    
    printText("Countdown: " + timeText.str(), smallFont, TextBlock, 1100, 50);
    
    
    return success;
}

bool startMenu() {

    printText("CONQUEROR ", bigFont, TextBlock, (WIDTH/2) - 410 , (HEIGHT/2) - 200);
    printText("game by deric kwok", midFont, TextBlock, (WIDTH/2) - 220, (HEIGHT/2) - 70);
    
    //printText("START", font100, textButton, 300 , 1300);
    textSurface = TTF_RenderText_Blended(smallFont, "START", textColor);
    if (!textSurface){
        cout << "Failed to load" << " " << smallFont << " " << "font! SDL_ttf Error: %s\n" << TTF_GetError();
    }
    
    text = SDL_CreateTextureFromSurface(gRenderer, textSurface);
    if (!text){
        cout << "Failed to load texture surface.\n";
    }
    
    SDL_QueryTexture(text, NULL, NULL, &startRect.w, &startRect.h);
    
    SDL_FreeSurface(textSurface);
    textSurface = nullptr;
    
    SDL_RenderCopy(gRenderer, text, NULL, &startRect);
    
    return success;
}

bool init(){
    
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
    
    bigFont = TTF_OpenFont("tomhand.ttf", 150);
    if (bigFont == NULL){
        cout << "Failed to load" << " " << "tomhand.ttf" << " " << "font! SDL_ttf Error: %s\n" << TTF_GetError();
        success = false;
    }
    
    midFont = TTF_OpenFont("tomhand.ttf",50);
    if (midFont == NULL){
        cout << "Failed to load" << " " << "tomhand.ttf" << " " << "font! SDL_ttf Error: %s\n" << TTF_GetError();
        success = false;
    }
    
    smallFont = TTF_OpenFont("tomhand.ttf", 40);
    if (smallFont == NULL){
        cout << "Failed to load" << " " << "tomhand.ttf" << " " << "font! SDL_ttf Error: %s\n" << TTF_GetError();
        success = false;
    }
     
    
    return success;
}

int main(){
    
    startRect.x = 200;
    startRect.y = 700;
    
    SDL_Event windowEvent;
    
    if(!init()){
        printf( "Failed to initialize!\n" );
    }
    if ( !loadMedia()) {
        printf( "Failed to load media!\n" );
    }
    
    int xMouse = 0, yMouse = 0;
    
    while(success){
        
        SDL_RenderSetLogicalSize(gRenderer, WIDTH, HEIGHT);
        
        if (SDL_GetTicks() >= 13000){
            timer.pause();
        }
        
        if(SDL_PollEvent(&windowEvent)){
            if (windowEvent.type == SDL_QUIT){
                success = false;
            }
            if (windowEvent.type == SDL_MOUSEBUTTONDOWN){
                if (windowEvent.button.button == SDL_BUTTON_LEFT){
                    xMouse = windowEvent.button.x;
                    yMouse = windowEvent.button.y;
                    cout << "(" << xMouse << ", " << yMouse << ") "<< endl;
                    if( ( xMouse > startRect.x ) && ( xMouse < startRect.x + startRect.w ) && ( yMouse > startRect.y ) && ( yMouse < startRect.y + startRect.h ) ){
                        isPlaying = true;
                        cout << "Player pressed Start!\n";
                    }
                }
            }
            if ((block.x != (WIDTH / 2) - (block.w / 2)) || (block.y != (HEIGHT / 2) - (block.h / 2)) ){
                if (started == false){
                    timer.start();
                    cooldownTiming.start();
                    started = true;
                }
                
            }
            switch (windowEvent.type){
                case SDL_KEYDOWN:
                    switch (windowEvent.key.keysym.sym){
                        case SDLK_w: case SDLK_UP:{
                            block.y -= blockSpeed;
                            break;
                        }
                        case SDLK_a: case SDLK_LEFT:{
                            block.x -= blockSpeed;
                            break;
                        }
                        case SDLK_s: case SDLK_DOWN:{
                            block.y += blockSpeed;
                            break;
                        }
                        case SDLK_d: case SDLK_RIGHT:{
                            block.x += blockSpeed;
                            break;
                        }
                        case SDLK_ESCAPE:{
                            startTime = SDL_GetTicks();
                            cooldownTimer = SDL_GetTicks();
                            success = false;
                            break;
                        }
                    }
                default:
                    break;
            }
        }
        
        
        SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
        SDL_RenderClear(gRenderer);
        
        SDL_SetRenderDrawColor(gRenderer, 100, 0, 255, 255);
        
        if (isPlaying){
            startGame();
            delete textSurface;
            inMenu = false;
        }
        
        if (inMenu){
            startMenu();
        }
        
        //startGame();
        
        SDL_RenderPresent(gRenderer);
        
        
        // Prevent Memeory Overload
        SDL_DestroyTexture(textureText);
        SDL_DestroyTexture(gTexture);
        SDL_DestroyTexture(text);
    }
    
    auto endTime = chrono::steady_clock::now();
    double difference = chrono::duration_cast < chrono::seconds > (endTime - gStartTime).count();
    cout << "You took "<< difference << " seconds\n";
    close();
    return success;
}

bool loadMedia(){
    
    block.w = 100;
    block.h = 100;
    block.x = (WIDTH / 2) - (block.w / 2);
    block.y = (HEIGHT / 2) - (block.h / 2);
    
    SDL_Rect imagePosition;
    favicon = IMG_Load("icon.png");
    
    laser.y = rand() % 1000;
    laser2.x = rand() % 1500;
    
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
    
    SDL_DestroyTexture(textureText);
    textureText = NULL;
    
    SDL_DestroyTexture(text);
    text = NULL;
    
    
    TTF_CloseFont( bigFont );
    bigFont = NULL;
    
    TTF_CloseFont( midFont );
    midFont = NULL;
    
    TTF_CloseFont( smallFont );
    smallFont = NULL;
    
    SDL_RenderClear(gRenderer);
    SDL_DestroyWindow(gWindow);
    
    gWindow = NULL;
    gRenderer = NULL;
    
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
