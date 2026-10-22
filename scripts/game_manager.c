#include "game_manager.h"
#include "../src/tilemap.h"
#include "../src/camera_handler.h"
#include "raylib.h"

int SCREENWIDTH = 500;
int SCREENHEIGHT = 500;
double delta = 0;
int FPS = 0;
Font font;
int cameraSpeed = 20;

void start() //initializations
{
    InitWindow(SCREENWIDTH, SCREENHEIGHT, WINDOWNAME);
    InitAudioDevice();
    font = LoadFontEx("assets/fonts/tiny5.ttf", 30, 0, 250);
    SetTargetFPS(FPS);
    SetWindowState(FLAG_VSYNC_HINT); 
}

void gameStart()
{
    initTextures();
    initTimer();
    initParticleSystem();

    cameraInit();
    initTiles();

    loadMap("debug_map.mapdata");
}

void restart() 
{
    freeCoreComponents();
    gameStart();
}
void update() //input handling, logic
{   
    if(IsKeyDown(KEY_LEFT_SHIFT))   
        cameraSpeed = 60; 
    else 
        cameraSpeed = 20;

    if(IsKeyDown(KEY_W))
        moveCameraRelative(0, -cameraSpeed * UNIT_PIXEL_SIZE * delta); 
    if(IsKeyDown(KEY_S))
        moveCameraRelative(0, cameraSpeed * UNIT_PIXEL_SIZE * delta); 
    if(IsKeyDown(KEY_A))
        moveCameraRelative(-cameraSpeed * UNIT_PIXEL_SIZE * delta, 0); 
    if(IsKeyDown(KEY_D))
        moveCameraRelative(cameraSpeed * UNIT_PIXEL_SIZE * delta, 0); 

    if(IsKeyPressed(KEY_F))
        fullScreenToggle(); 
    
}

void draw() //drawing a frame (primarely for text)
{
    DrawTextEx(font, TextFormat("X: %.0f, Y:%.0f", mousePositionLocal.x, mousePositionLocal.y), (Vector2){(int)(round(camera.target.x + 10)),(int)(round(camera.target.y + 10))},font.baseSize, 0, YELLOW);

    DrawText(TextFormat("X: %.0f, Y:%.0f", mousePositonGlobal.x, mousePositonGlobal.y),(int)(round(camera.target.x + 10)), (int)(round(camera.target.y + 40)), 30, YELLOW);

    DrawText(TextFormat("%.0fFPS", 1 / GetFrameTime()), (int)(round(CAMERA_ANCHOR_BOTTOM_LEFT.x)), (int)(round(CAMERA_ANCHOR_BOTTOM_LEFT.y - 20)), 20, GREEN);
}

void end() 
{ 
    if(CURRENTLY_LOADED_MAP.mapData != NULL) 
        freeMapData();
}

void freeCoreComponents() 
{
    freeSprites();
    freeObjects();
    freeTimers();
    freeParticles();
    UnloadFont(font);
}
