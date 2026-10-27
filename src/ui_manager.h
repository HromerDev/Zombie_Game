#ifndef UI_MANAGER_H
#define UI_MANAGER_H
#include "dynamic_array.h"
#include "object.h"
#include "raylib.h"

typedef struct {

    Rectangle textArea;
    Vector2 anchors[5];
    char* text;

} TextUIElement;

typedef struct {

    int anchor;
    TextUIElement* textUIElements;
    Sprite** sprites;
    //Button* buttons

} UIContainer;

typedef struct {

    float circleCollider;
    Sprite* sprite;
    Vector2 mousePositionLocal;
    Vector2 mousePositionGlobal;

} MousePointer;

extern MousePointer mousePointer;

void initUIContainers();
UIContainer* createUIContainer();
void configureUIContainer(UIContainer* uiContainer, int anchor);
void updateUIElementPositions();

void handleMousePointer();
void freeUIContainers();
#endif
