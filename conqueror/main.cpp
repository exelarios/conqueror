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

#include "LTimer.h"

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
TTF_Font* font300 = nullptr;
TTF_Font* font100 = nullptr;
TTF_Font* font50 = nullptr;

SDL_Surface* textSurface = nullptr;
SDL_Texture* text = nullptr;

SDL_Rect startRect;

SDL_Color textColor = { 255, 255, 255, 255};

SDL_Rect TextBlock;
SDL_Rect textButton;

SDL_Rect block;

//Initalize Timer
auto gStartTime = chrono::steady_clock::now();
bool started = false;
bool pause = true;

int countdown = 20; // In Seconds
Uint32 startTime = (countdown*1000) + 3500;
std::stringstream timeText;

//framerates



int countedFrames = 0;

//Game Values
int score = 0;
int rounds = 1;
int lives = 20;
int blockSpeed = 10;
int gtimer = 50;
int timeout = 0;

bool success = true;

//Screens
bool isPlaying = false;
bool inMenu = true;

SDL_Surface* renderText = nullptr;
SDL_Texture* textureText = nullptr;


LTimer::LTimer()
{
    //Initialize the variables
    mStartTicks = 0;
    mPausedTicks = 0;
    
    mPaused = false;
    mStarted = false;
}

void LTimer::start()
{
    //Start the timer
    mStarted = true;
    
    //Unpause the timer
    mPaused = false;
    
    //Get the current clock time
    mStartTicks = SDL_GetTicks();
    mPausedTicks = 0;
}

void LTimer::stop()
{
    //Stop the timer
    mStarted = false;
    
    //Unpause the timer
    mPaused = false;
    
    //Clear tick variables
    mStartTicks = 0;
    mPausedTicks = 0;
}

void LTimer::pause()
{
    //If the timer is running and isn't already paused
    if( mStarted && !mPaused )
    {
        //Pause the timer
        mPaused = true;
        
        //Calculate the paused ticks
        mPausedTicks = SDL_GetTicks() - mStartTicks;
        mStartTicks = 0;
    }
}

void LTimer::unpause()
{
    //If the timer is running and paused
    if( mStarted && mPaused )
    {
        //Unpause the timer
        mPaused = false;
        
        //Reset the starting ticks
        mStartTicks = SDL_GetTicks() - mPausedTicks;
        
        //Reset the paused ticks
        mPausedTicks = 0;
    }
}

Uint32 LTimer::getTicks()
{
    //The actual timer time
    Uint32 time = 0;
    
    //If the timer is running
    if( mStarted )
    {
        //If the timer is paused
        if( mPaused )
        {
            //Return the number of ticks when the timer was paused
            time = mPausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            time = SDL_GetTicks() - mStartTicks;
        }
    }
    
    return time;
}

bool LTimer::isStarted()
{
    //Timer is running and paused or unpaused
    return mStarted;
}

bool LTimer::isPaused()
{
    //Timer is running and paused
    return mPaused && mStarted;
}


LTimer timer;

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
    
    timeText.str( "" );
    timeText << (timer.getTicks() / 1000 );
    
    //timeText <<  (startTime - SDL_GetTicks()) / 1000;
    
    isPlaying = true;
    
    SDL_SetRenderDrawColor(gRenderer, 200, 0, 255, 255);
    SDL_RenderFillRect(gRenderer, &block);
    
    printText("Score: " + to_string(score), font50, TextBlock, 100, 50);
    
    printText("Lives: " + to_string(lives), font50, TextBlock, 100, 120);
    
    printText("Countdown: " + timeText.str(), font50, TextBlock, 2500, 50);
    
    
    return success;
}

bool startMenu() {
    
    printText("CONQUEROR ", font300, TextBlock, WIDTH/2 , HEIGHT/2);
    printText("game by deric kwok", font100, TextBlock, (WIDTH/2) + 360, (HEIGHT/2) + 300);
    
    //printText("START", font100, textButton, 300 , 1300);
    textSurface = TTF_RenderText_Blended(font100, "START", textColor);
    if (!textSurface){
        cout << "Failed to load" << " " << font100 << " " << "font! SDL_ttf Error: %s\n" << TTF_GetError();
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
    
    font300 = TTF_OpenFont("tomhand.ttf", 300);
    if (font300 == NULL){
        cout << "Failed to load" << " " << "tomhand.ttf" << " " << "font! SDL_ttf Error: %s\n" << TTF_GetError();
        success = false;
    }
    
    font100 = TTF_OpenFont("tomhand.ttf", 100);
    if (font100 == NULL){
        cout << "Failed to load" << " " << "tomhand.ttf" << " " << "font! SDL_ttf Error: %s\n" << TTF_GetError();
        success = false;
    }
    
    font50 = TTF_OpenFont("tomhand.ttf", 50);
    if (font50 == NULL){
        cout << "Failed to load" << " " << "tomhand.ttf" << " " << "font! SDL_ttf Error: %s\n" << TTF_GetError();
        success = false;
    }
     
    
    return success;
}

int main(){
    
    
    startRect.x = 300;
    startRect.y = 1300;
    
    SDL_Event windowEvent;
    
    if(!init()){
        printf( "Failed to initialize!\n" );
    }
    if ( !loadMedia()) {
        printf( "Failed to load media!\n" );
    }
    
    int xMouse = 0, yMouse = 0;
    
    
    
    while(success){
        
        if (!SDL_TICKS_PASSED(SDL_GetTicks(), timeout)){
            cout << "dank memes\n";
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
                    if( ( xMouse > startRect.x /2 ) && ( xMouse < startRect.x ) && ( yMouse <= 700 ) && (yMouse >= 642)){
                        isPlaying = true;
                        cout << "Player is pressed Start!\n";
                    }
                }
            } else if( windowEvent.type == SDL_KEYDOWN ) {
                //Start/stop
                if( windowEvent.key.keysym.sym == SDLK_s )
                {
                    if( timer.isStarted() )
                    {
                        timer.stop();
                    }
                    else
                    {
                        timer.start();
                    }
                }
                //Pause/unpause
                else if( windowEvent.key.keysym.sym == SDLK_p )
                {
                    if( timer.isPaused() )
                    {
                        timer.unpause();
                    }
                    else
                    {
                        timer.pause();
                    }
                }
            }
            /*
            if (windowEvent.type == SDL_KEYDOWN){
                pause = false;
            }
             */
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
    
    block.w = 200;
    block.h = 200;
    block.x = (WIDTH / 2) - (block.w / 2);
    block.y = (HEIGHT / 2) - (block.h / 2);
    
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
    
    SDL_DestroyTexture(textureText);
    textureText = NULL;
    
    SDL_DestroyTexture(text);
    text = NULL;
    
    
    TTF_CloseFont( font100 );
    font100 = NULL;
    
    TTF_CloseFont( font300 );
    font300 = NULL;
    
    TTF_CloseFont( font50 );
    font50 = NULL;
    
    SDL_RenderClear(gRenderer);
    SDL_DestroyWindow(gWindow);
    
    gWindow = NULL;
    gRenderer = NULL;
    
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
