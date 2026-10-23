#include "camera_handler.h" 
#include "object.h"
#include "../scripts/game_manager.h"
#include "raylib.h"
#include "raymath.h"
#include "ui_manager.h"
Camera2D camera;

Vector2 mousePositionLocal;
Vector2 mousePositionGlobal;

int UNIT_PIXEL_SIZE = 0;
int scaler = 60;
bool isToggledFullscreen = false;
int SCREENWIDTHPREFULLSCREEN;
int SCREENHEIGHTPREFULLSCREEN;

Vector2 CAMERA_ANCHOR;
Vector2 CAMERA_ANCHOR_TOP_RIGHT;
Vector2 CAMERA_ANCHOR_BOTTOM_LEFT;
Vector2 CAMERA_ANCHOR_BOTTOM_RIGHT;
Vector2 CAMERA_ANCHOR_CENTER;

void recalculateCameraAnchors() {

    CAMERA_ANCHOR = camera.target;
    CAMERA_ANCHOR_TOP_RIGHT = (Vector2){camera.target.x + SCREENWIDTH, camera.target.y};
    CAMERA_ANCHOR_BOTTOM_LEFT = (Vector2){camera.target.x, camera.target.y + SCREENHEIGHT};
    CAMERA_ANCHOR_BOTTOM_RIGHT = (Vector2){camera.target.x + SCREENWIDTH, camera.target.y + SCREENHEIGHT};
    CAMERA_ANCHOR_CENTER = (Vector2) {camera.target.x + SCREENWIDTH / 2, camera.target.y + SCREENHEIGHT / 2};
    updateUIElementPositions();
}

void cameraInit() {
    
    camera.target = Vector2Zero();
    camera.zoom = 1;
    changeResolution(SCREENWIDTH, SCREENHEIGHT);
}

void changeResolution(int newScreenWidth, int newScreenHeight) {

    int shortestScreenSide = (newScreenWidth < newScreenHeight) ? newScreenWidth : newScreenHeight;
    int newUnitSize = (int)round((double)shortestScreenSide / scaler);


    int leftover = newUnitSize % 8;
    
    if(leftover != 0)
        newUnitSize += 8 - leftover; 

    recalculateAllUnits(newUnitSize, UNIT_PIXEL_SIZE);
    UNIT_PIXEL_SIZE = newUnitSize;

    SCREENWIDTH = newScreenWidth;
    SCREENHEIGHT = newScreenHeight;

    recalculateCameraAnchors();

    if(mousePointer.sprite != NULL)
        mousePointer.circleCollider = mousePointer.sprite->textureArea.width / 2;

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
        isToggledFullscreen = true;
    } 

}

void moveCamera(int x, int y) {

    camera.target.x = x;
    camera.target.y = y;

    recalculateCameraAnchors();
}

void moveCameraRelative(float x, float y) {

    camera.target.x += x;
    camera.target.y += y;

    recalculateCameraAnchors();
}

void calculateMousePosition() {

    mousePositionLocal = GetMousePosition();
    mousePositionGlobal = GetScreenToWorld2D(mousePositionLocal, camera);
}
