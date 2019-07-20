#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

bool run = true;

class Game
{
private:
    SDL_Window* window = NULL;
    SDL_Renderer* render = NULL;
    SDL_Rect snake;
    
public:
    Game()
    {
        window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, SDL_WINDOW_RESIZABLE);
        render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        snake.x = 400;
        snake.y = 400;
        snake.w = 50;
        snake.h = 50;
        SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
        SDL_RenderClear(render);
        SDL_RenderPresent(render);
    }
    
    void Event()
    {
        SDL_Event close;
        if(SDL_PollEvent(&close)){
            if(close.type == SDL_QUIT){
                run = false;
            }
        }
    }
    
    void InitSnake()
    {
        SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
        SDL_RenderFillRect(render, &snake);
        SDL_RenderClear(render);
        SDL_RenderPresent(render);
    }
};

int main( int argc, char *argv[] )
{
    SDL_Init(SDL_INIT_EVERYTHING);
    Game game;
    while(run)
    {
        game.Event();
        game.InitSnake();
    }
    return 0;
}
