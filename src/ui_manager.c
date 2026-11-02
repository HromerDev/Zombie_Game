#include "ui_manager.h"
#include "object.h"
#include "raylib.h"
#include "stdlib.h"
#include "dynamic_array.h"
#include "camera_handler.h"
#include <string.h>
#include "stdio.h"
#include "math.h"
#define DEFAULT_CONTAINER_CAPACITY 8
MousePointer mousePointer;
dynarray* allUIContainers;

void initUIContainers() {

    allUIContainers = malloc(sizeof(dynarray)); 
    dynarray_init(allUIContainers, 1);

    mousePointer.sprite = createSprite((Rectangle){-1,-1, 2,2}, TEXTURE_CURSOR, LAYER_CURSOR);
    mousePointer.circleCollider = mousePointer.sprite->position.area.width / 2;
}

UIContainer* createUIContainer() {

    UIContainer* temp = malloc(sizeof(UIContainer));
    
    temp->sprites = malloc(sizeof(Sprite*) * DEFAULT_CONTAINER_CAPACITY);
    temp->textUIElements = calloc(
    DEFAULT_CONTAINER_CAPACITY,
    sizeof(TextUIElement)
);
   
    for(size_t i = 0; i < DEFAULT_CONTAINER_CAPACITY; i++) 
        temp->sprites[i] = NULL;
    
    dynarray_push(allUIContainers, temp);

    return temp;
}

void configureUIContainer(UIContainer* uiContainer, int anchor) {

    uiContainer->anchor = anchor;
}

void configureUITextContainer(TextUIElement* textUIElement, Vector2 offset, int height, int spacing, Color color, int anchor) {

    textUIElement->offset = offset;
    textUIElement->position.area.height = (float)height * UNIT_PIXEL_SIZE / 2;
    textUIElement->anchor = anchor;
    textUIElement->color = color;
    movePosition(&textUIElement->position, offset.x, offset.y, anchor);
}

void updateUIElementPositions() {

    if(allUIContainers == NULL || allUIContainers->size < 1)
        return;

    for(int i = 0; i < allUIContainers->size; i++) {
        
        UIContainer* temp = allUIContainers->items[i];

        for(size_t j = 0; j < DEFAULT_CONTAINER_CAPACITY; j++) {
            
            if(temp->sprites[j] != NULL)
                movePosition(&temp->sprites[j]->position, camera.position.anchors[temp->anchor].x + temp->sprites[j]->originalArea.x * UNIT_PIXEL_SIZE, camera.position.anchors[temp->anchor].y + temp->sprites[j]->originalArea.y * UNIT_PIXEL_SIZE, ANCHOR_ORIGIN);
            if(temp->textUIElements[j].text[0] != '\0')
                movePosition(&temp->textUIElements[j].position, (int)(round(camera.position.anchors[temp->textUIElements[j].anchor].x + temp->textUIElements[j].offset.x)), (int)(round(camera.position.anchors[temp->textUIElements[j].anchor].y + temp->textUIElements[j].offset.y)), temp->textUIElements[j].anchor);
        }
    }
}
void updateText(TextUIElement* textUIElement, const char* text, Color color) {

    if (textUIElement == NULL)
        return;

    if (text != NULL)
        snprintf(textUIElement->text, sizeof(textUIElement->text), "%s", text);
    
    textUIElement->position.area.width = MeasureTextEx(GetFontDefault(), textUIElement->text, textUIElement->position.area.height, UNIT_PIXEL_SIZE).x;

    updatePositionAnchors(
        &textUIElement->position,
        ANCHOR_ORIGIN
    );
}

void drawUIText(UIContainer* uiContainer) { 

    for(int i = 0; i < DEFAULT_CONTAINER_CAPACITY; i++) {
        
        if(uiContainer->textUIElements[i].text[0] == '\0')
            continue;

        DrawTextEx(GetFontDefault(), uiContainer->textUIElements[i].text, uiContainer->textUIElements[i].position.anchors[ANCHOR_ORIGIN], uiContainer->textUIElements[i].position.area.height, 1 * UNIT_PIXEL_SIZE, uiContainer->textUIElements->color);
        DrawRectangleLines(uiContainer->textUIElements[i].position.area.x, uiContainer->textUIElements[i].position.area.y, uiContainer->textUIElements[i].position.area.width , uiContainer->textUIElements[i].position.area.height, GREEN);
    }
}

void handleMousePointer() {

    movePosition(&mousePointer.sprite->position, mousePointer.mousePositionGlobal.x, mousePointer.mousePositionGlobal.y, ANCHOR_CENTER);

    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        mousePointer.sprite->color = YELLOW;
    else if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        mousePointer.sprite->color = BLUE;
    else
        mousePointer.sprite->color = WHITE;
}

void recalculateTextUnits(double difference, double newUnitSize) {

if (allUIContainers != NULL) {

    for (int i = 0; i < allUIContainers->size; i++) {

        UIContainer* temp = allUIContainers->items[i];

        if (temp->textUIElements == NULL)
            continue;

        for (int j = 0; j < DEFAULT_CONTAINER_CAPACITY; j++) {

            TextUIElement* tempElement = &temp->textUIElements[j];

            tempElement->position.anchors[ANCHOR_ORIGIN].x *= difference;
            tempElement->position.anchors[ANCHOR_ORIGIN].y *= difference;

            tempElement->position.area.height *= difference;

            tempElement->position.area.width = MeasureTextEx(GetFontDefault(), tempElement->text, tempElement->position.area.height, newUnitSize).x;

            updatePositionAnchors(&tempElement->position, ANCHOR_ORIGIN);
            tempElement->position.area.x = tempElement->position.anchors[ANCHOR_ORIGIN].x;
            tempElement->position.area.y = tempElement->position.anchors[ANCHOR_ORIGIN].y;
        }
    }
}
}

void freeUIContainers() {

    if(allUIContainers->size < 1)
        return;

    for(size_t i = 0; i < allUIContainers->size; i++) {
        
        UIContainer* temp = allUIContainers->items[i];
        free(temp->textUIElements);
        free(temp->sprites);
    }
    dynarray_free(allUIContainers);
    free(allUIContainers);
    allUIContainers = NULL;
}
