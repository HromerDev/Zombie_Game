#include "raylib.h"
#include "object.h"
#include "timer.h"
#include "../scripts/game_manager.h"
#include <time.h>
#include "camera_handler.h"
#include "tilemap.h"
#include "ui_manager.h" 
int main()
{
    SetRandomSeed(time(NULL));
    start();
    gameStart();
    
    updateUIElementPositions();

    while (!WindowShouldClose()) 
    {   
        delta = GetFrameTime();
        update();
        calculateMousePosition();
        handleMousePointer();
        iterateTimers();

        BeginDrawing();
        BeginMode2D(camera.camera);
            ClearBackground(BLACK);
            drawMap();
            drawSprites();  
            draw();
        EndMode2D();
        EndDrawing();
    }       

    end();
    freeCoreComponents();
    CloseWindow();        

    return 0;
}

