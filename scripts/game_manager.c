#include "game_manager.h"
#include "../src/unit_system.h"
#include "raylib.h"

int SCREENWIDTH = 500;
int SCREENHEIGHT = 500;
int FPS = 0;
bool gameOver = true;
Vector2 cameraTarget;
Camera2D camera;

void start() //initializations
{
    InitWindow(SCREENWIDTH, SCREENHEIGHT, WINDOWNAME);
    InitAudioDevice();
    SetTargetFPS(FPS);
//    SetWindowState(FLAG_VSYNC_HINT);
    ToggleFullscreen();
    recalculateUnitSize(GetScreenWidth(), GetScreenHeight());
}

void gameStart()
{
    initTextures();
    initTimer();
    initParticleSystem();
    
    camera.zoom = 1;
    camera.target = (Vector2){0,0};

    for(int i = 0; i * UNIT_PIXEL_SIZE< SCREENWIDTH; i+=4) {
        for(int j = 0; j * UNIT_PIXEL_SIZE < SCREENHEIGHT; j+=4) 
        {
            createObject((Rectangle){i,j,4,4}, createSprite((Rectangle){i,j,4,4}, TEXTURE_DEBUG, 0,0),0);
        }
    }
}

void restart() 
{
    freeCoreComponents();
    gameStart();
}
void update() //input handling, logic
{   
   if(IsKeyDown(KEY_W))
       camera.target.y -= 20 * UNIT_PIXEL_SIZE * GetFrameTime();
   if(IsKeyDown(KEY_S))
       camera.target.y += 20 * UNIT_PIXEL_SIZE * GetFrameTime();
   if(IsKeyDown(KEY_A))
       camera.target.x -= 20 * UNIT_PIXEL_SIZE * GetFrameTime();
   if(IsKeyDown(KEY_D))
       camera.target.x += 20 * UNIT_PIXEL_SIZE * GetFrameTime();
}

void draw() //drawing a frame (primarely for text)
{ 
     
    
    DrawText("Hello World!", camera.target.x + 10, camera.target.y + 10, 30, YELLOW);

    DrawText(TextFormat("%.0fFPS", 1 / GetFrameTime()), camera.target.x , camera.target.y + SCREENHEIGHT - 20, 20, GREEN);
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
