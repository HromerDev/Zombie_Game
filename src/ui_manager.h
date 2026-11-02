#ifndef UI_MANAGER_H
#define UI_MANAGER_H
#include "dynamic_array.h"
#include "object.h"
#include "raylib.h"

typedef struct {

    Position position;
    char text[64];
    Vector2 offset;
    int anchor;
    Color color;

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

void configureUITextContainer(TextUIElement* textUIElement, Vector2 origin, int height, int spacing, Color color, int anchor);
void updateText(TextUIElement* textUIElement, const char* text, Color color);
void drawUIText(UIContainer* uiContainer);
void initUIContainers();
UIContainer* createUIContainer();
void configureUIContainer(UIContainer* uiContainer, int anchor);
void updateUIElementPositions();
void drawUIText(UIContainer* uiContainer);
void recalculateTextUnits(double difference, double newUnitSize);
void handleMousePointer();
void freeUIContainers();
#endif
