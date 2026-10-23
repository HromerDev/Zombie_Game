#ifndef CAMERA_HANDLER_H
#define CAMERA_HANDLER_H
#include "raylib.h"
#include "object.h"

extern Camera2D camera;

extern Vector2 mousePosition;

extern double UNIT_PIXEL_SIZE;

extern Vector2 CAMERA_ANCHOR;
extern Vector2 CAMERA_ANCHOR_TOP_RIGHT;
extern Vector2 CAMERA_ANCHOR_BOTTOM_LEFT;
extern Vector2 CAMERA_ANCHOR_BOTTOM_RIGHT;
extern Vector2 CAMERA_ANCHOR_CENTER;

extern Vector2 mousePositionLocal;
extern Vector2 mousePositionGlobal;

void cameraInit();
void changeResolution(int newScreenWidth, int newScreenHeight);
void fullScreenToggle();

void moveCamera(int x, int y);
void moveCameraRelative(float x, float y);

void calculateMousePosition();
#endif
