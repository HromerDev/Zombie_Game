#include "game_manager.h"
#include "../src/unit_system.h"
#include "raylib.h"

int SCREENWIDTH = 500;
int SCREENHEIGHT = 500;
int FPS = 0;
bool gameOver = true;

void start() //initializations
{
    InitWindow(SCREENWIDTH, SCREENHEIGHT, WINDOWNAME);
    InitAudioDevice();
    SetTargetFPS(FPS);
    SetWindowState(FLAG_VSYNC_HINT);
    ToggleFullscreen();
    recalculateUnitSize(GetScreenWidth(), GetScreenHeight());
}

void gameStart()
{
    initTextures();
    initTimer();
    initParticleSystem();
}

void restart() 
{
    freeCoreComponents();
    gameStart();
}
void update() //input handling, logic
{   
    
}

void draw() //drawing a frame (primarely for text)
{   
   DrawText("Hello World!", 10, 10, 30, YELLOW);
}

void end() 
{ 
    
}

void freeCoreComponents() 
{
    freeSprites();
    freeObjects();
    freeTimers();
    freeParticles();
}
