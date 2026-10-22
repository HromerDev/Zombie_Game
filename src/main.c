#include "raylib.h"
#include "object.h"
#include "timer.h"
#include "../scripts/game_manager.h"
#include <time.h>
#include "camera_handler.h"
#include "tilemap.h"
int main()
{
    SetRandomSeed(time(NULL));
    start();
    gameStart();
    
    while (!WindowShouldClose()) 
    {        
        update();
        iterateTimers();

        BeginDrawing();
        BeginMode2D(camera);
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

