/*
 
 The objective of the game is to dodge the two lasers intersecting from both axes.
 If the player tocuhes the laser for each second the player loses a life.
 Each round the lasers will go faster and the player has the updates to buy perks to upgrade their block.
 The player's score is also their currency in the game to increase speed, strink their blocks and also buy back lives.
 
*/

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

using namespace std;

const int WIDTH = 1500, HEIGHT = 1000;

bool init();
bool loadMedia();
void close();

//SDL_Texture* loadTexture( std::string path );


SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
SDL_Texture* gTexture = nullptr;
// Images Assets
SDL_Surface* favicon = nullptr;

SDL_Surface* speedIcon = nullptr;
SDL_Rect speedRect;
SDL_Rect livesRect;
SDL_Rect shrinkRect;
SDL_Texture* speedTexture = nullptr;

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

//int countdown = 10; // In Seconds
//int startTime = (countdown*1000);
//std::stringstream timeText;
//int cooldownTimer = 0;
//std::stringstream cooldownTime;

int runTime = 0;

int myCountdown = 10 * 1000;
bool startCountdown = false;

//OutputFiles
ofstream outputFile;

//Game Values
int score = 0;
int lives = 20;
int gRound = 1;
int blockSpeed = 10;
bool debounce = true;
int cooldownHit = 0;

int roundWait = 0;

bool isRan = false;

bool success = true;

//Screens
bool isPlaying = false;
bool inMenu = true;
bool gameOver = false;
bool newRound = false;

bool stopMovement = false;

SDL_Surface* renderText = nullptr;
SDL_Texture* textureText = nullptr;


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

void createLaser(){
    
    laser.w = WIDTH;
    laser2.h = HEIGHT;
    laser.x = 1500;
    laser2.y = 1000;
    
    if (stopMovement == false){
        if (started == true){
            startCountdown = true;
            
            laser.x = runTime - laser.w;
            laser2.y = runTime - laser2.h;
            
            if (isRan == false){
                score += 1 + gRound;
                laser2.x = rand() % 1400;
                laser.y = rand() % 900;
                laser.h = (rand() % 51) + 50;
                laser2.w = (rand() % 51) + 50;
                cout << laser2.x  << " " << laser.y << endl;
                isRan = true;
            }
        }
    }
    
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
        isRan = false;
    }
    
    cooldownHit++;
    runTime++;
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

void printImage(std::string path, SDL_Surface* speedIcon, SDL_Rect speedRect, int xPos, int yPos){
    speedIcon = IMG_Load(path.c_str());
    if (speedIcon == NULL){
        cout << "SpeedIcon failed to intialize.\n";
        success = false;
    }
    
    speedRect.x = xPos;
    speedRect.y = yPos;
    speedRect.w = 100;
    speedRect.h = 100;
    
    speedTexture = SDL_CreateTextureFromSurface(gRenderer, speedIcon);
    if (!speedTexture){
        cout << "Can't load Speed Image texture.\n";
    }
    SDL_FreeSurface(speedIcon);
    
    SDL_QueryTexture(speedTexture, NULL, NULL, &speedRect.w, &speedRect.h);
    
    SDL_RenderCopy(gRenderer, speedTexture, NULL, &speedRect);
}

bool startGame() {
    
    isPlaying = true;

    
    SDL_SetRenderDrawColor(gRenderer, 200, 0, 255, 255);
    SDL_RenderFillRect(gRenderer, &block);
    
    SDL_SetRenderDrawColor(gRenderer, 100, 0, 255, 255);
    SDL_RenderFillRect(gRenderer, &laser);
    SDL_RenderFillRect(gRenderer, &laser2);
    
    // The higher the value of gRound the faster the lasers will go.
    for (int i = 0; i < gRound + 1; i++){
        createLaser();
    }

    
    printText("Score: " + to_string(score), smallFont, TextBlock, 100, 50);
    
    printText("Lives: " + to_string(lives), smallFont, TextBlock, 100, 100);
    
    printText("Countdown: " + to_string(myCountdown / 1000), smallFont, TextBlock, 1100, 50);
    
    printText("Round: " + to_string(gRound), smallFont, TextBlock, 1100, 100);
    
    if (startCountdown == true){
        myCountdown--;
    }

    return success;
}



bool startMenu() {

    printText("CONQUEROR ", bigFont, TextBlock, (WIDTH/2) - 410 , (HEIGHT/2) - 200);
    printText("game by deric kwok", midFont, TextBlock, (WIDTH/2) - 220, (HEIGHT/2) - 70);
    
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

bool gameOverScreen() {
    
    printText("GAME OVER!", bigFont, TextBlock, (WIDTH/2) - 390, (HEIGHT/2) - 200);
    printText("Press SPACEBAR to try again.", midFont, TextBlock, (WIDTH/2) - 300 , (HEIGHT/2));
    //printText("What's your name?", midFont, TextBlock, (WIDTH/2) - 170 , (HEIGHT/2));
    
    gameOver = true;
    
    return success;
}

bool UpdateRound(){
    
    printText("Round: " + to_string(gRound), bigFont, TextBlock, (WIDTH/2) - 300 , (HEIGHT/2) - 200);
    printText("Press SPACEBAR to continue.", midFont, TextBlock, (WIDTH/2) - 300 , (HEIGHT/2));
    
    SDL_SetRenderDrawColor(gRenderer, 200, 0, 255, 255);
    SDL_RenderFillRect(gRenderer, &speedRect);
    
    printImage("speed.png", speedIcon, speedRect, 100, 200);
    printImage("lives.png", speedIcon, livesRect, 100, 450);
    printImage("shrink.png", speedIcon, shrinkRect, 100, 700);
    
    return success;
}


bool init(){
    
    srand(static_cast<unsigned int>(time(0)));
    
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
    
    startRect.x = 100;
    startRect.y = 700;
    
    speedRect.x = 100;
    speedRect.y = 200;
    
    SDL_Event windowEvent;
    
    if(!init()){
        printf( "Failed to initialize!\n" );
    }
    if ( !loadMedia()) {
        printf( "Failed to load media!\n" );
    }
    
    int xMouse = 0, yMouse = 0;
    
    outputFile.open("scoreKeeping.txt", ios::app);
    if (outputFile.fail()){
        cout << "Output file failed to open.\n";
        success = false;
    }
    
    if (gameOver == true){
        outputFile << "The player's final score is: " << score << endl;
    }
    
    while(success){
        
        SDL_RenderSetLogicalSize(gRenderer, WIDTH, HEIGHT);
        
        if (myCountdown == 0){
            lives += 10;
            gRound++;
            newRound = true;
            myCountdown = (10000 * gRound);
            stopMovement = true;
            startCountdown = false;
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
                    if( isPlaying == false && ( xMouse > startRect.x ) && ( xMouse < startRect.x + startRect.w ) && ( yMouse > startRect.y ) && ( yMouse < startRect.y + startRect.h ) ){
                        isPlaying = true;
                        cout << "Player pressed Start!\n";
                    }
                    if( ( xMouse > speedRect.x ) && ( xMouse < speedRect.x + speedRect.w ) && ( yMouse > speedRect.y ) && ( yMouse < speedRect.y + speedRect.h ) ){
                        if (score >= 500){
                            blockSpeed += 5;
                            cout << "yeet";
                            score -= 500;
                        } else {
                            cout << "not enough\n";
                        }
                    }
                }
            }
            
            if ((block.x != (WIDTH / 2) - (block.w / 2)) || (block.y != (HEIGHT / 2) - (block.h / 2)) ){
                if (started == false){
                    started = true;
                }
            }
            // Checks for wall bounds.
            if (block.x <= 0){
                block.x = 0;
            }
            if (block.y <= 0){
                block.y = 0;
            }
            if (block.x >= WIDTH - block.w){
                block.x = WIDTH - block.w;
            }
            if (block.y >= HEIGHT - block.h){
                block.y = HEIGHT - block.h;
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
                            //startTime = SDL_GetTicks();
                            success = false;
                            break;
                        }
                        case SDLK_SPACE: {
                            newRound = false;
                            isPlaying = true;
                            startCountdown = true;
                            stopMovement = false;
                            
                            if (gameOver == true){
                                score = 0;
                                lives = 20;
                                gRound = 0;
                                block.w = 100;
                                block.h = 100;
                                myCountdown = 10;
                                isPlaying = true;
                            }
                            break;
                        }
                        case SDLK_1: {
                            if (newRound == true){
                                if (score >= 500){
                                    blockSpeed += 5;
                                    score -= 500;
                                }
                            }
                            break;
                        }
                        case SDLK_2: {
                            if (newRound == true){
                                if (score >= 100){
                                    lives += 10;
                                    score -= 100;
                                }
                            }
                            break;
                        }
                        case SDLK_3: {
                            if (newRound == true){
                                if (block.w >= 10 && score >= 200){
                                    block.w -= 5;
                                    block.h -= 5;
                                    score -= 200;
                                }
                            }
                            break;
                        }
                        case SDLK_c: {
                            lives = 99999;
                            score = 99999;
                            break;
                        }
                        case SDLK_k: {
                            lives = 1;
                            break;
                        }
                        case SDLK_t: {
                            myCountdown = 2;
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
            gameOver = false;
        }
        
        if (inMenu){
            startMenu();
        }
        
        if(newRound){
            UpdateRound();
        }
        
        if(lives == 0){
            gameOver = true;
            inMenu = false;
            isPlaying = false;
        }
        
        if (gameOver){
            gameOverScreen();
            stopMovement = false;
        }
        
        //startGame();
        
        SDL_RenderPresent(gRenderer);
        
        
        // Prevent Memeory Overload
        SDL_DestroyTexture(textureText);
        SDL_DestroyTexture(speedTexture);
        SDL_DestroyTexture(gTexture);
        SDL_DestroyTexture(text);
    }
    
    close();
    return success;
}

bool loadMedia(){
    
    block.w = 100;
    block.h = 100;
    block.x = (WIDTH / 2) - (block.w / 2);
    block.y = (HEIGHT / 2) - (block.h / 2);
    
    favicon = IMG_Load("icon.png");
    
    
    SDL_SetWindowIcon(gWindow, favicon);
    SDL_FreeSurface(favicon);
    
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
