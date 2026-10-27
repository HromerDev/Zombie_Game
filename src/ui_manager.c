#include "ui_manager.h"
#include "object.h"
#include "raylib.h"
#include "stdlib.h"
#include "dynamic_array.h"
#include "camera_handler.h"
#define DEFAULT_CONTAINER_CAPACITY 8
MousePointer mousePointer;
dynarray* allUIContainers;

void initUIContainers() {

    allUIContainers = malloc(sizeof(dynarray)); 
    dynarray_init(allUIContainers, 1);

    mousePointer.sprite = createSprite((Rectangle){-1,-1, 2,2}, TEXTURE_CURSOR, LAYER_CURSOR);
    mousePointer.circleCollider = mousePointer.sprite->textureArea.width / 2;
}

UIContainer* createUIContainer() {

    UIContainer* temp = malloc(sizeof(UIContainer));
    
    temp->sprites = malloc(sizeof(Sprite*) * DEFAULT_CONTAINER_CAPACITY);
    temp->text = malloc(sizeof(char) * 64 * DEFAULT_CONTAINER_CAPACITY);
   
    
    for(size_t i = 0; i < DEFAULT_CONTAINER_CAPACITY; i++) 
        temp->sprites[i] = NULL;
    
    dynarray_push(allUIContainers, temp);

    return temp;
}

void configureUIContainer(UIContainer* uiContainer, float* x, float* y) {
    
   uiContainer->anchorX = x;
   uiContainer->anchorY = y;
}

void updateUIElementPositions() {

    if(allUIContainers == NULL || allUIContainers->size < 1)
        return;

    for(int i = 0; i < allUIContainers->size; i++) {
        
        UIContainer* temp = allUIContainers->items[i];

        for(size_t j = 0; j < DEFAULT_CONTAINER_CAPACITY; j++) {
            
            if(temp->sprites[j] == NULL)
                continue;
            
            moveSprite(temp->sprites[j], *temp->anchorX + temp->sprites[j]->originalArea.x * UNIT_PIXEL_SIZE, *temp->anchorY + temp->sprites[j]->originalArea.y * UNIT_PIXEL_SIZE, ANCHOR_ORIGIN);
        }
    }
}

void handleMousePointer() {

    moveSprite(mousePointer.sprite, mousePointer.mousePositionGlobal.x, mousePointer.mousePositionGlobal.y, ANCHOR_CENTER);

    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        mousePointer.sprite->color = YELLOW;
    else if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        mousePointer.sprite->color = BLUE;
    else
        mousePointer.sprite->color = WHITE;
}

void freeUIContainers() {

    if(allUIContainers->size < 1)
        return;

    for(size_t i = 0; i < allUIContainers->size; i++) {
        
        UIContainer* temp = allUIContainers->items[i];
        free(temp->text);
        free(temp->sprites);
    }
    dynarray_free(allUIContainers);
    free(allUIContainers);
    allUIContainers = NULL;
}
