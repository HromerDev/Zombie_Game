#ifndef UI_MANAGER_H
#define UI_MANAGER_H
#include "dynamic_array.h"
#include "object.h"
typedef struct {

    float* anchorX;
    float* anchorY;
    char (*text)[64];
    Sprite** sprites;
    //Button* buttons


} UIContainer;

typedef struct {

    float circleCollider;
    Sprite* sprite;

} MousePointer;

extern MousePointer mousePointer;

void initUIContainers();
UIContainer* createUIContainer();
void configureUIContainer(UIContainer* uiContainer, float* x, float* y);
void updateUIElementPositions();

void handleMousePointer();
void freeUIContainers();
#endif
