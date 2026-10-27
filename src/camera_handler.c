#include "camera_handler.h" 
#include "object.h"
#include "../scripts/game_manager.h"
#include "raylib.h"
#include "raymath.h"
#include "ui_manager.h"

CameraProperties camera;

int UNIT_PIXEL_SIZE = 0;
int scaler = 68;
bool isToggledFullscreen = false;
int SCREENWIDTHPREFULLSCREEN;
int SCREENHEIGHTPREFULLSCREEN;

void recalculateCameraAnchors() {

    camera.anchors[ANCHOR_ORIGIN] = camera.camera.target;
    camera.anchors[ANCHOR_TOP_RIGHT] = (Vector2){camera.anchors[ANCHOR_ORIGIN].x + SCREENWIDTH, camera.anchors[ANCHOR_ORIGIN].y};
    camera.anchors[ANCHOR_BOTTOM_LEFT] = (Vector2){camera.anchors[ANCHOR_ORIGIN].x, camera.anchors[ANCHOR_ORIGIN].y + SCREENHEIGHT};
    camera.anchors[ANCHOR_BOTTOM_RIGHT] = (Vector2){camera.anchors[ANCHOR_TOP_RIGHT].x, camera.anchors[ANCHOR_BOTTOM_LEFT].y};
    camera.anchors[ANCHOR_CENTER] = (Vector2) {camera.anchors[ANCHOR_ORIGIN].x + (float)SCREENWIDTH / 2, camera.anchors[ANCHOR_ORIGIN].x + (float)SCREENHEIGHT / 2};

    updateUIElementPositions();
}

void cameraInit() {
   
    camera.camera.target = Vector2Zero();
    camera.camera.zoom = 1;

    recalculateCameraAnchors();
    changeResolution(SCREENWIDTH, SCREENHEIGHT);
}

void changeResolution(int newScreenWidth, int newScreenHeight) {

    int shortestScreenSide = (newScreenWidth < newScreenHeight) ? newScreenWidth : newScreenHeight;
    int newUnitSize = (int)round((double)shortestScreenSide / scaler);

    int leftover = newUnitSize % 4;
    
    if(leftover != 0)
        newUnitSize += 4 - leftover; 

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

    camera.camera.target.x = x;
    camera.camera.target.y = y;

    recalculateCameraAnchors();
}

void moveCameraRelative(float x, float y) {

    camera.camera.target.x += x;
    camera.camera.target.y += y;

    recalculateCameraAnchors();
}

void calculateMousePosition() {

    mousePointer.mousePositionLocal = GetMousePosition();
    mousePointer.mousePositionGlobal = GetScreenToWorld2D(mousePointer.mousePositionLocal, camera.camera);
}
