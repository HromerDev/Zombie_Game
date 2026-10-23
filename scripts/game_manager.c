#include "game_manager.h"
#include "../src/tilemap.h"
#include "../src/camera_handler.h"
#include "raylib.h"
#include "../src/ui_manager.h"  
#include "../src/object.h"
int SCREENWIDTH = 1000;
int SCREENHEIGHT = 500;
double delta = 0;
int FPS = 60;
Font font;
int cameraSpeed = 20;
UIContainer* debug;

void start() //initializations
{
    InitWindow(SCREENWIDTH, SCREENHEIGHT, WINDOWNAME);
    InitAudioDevice();
    font = LoadFontEx("assets/fonts/tiny5.ttf", 30, 0, 250);
    SetTargetFPS(FPS);
    SetWindowState(FLAG_VSYNC_HINT); 
    HideCursor();
}

void gameStart()
{
    initTextures();
    initTimer();
    initParticleSystem();

    cameraInit();
    initUIContainers(); 

    initTiles();
    loadMap("debug_map.mapdata");

    debug = createUIContainer();
    configureUIContainer(debug, &CAMERA_ANCHOR_TOP_RIGHT.x, &CAMERA_ANCHOR.y);
    
    debug->sprites[0] = createSprite((Rectangle){-2,0,2,2}, TEXTURE_CURSOR, LAYER_UI, 0);
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

    DrawText(TextFormat("X: %.0f, Y:%.0f", mousePositionGlobal.x, mousePositionGlobal.y),(int)(round(camera.target.x + 10)), (int)(round(camera.target.y + 40)), 30, YELLOW);

    DrawText(TextFormat("unit pixel size:%d pixels",UNIT_PIXEL_SIZE),(int)(round(camera.target.x + 10)), (int)(round(camera.target.y + 70)), 30, YELLOW);

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
    freeUIContainers();
    UnloadFont(font);
}
