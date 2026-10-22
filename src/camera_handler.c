#include "camera_handler.h" 
#include "object.h"
#include "../scripts/game_manager.h"
#include "raymath.h"

double UNIT_PIXEL_SIZE = 0;
int scaler = 60;
bool isToggledFullscreen = false;
int SCREENWIDTHPREFULLSCREEN;
int SCREENHEIGHTPREFULLSCREEN;

Vector2 CAMERA_ANCHOR;
Vector2 CAMERA_ANCHOR_TOP_RIGHT;
Vector2 CAMERA_ANCHOR_BOTTOM_LEFT;
Vector2 CAMERA_ANCHOR_BOTTOM_RIGHT;

void cameraInit() {

    changeResolution(SCREENWIDTH, SCREENHEIGHT);
    camera.offset = Vector2Zero();
    camera.zoom = 1;
}

void changeResolution(int newScreenWidth, int newScreenHeight)
{
    double shortestScreenSide = (newScreenWidth < newScreenHeight) ? newScreenWidth : newScreenHeight;
    double newUnitSize = shortestScreenSide / scaler;

    SCREENWIDTH = newScreenWidth;
    SCREENHEIGHT = newScreenHeight;
    recalculateAllUnits(newUnitSize, UNIT_PIXEL_SIZE);
    UNIT_PIXEL_SIZE = newUnitSize;
    
    CAMERA_ANCHOR = camera.offset;
    CAMERA_ANCHOR_TOP_RIGHT = (Vector2){camera.offset.x + SCREENWIDTH, camera.offset.y};
    CAMERA_ANCHOR_BOTTOM_LEFT = (Vector2){camera.offset.x, camera.offset.y + SCREENHEIGHT};
    CAMERA_ANCHOR_BOTTOM_RIGHT = (Vector2){camera.offset.x + SCREENWIDTH, camera.offset.y + SCREENHEIGHT};

    SetWindowSize(SCREENWIDTH, SCREENHEIGHT);
}

void fullScreenToggle() {
    
    if(isToggledFullscreen){

        ToggleFullscreen();
        SCREENWIDTH = SCREENWIDTHPREFULLSCREEN;
        SCREENHEIGHT = SCREENHEIGHTPREFULLSCREEN;

        changeResolution(SCREENWIDTH, SCREENHEIGHT);
        isToggledFullscreen = false;
            
    }
    else {
                 
        ToggleFullscreen();

        SCREENWIDTHPREFULLSCREEN = SCREENWIDTH;
        SCREENHEIGHTPREFULLSCREEN = SCREENHEIGHT;

        changeResolution(GetScreenWidth(), GetScreenHeight());

        isToggledFullscreen = false;
    } 
}
