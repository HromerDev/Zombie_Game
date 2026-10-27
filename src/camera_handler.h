#ifndef CAMERA_HANDLER_H
#define CAMERA_HANDLER_H
#include "raylib.h"
#include "object.h"

extern Vector2 mousePosition;

extern int UNIT_PIXEL_SIZE;


typedef struct {

    Camera2D camera;
    Vector2 anchors[5];

} CameraProperties;

extern CameraProperties camera;

void cameraInit();
void recalculateCameraAnchors();
void changeResolution(int newScreenWidth, int newScreenHeight);
void fullScreenToggle();

void moveCamera(int x, int y);
void moveCameraRelative(float x, float y);

void calculateMousePosition();
#endif
