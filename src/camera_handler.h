#ifndef CAMERA_HANDLER_H
#define CAMERA_HANDLER_H
#include "raylib.h"

extern double UNIT_PIXEL_SIZE;
extern Vector2 CAMERA_ANCHOR;
extern Vector2 CAMERA_ANCHOR_TOP_RIGHT;
extern Vector2 CAMERA_ANCHOR_BOTTOM_LEFT;
extern Vector2 CAMERA_ANCHOR_BOTTOM_RIGHT;

void cameraInit();
void changeResolution(int newScreenWidth, int newScreenHeight);
void fullScreenToggle();

#endif
