#include <iostream>
#include "SDL.h"
#include "classes.h"
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>

int main(int argc, char* argv[])
{
    Game* g_game = 0;

    g_game = new Game();

    g_game->initSDL();

    g_game->background = g_game->loadTexture("Textures/background.png");

    g_game->bullet_texture = g_game->loadTexture("Textures/bullet.png");

    g_game->setBackgroundCoordinates();

    g_game->camera.w = 640;

    g_game->camera.h = 480;

    playerCharacter mainCharacter; 

    mainCharacter.setSize(PLAYER_WIDTH, PLAYER_HEIGHT);

    mainCharacter.image[0] = g_game->loadTexture("Textures/man1_v2.png"); 

    mainCharacter.image[1] = g_game->loadTexture("Textures/man2_v2.png"); 

    mainCharacter.image[2] = g_game->loadTexture("Textures/man_side0.png"); 

    mainCharacter.image[3] = g_game->loadTexture("Textures/man_side1.png"); 

    mainCharacter.image[4] = g_game->loadTexture("Textures/man1.png"); 

    mainCharacter.image[5] = g_game->loadTexture("Textures/player_shot1.png"); 

    mainCharacter.image[6] = g_game->loadTexture("Textures/player_shot2.png"); 

    mainCharacter.image[7] = g_game->loadTexture("Textures/player_shot3.png"); 

    mainCharacter.image[8] = g_game->loadTexture("Textures/player_shot4.png"); 

    mainCharacter.image[9] = g_game->loadTexture("Textures/player_shot5.png"); 

    mainCharacter.image[10] = g_game->loadTexture("Textures/player_shot6.png"); 

    mainCharacter.image[11] = g_game->loadTexture("Textures/player_shot7.png"); 

    bulletClassManager bulletManager;

    const int fps = 60;

    int delayTime = 1000/fps;

    uint32_t frameStart, frameTime; 
    
    while(g_game->running())
    {
        frameStart = SDL_GetTicks();

        g_game->incrementTime();    

        if(g_game->whatsTheTime() % 500 == 0){

            g_game->incrementEnemies();

            enemyCharacter* new_enemy;

            new_enemy = new enemyCharacter();

            new_enemy->image = g_game->loadTexture("Textures/enemy.png"); 

            new_enemy->setSize(ENEMY_WIDTH, ENEMY_HEIGHT);

            g_game->characterVector.push_back(new_enemy);

        }
        
        g_game->handleEvents(mainCharacter);

        g_game->update(mainCharacter, bulletManager);

        mainCharacter.fireBullet(bulletManager);

        g_game->moveEnemies(mainCharacter, bulletManager);

        g_game->move(mainCharacter);

        g_game->positionCamera(mainCharacter); 

        g_game->doAnimation(mainCharacter);

        g_game->drawFigures(mainCharacter, bulletManager);

        SDL_RenderPresent(g_game->renderer);

        frameTime = SDL_GetTicks() - frameStart;

        if(frameTime < delayTime){

            SDL_Delay((int)delayTime - frameTime);

        }

    }
   
};