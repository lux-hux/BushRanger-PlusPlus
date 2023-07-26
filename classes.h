#ifndef OBJECTSANDITEMS
#define OBJECTSANDITEMS

#include <SDL2/SDL.h>
#include "SDL_ttf.h"
#include "SDL_image.h"
#include <string>
#include <vector>
#include <iostream>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SCREEN_HEIGHT_HALVED 240

#define PLAYER_HEIGHT 48
#define PLAYER_WIDTH 48

#define BULLET_DIMENSION 8 

#define ENEMY_HEIGHT 48
#define ENEMY_WIDTH 48

#define PLAYER_SPEED 4

#define PLAYER true 
#define ENEMY false

#define ALIVE 1 
#define DEAD 0 


class textureManager{

    private:

    textureManager();

    ~textureManager();

};


class bulletClass{

    public:

    int x; 

    int y;

    float dx;

    float dy;

    int angle;

    bool player_or_enemy; 

    bulletClass();

    ~bulletClass();
    
};

class bulletClassManager
{
    public:

    int x_aim;

    int y_aim;

    int bullet_angle;
    
    std::vector <bulletClass*> bulletVector;

    bulletClassManager();

    ~bulletClassManager();

};

class Character
{
    public:

        int reloading = {15};

        bool alive = {ALIVE};

        bool face_left = {false}; 

        bool face_right = {false};

        bool moving = {false};

        bool characterMoving = {false};

        SDL_RendererFlip flip;

   protected:
        virtual void setSize(int w, int h) = 0;

        virtual void setPos(int x, int y) = 0;

        virtual void move() = 0;
};





class playerCharacter: public Character
{
    public:

    playerCharacter();

    virtual ~playerCharacter();

    // | METHODS |

    virtual void setSize(int w, int h) override;

    virtual void setPos(int x, int y) override;

    void fireBullet(bulletClassManager& bulletManager);

    // Move was later moved to the Game class, but it is a pure virtual method for character.

    virtual void move() override;

    // | ATTRIBUTES |

    int fire = 0;

    int animFrame;

    SDL_Rect pos;

    SDL_Texture *image[12];

    SDL_Rect rect_player_dest;

    SDL_Rect rect_player_source;

};


class enemyCharacter: public Character
{

    public:

    enemyCharacter();

    ~enemyCharacter();

    // | METHODS |

    virtual void move() override;

    virtual void setSize(int w, int h) override;

    virtual void setPos(int x, int y) override;

    void fireBullet(bulletClassManager& bulletManager);

    // | ATTRIBUTES |

    int reloading = 15;

    int animFrame;

    SDL_Rect pos;

    SDL_Texture * image;

    SDL_Rect rect_player_dest;

    SDL_Rect rect_player_source;

};





class Game
{

    public:

    Game();

    ~Game();

    SDL_Rect camera;

    SDL_Texture *background;

    SDL_Renderer *renderer;

    SDL_Texture *bullet_texture;

    SDL_Window *window; 

    std::vector <enemyCharacter*> characterVector;  

    void initSDL();

    void positionCamera(playerCharacter& player);

    void drawFigures(playerCharacter& player, bulletClassManager& bulletManager);

    void prepareScene();

    void update(playerCharacter& player, bulletClassManager& bulletManager);

    void presentScene();

    void handleEvents(playerCharacter& mainCharacter);

    void doKeyDown(SDL_KeyboardEvent *event);

    void doKeyUp(SDL_KeyboardEvent *event);

    void move(playerCharacter& player);

    void setBackgroundCoordinates();

    void incrementEnemies();

    int whatsTheTime();

    void incrementTime();

    void moveEnemies(playerCharacter& player, bulletClassManager& bulletManager);
    
    

    void doAnimation(playerCharacter& player);

    SDL_Texture *loadTexture(std::string fileName);

    bool running(){return m_bRunning;}

    private:

    bool m_bRunning;

    int x_background;

    int y_background;

    int noEnemies;

    int gameTime;

    bool up = {false};

    bool down = {false};

    bool left = {false};

    bool right = {false};

    bool fire = {false};

};



#endif