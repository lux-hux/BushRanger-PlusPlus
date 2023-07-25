#include "classes.h"
#include <iostream>
#include <cmath>


Game::Game()
{
    x_background = 0;

    y_background = 0;
}

Game::~Game()
{

    std::cout << "Game deconstructor" << std::endl;

}

void Game::moveEnemies(playerCharacter& player, bulletClassManager& bulletManager)
{

    float dirx;
    float diry; 
    float angle_x;
    float angle_y;
    float run_x;
    float run_y;
    float hyp;

    // || Move enemies and shoot at player

    for(std::vector<enemyCharacter*>::size_type i = 0; i !=characterVector.size(); i++)
    {

        if(characterVector[i]->alive == true)
        {

            characterVector[i]->reloading -= 1;

            dirx =  player.pos.x - characterVector[i]->pos.x;

            diry =  player.pos.y - characterVector[i]->pos.y;

            hyp = sqrt(dirx*dirx + diry*diry);

            run_x = dirx/hyp;

            run_y = diry/hyp;

            // If the enemy is not within distance of the player, move the enemy toward the player

            if(fabsf(dirx) >= 50 && fabsf(diry) >= 50){

                characterVector[i]->pos.x += run_x * 2;

                characterVector[i]->pos.y += run_y * 2;

            }

            if(characterVector[i]->reloading == 0){

                bulletClass* new_bullet;

                 new_bullet = new bulletClass(); 

                new_bullet->x = characterVector[i]->pos.x + (PLAYER_WIDTH/2);

                new_bullet->y = characterVector[i]->pos.y + (PLAYER_HEIGHT/2);

                new_bullet->dx = run_x; 

                new_bullet->dy = run_y;

                new_bullet->player_or_enemy = false;

                angle_x = characterVector[i]->pos.x - player.pos.x + PLAYER_WIDTH/2;

                angle_y = characterVector[i]->pos.y - player.pos.y + PLAYER_HEIGHT/2;

                bulletManager.bullet_angle =  (atan2f(angle_y,angle_x)) * (180/3.14);

                if(bulletManager.bullet_angle  >= 90){

                    bulletManager.bullet_angle  = bulletManager.bullet_angle - 90;

                 }else{

                    bulletManager.bullet_angle = bulletManager.bullet_angle  + 270;
                }
            
                bulletManager.bullet_angle = fmodf(bulletManager.bullet_angle  + 360, 360);

                new_bullet->angle = bulletManager.bullet_angle;

                bulletManager.bulletVector.push_back(new_bullet);

                characterVector[i]->reloading = 50;

            }

        }
    }
}

int Game::whatsTheTime()
{
    return gameTime;
}

void Game::incrementTime()
{
    gameTime++;
}


void Game::incrementEnemies()
{
    noEnemies ++;

}

void Game::setBackgroundCoordinates()
{

    // Sets the dimensions of the background texture to the x_background and y_background variables
    // this is used for camera re-centering when it leaves the bounds of the Background dimensions

    SDL_QueryTexture(background, NULL, NULL, &x_background, &y_background);

};


void Game::initSDL()
{

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Failed to initialise SDL: " << std::endl;
        std::cout << SDL_GetError() << std::endl; 
        exit(1);
    }

    window = SDL_CreateWindow("Game Window", \
    SDL_WINDOWPOS_UNDEFINED, \
    SDL_WINDOWPOS_UNDEFINED, \
    //the width of the window, in screen coordinates
    SCREEN_WIDTH, \
    //the height of the window, in screen coordinates 
    SCREEN_HEIGHT, \
    // 0, or one or more SDL_WindowFlags OR'd together
    // used for things like resizable window, full-screen etc.
    0);

    if(!window)
    {
        std::cout << "Failed to open window: " << std::endl;    

        exit(1);
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if(!renderer)
    {
        std::cout << "Failed to create renderer:" << std::endl;   

        exit(1);
    }

    m_bRunning = true;

}

void Game::handleEvents(playerCharacter& mainCharacter)
{
    SDL_Event ev;

    mainCharacter.face_left = false;
    mainCharacter.face_right = false;
    mainCharacter.moving = false;

    while(SDL_PollEvent(&ev))
    {
        switch(ev.type)
        {
            case SDL_QUIT:

                m_bRunning = false;

                break;

            case SDL_KEYDOWN:

                switch(ev.key.keysym.sym)
                {
                    case SDLK_UP:
                        mainCharacter.speedy = -1;
                        mainCharacter.moving = true;
                        break;
                    case SDLK_DOWN: 
                        mainCharacter.speedy = 1;
                        mainCharacter.moving = true;
                        break;
                    case SDLK_RIGHT:
                        mainCharacter.speedx = 1;
                        mainCharacter.face_right = true; 
                        mainCharacter.moving = true;
                    
                        break;
                    case SDLK_LEFT:
                        mainCharacter.speedx = -1;
                        mainCharacter.face_right = true;
                        mainCharacter.moving = true;
                        break;
                }

                break;
            
            case SDL_KEYUP:
               
                switch(ev.key.keysym.sym){
                    
                    case SDLK_UP:
                        mainCharacter.speedy = 0;
                        break;
                    case SDLK_DOWN: 
                        mainCharacter.speedy = 0;
                        break;
                    case SDLK_RIGHT:
                        mainCharacter.speedx = 0;
                        break;
                    case SDLK_LEFT:
                        mainCharacter.speedx = 0;
                        break;
                    case SDLK_SPACE:
                        mainCharacter.fire = 1;
                        break;
                }

                break;
        }
        
    }
}

void Game::prepareScene()
{

    SDL_RenderClear(renderer);

}

void Game::presentScene()
{
    SDL_RenderPresent(renderer);

}

SDL_Texture *Game::loadTexture(std::string fileName)
{

    SDL_Texture *texture;

    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", fileName.c_str());

    texture = IMG_LoadTexture(renderer, fileName.c_str());  

    if(texture ==NULL){
        printf("Cannot find texture %s \n", fileName.c_str());
         SDL_Quit();
         exit(1);
    }

    return texture;
}

playerCharacter::playerCharacter()
{

    pos.x = 30;

    pos.y = 1800;

    pos.w = 80;

    pos.h = 140;

    animFrame = 0;

    // I don't know why, but explicitly referring to SDL_Rect attributes in the
    // constructor leads to a seg fault (in case of 0'd values) or bus error (in case of != 0)
    // Has to be declared as follows, with {x, y, w, h}

    rect_player_source = {0, 0,80,160};

}

playerCharacter::~playerCharacter()
{
    if(image[0] != NULL)
    {

        for(unsigned int i = 0; i < sizeof(image); i++ ){

        SDL_DestroyTexture(image[i]);

        std::cout << i << std::endl;

        image[i] = NULL;

        }
    }

}

enemyCharacter::enemyCharacter()
{
    pos.x = 60;

    pos.y = 1500;

    image = NULL;

    pos.w = 80;

    pos.h = 140;

    rect_player_source = {0, 0,80,160};

}


bulletClass::~bulletClass()
{
}


bulletClass::bulletClass(){
}

bulletClassManager::bulletClassManager(){
}

bulletClassManager::~bulletClassManager(){

    std::cout << "Bullet class deconstructor activated" << std::endl;

    for(auto it = bulletVector.begin(); it != bulletVector.end();)
    {
            delete *it;

            it = bulletVector.erase(it);

    }

}

enemyCharacter::~enemyCharacter()
{

    std::cout << "enemyCharacter desconstructor" << std::endl;

    if(image != NULL)
    {
        SDL_DestroyTexture(image);

        image = NULL;
    }
}


void playerCharacter::setSize(int w, int h) 
{   
    pos.w = w;
    pos.h = h;
}

void playerCharacter::setPos(int x, int y) 
{
    pos.x = x;
    pos.y = y;
}


void enemyCharacter::setSize(int w, int h) 
{   
    pos.w = w;
    pos.h = h;
}

void enemyCharacter::setPos(int x, int y) 
{
    pos.x = x;
    pos.y = y;
}


void playerCharacter::move() 
{

    if(alive == DEAD){
        return; 
    }

    pos.x += speedx *4;
    pos.y += speedy *4;

    reloading -= 1; 

}


 void enemyCharacter::move()
 {

 }


void Game::positionCamera(playerCharacter& player)
{

        camera.y = player.pos.y - SCREEN_HEIGHT_HALVED; 

        if(camera.y < 0){
            camera.y = 0;
        }

        // For horizontal scrolling 

        // if(camera.x < 0){
        //     camera.x = 0;
        // }
        
        // if(camera.x + camera.w >= x_background)
        // {
        //     camera.x = x_background - SCREEN_WIDTH; 
        // }

        if(camera.y + camera.h >= y_background)
        {

            camera.y = y_background - SCREEN_HEIGHT; 

        }

}


void Game::doAnimation(playerCharacter& player){

    if(player.alive == DEAD)
    {

        if(gameTime % 10 == 0){
            switch(player.animFrame){
            case 4:
                player.animFrame = 5; 
                break;
            case 5:
                player.animFrame = 6; 
                break;
            case 6:
                player.animFrame = 7; 
                break;
            case 7:
                player.animFrame= 8; 
                break;
            case 8:
                player.animFrame = 9; 
                break;
            case 9:
                player.animFrame = 10; 
                break;
            case 10:
                break;
            default:
                player.animFrame = 5;
            }
        }
    } else if (player.moving == 1){

        std::cout << "player is moving" << player.animFrame << std::endl;

// Animate player when strafing left or right 

    if(player.face_left == true || player.face_right == true){
        if(gameTime % 10 == 0){
            if(player.animFrame == 2){
                player.animFrame = 3;
            }else if(player.animFrame == 3){
                player.animFrame = 2;
            }else{
                player.animFrame = 2;
            }
        }

    }else{

// |======= Animate player when walking forward or downward

        if(gameTime % 10 == 0){
            if(player.animFrame == 0){
                 player.animFrame = 1;
             }else if(player.animFrame == 1){
                player.animFrame = 0;
             }else{
                player.animFrame = 0;
             }
         }
    }

// |======= Return player to default state if not shot or moving 
}
else
{
    player.animFrame = 0;
}







}


void Game::drawFigures(playerCharacter& player, bulletClassManager& bulletManager)
{

    SDL_RenderCopy(renderer, background, &camera, NULL);

    player.rect_player_dest = {player.pos.x - camera.x, player.pos.y - camera.y, PLAYER_WIDTH , PLAYER_HEIGHT};

    SDL_RenderCopyEx(renderer, player.image[player.animFrame], &player.rect_player_source, &player.rect_player_dest, 0, NULL, SDL_FLIP_NONE);

    for(std::vector<enemyCharacter*>::size_type i = 0; i !=characterVector.size(); i++)
     {

        characterVector[i]->rect_player_dest = {characterVector[i]->pos.x - camera.x, characterVector[i]->pos.y - camera.y, ENEMY_WIDTH , ENEMY_HEIGHT};

        SDL_RenderCopyEx(renderer, characterVector[i]->image, &(characterVector[i]->rect_player_source), &(characterVector[i]->rect_player_dest), 0, NULL, SDL_FLIP_NONE);

     }

    for(std::vector<bulletClass*>::size_type i = 0; i !=bulletManager.bulletVector.size(); i++)
     {

        SDL_Rect bullet_enemy = {bulletManager.bulletVector[i]->x - camera.x, bulletManager.bulletVector[i]->y - camera.y, 8 , 8};
 
        SDL_RenderCopyEx(renderer, bullet_texture, NULL, &bullet_enemy, 0, NULL, SDL_FLIP_NONE);

     }

}

void Game::update(playerCharacter& player, bulletClassManager& bulletManager)
{

    for(std::vector<bulletClass*>::size_type i = 0; i !=bulletManager.bulletVector.size(); i++){
    
        bulletManager.bulletVector[i]->y += 10 * bulletManager.bulletVector[i]->dy;

        bulletManager.bulletVector[i]->x += 10 * bulletManager.bulletVector[i]->dx;
    }


    for(auto it = bulletManager.bulletVector.begin(); it != bulletManager.bulletVector.end();)
    {
        
       if((*it)->y > 1920 || (*it)->y < 0 || (*it)->x > 640 || (*it)-> x < 0){

            delete *it;

            it = bulletManager.bulletVector.erase(it);

       }else{

            it++;
       
       } 
    }

// Collision detect for enemy characters

    for(std::vector<enemyCharacter*>::size_type i = 0; i !=characterVector.size(); i++)
     {
        for(auto it = bulletManager.bulletVector.begin(); it != bulletManager.bulletVector.end();)
        {
            if((*it)->player_or_enemy == PLAYER && \
                (*it)->x + 8 < characterVector[i]->pos.x + PLAYER_WIDTH && (*it)->x > characterVector[i]->pos.x && \
                (*it)->y + 8 < characterVector[i]->pos.y + PLAYER_HEIGHT && (*it)->y > characterVector[i]->pos.y){
                    
                    characterVector[i]->alive = DEAD;
                
                    delete *it;

                    it = bulletManager.bulletVector.erase(it);
                    
            }else{

                    it++;
            }
        }
     }

// Collision detect for player character

    for(auto it = bulletManager.bulletVector.begin(); it != bulletManager.bulletVector.end();)
    {
    
        if((*it)->player_or_enemy == false && \
        (*it)->x + 8 < player.pos.x + PLAYER_WIDTH && (*it)->x > player.pos.x && \
        (*it)->y + 8 < player.pos.y + PLAYER_HEIGHT && (*it)->y > player.pos.y)
        {
                player.alive = DEAD;

                std::cout << "Player is dead" << std::endl;
            
                delete *it;

                it = bulletManager.bulletVector.erase(it);

        }else{

                it++;
        }
    }

    // Used to cycle through frames of a sprite

    //player.rect_player_source.x = 85 * int(((SDL_GetTicks() / 100) % 17));
}



void playerCharacter::fireBullet(bulletClassManager& bulletManager)
{
    if(fire == 1 && alive == ALIVE)
    {

        reloading = 50; 

        bulletClass* new_bullet;

        new_bullet = new bulletClass(); 

        new_bullet->x = pos.x + (PLAYER_WIDTH/2);

        new_bullet->y = pos.y;

        new_bullet->dx = 0; 

        new_bullet->dy = -1;

        new_bullet->angle = 0;

        new_bullet->player_or_enemy = true; 

        bulletManager.bulletVector.push_back(new_bullet);

        fire = 0;

    }

}
