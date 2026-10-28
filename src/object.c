#include "dynamic_array.h"
#include "raylib.h"
#include "camera_handler.h" 
#include "object.h"
#include "stdio.h"
#include "stdlib.h"
#include <time.h>
#include "../scripts/game_manager.h"

Color backgroundColor = {0, 0, 0, 255};
Texture2D textureArray[10];
Sound sounds[10];
dynarray *allObjects;
dynarray *allSprites;   

void initTextures() {

    allObjects = malloc(sizeof(dynarray)); 
    dynarray_init(allObjects, 1);

    allSprites = malloc(sizeof(dynarray));
    dynarray_init(allSprites, 1);
    textureArray[TEXTURE_DEBUG] = LoadTexture("assets/debug.png");  

    textureArray[TEXTURE_CURSOR] = LoadTexture("assets/cursor.png");

    SetTextureFilter(textureArray[TEXTURE_DEBUG], TEXTURE_FILTER_POINT);
    SetTextureFilter(textureArray[TEXTURE_CURSOR], TEXTURE_FILTER_POINT);
}

Object* createObject(Rectangle collider, Sprite *sprite) {

    Object *object = malloc(sizeof(Object));
    object->originalCollider = collider;

    object->position.area = (Rectangle){collider.x * UNIT_PIXEL_SIZE, collider.y * UNIT_PIXEL_SIZE, collider.width * UNIT_PIXEL_SIZE, collider.height * UNIT_PIXEL_SIZE};

    object->position.anchors[ANCHOR_ORIGIN].x = collider.x;
    object->position.anchors[ANCHOR_ORIGIN].y = collider.y;

    object->sprite = sprite;
    object->angle = 0;
    
    object->spriteOffset = (Vector2){0,0};

    updatePositionAnchors(&object->position, ANCHOR_ORIGIN);

    movePosition(&object->sprite->position, object->position.anchors[ANCHOR_CENTER].x + object->spriteOffset.x, object->position.anchors[ANCHOR_CENTER].y + object->spriteOffset.y, ANCHOR_CENTER);  
    dynarray_push(allObjects, object);
    
    return object;
}

Sprite* createSprite(Rectangle textureArea, unsigned int texture, unsigned int layer) {

    Sprite* sprite = malloc(sizeof(Sprite));    
    sprite->originalArea = textureArea;
    sprite->color = WHITE;
    
    sprite->position.area = (Rectangle) {
        
        textureArea.x * UNIT_PIXEL_SIZE,
        textureArea.y * UNIT_PIXEL_SIZE,
        textureArea.width * UNIT_PIXEL_SIZE,
        textureArea.height * UNIT_PIXEL_SIZE
    };

    sprite->texture = texture; 
    sprite->layer = layer;
    sprite->angle = 0;
    sprite->opacity = 255;

    updatePositionAnchors(&sprite->position, ANCHOR_ORIGIN);
    dynarray_push(allSprites, sprite);

    // Binary search for the insertion position.
    int low = 0;
    int high = allSprites->size - 1;

    Sprite* temp = allSprites->items[0];

    while (low < high)
    {
        int mid = low + (high - low) / 2;
        temp = allSprites->items[mid];

        if (temp->layer < sprite->layer)       
            low = mid + 1;        
        else        
            high = mid;        
    }

    int insertIndex = low;
    temp = allSprites->items[insertIndex];
    // If the new sprite belongs after the last element.
    if (temp->layer < sprite->layer)
        insertIndex++;

    // Shift elements right to make room.
    for (int i = allSprites->size - 1; i > insertIndex; i--)   
        allSprites->items[i] = allSprites->items[i - 1];
    
    allSprites->items[insertIndex] = sprite;

    return sprite;
}

bool layersSortedCorrectly() {        

    for(int i = 0; i < allSprites->size - 1; i++) {   
        Sprite* temp1 = allSprites->items[i];
        Sprite* temp2 = allSprites->items[i + 1];
        
        if(temp1->layer > temp2->layer)
            return true;
    }
    return true;
}

void destroySprite(Sprite *sprite) {

    dynarray_remove(allSprites, sprite);
}

void destroyObject(Object *object) {

    dynarray_remove(allObjects, object);
}

void updatePositionAnchors(Position* positionData, int anchor) {

    anchor = (anchor <= ANCHOR_CENTER) ? anchor : ANCHOR_ORIGIN;

    switch(anchor) {

        case ANCHOR_ORIGIN:

            positionData->anchors[ANCHOR_TOP_RIGHT].x = positionData->anchors[ANCHOR_ORIGIN].x + positionData->area.width;
            positionData->anchors[ANCHOR_TOP_RIGHT].y = positionData->anchors[ANCHOR_ORIGIN].y;

            positionData->anchors[ANCHOR_BOTTOM_LEFT].x = positionData->anchors[ANCHOR_ORIGIN].x;
            positionData->anchors[ANCHOR_BOTTOM_LEFT].y = positionData->anchors[ANCHOR_ORIGIN].y + positionData->area.height;

            positionData->anchors[ANCHOR_BOTTOM_RIGHT].x = positionData->anchors[ANCHOR_BOTTOM_LEFT].x + positionData->area.width;
            positionData->anchors[ANCHOR_BOTTOM_RIGHT].y = positionData->anchors[ANCHOR_BOTTOM_LEFT].y;

            positionData->anchors[ANCHOR_CENTER].x = positionData->anchors[ANCHOR_ORIGIN].x + positionData->area.width / 2;
            positionData->anchors[ANCHOR_CENTER].y = positionData->anchors[ANCHOR_ORIGIN].y + positionData->area.height / 2;
            break;

        case ANCHOR_TOP_RIGHT:

            positionData->anchors[ANCHOR_ORIGIN].x = positionData->anchors[ANCHOR_TOP_RIGHT].x - positionData->area.width;
            positionData->anchors[ANCHOR_ORIGIN].y = positionData->anchors[ANCHOR_TOP_RIGHT].y;
            
            positionData->anchors[ANCHOR_BOTTOM_LEFT].x = positionData->anchors[ANCHOR_ORIGIN].x;
            positionData->anchors[ANCHOR_BOTTOM_LEFT].y = positionData->anchors[ANCHOR_ORIGIN].y + positionData->area.height;

            positionData->anchors[ANCHOR_BOTTOM_RIGHT].x = positionData->anchors[ANCHOR_BOTTOM_LEFT].x + positionData->area.width;
            positionData->anchors[ANCHOR_BOTTOM_RIGHT].y = positionData->anchors[ANCHOR_BOTTOM_LEFT].y;

            positionData->anchors[ANCHOR_CENTER].x = positionData->anchors[ANCHOR_ORIGIN].x + positionData->area.width / 2;
            positionData->anchors[ANCHOR_CENTER].y = positionData->anchors[ANCHOR_ORIGIN].y + positionData->area.height / 2;   
            break;

        case ANCHOR_BOTTOM_LEFT:

            positionData->anchors[ANCHOR_ORIGIN].x = positionData->anchors[ANCHOR_BOTTOM_LEFT].x;
            positionData->anchors[ANCHOR_ORIGIN].y = positionData->anchors[ANCHOR_BOTTOM_LEFT].y - positionData->area.height;

            positionData->anchors[ANCHOR_TOP_RIGHT].x = positionData->anchors[ANCHOR_ORIGIN].x + positionData->area.width;
            positionData->anchors[ANCHOR_TOP_RIGHT].y = positionData->anchors[ANCHOR_ORIGIN].y;
            
            positionData->anchors[ANCHOR_BOTTOM_RIGHT].x = positionData->anchors[ANCHOR_BOTTOM_LEFT].x + positionData->area.width;
            positionData->anchors[ANCHOR_BOTTOM_RIGHT].y = positionData->anchors[ANCHOR_BOTTOM_LEFT].y;

            positionData->anchors[ANCHOR_CENTER].x = positionData->anchors[ANCHOR_ORIGIN].x + positionData->area.width / 2;
            positionData->anchors[ANCHOR_CENTER].y = positionData->anchors[ANCHOR_ORIGIN].y + positionData->area.height / 2;   
            break;

        case ANCHOR_BOTTOM_RIGHT:

            positionData->anchors[ANCHOR_ORIGIN].x = positionData->anchors[ANCHOR_BOTTOM_RIGHT].x - positionData->area.width;
            positionData->anchors[ANCHOR_ORIGIN].y = positionData->anchors[ANCHOR_BOTTOM_RIGHT].y - positionData->area.height;

            positionData->anchors[ANCHOR_TOP_RIGHT].x = positionData->anchors[ANCHOR_ORIGIN].x + positionData->area.width;
            positionData->anchors[ANCHOR_TOP_RIGHT].y = positionData->anchors[ANCHOR_ORIGIN].y;

            positionData->anchors[ANCHOR_BOTTOM_LEFT].x = positionData->anchors[ANCHOR_ORIGIN].x;
            positionData->anchors[ANCHOR_BOTTOM_LEFT].y = positionData->anchors[ANCHOR_ORIGIN].y + positionData->area.height;

            positionData->anchors[ANCHOR_CENTER].x = positionData->anchors[ANCHOR_ORIGIN].x + positionData->area.width / 2;
            positionData->anchors[ANCHOR_CENTER].y = positionData->anchors[ANCHOR_ORIGIN].y + positionData->area.height / 2;   
            break;

        case ANCHOR_CENTER:

            positionData->anchors[ANCHOR_ORIGIN].x = positionData->anchors[ANCHOR_CENTER].x - positionData->area.width / 2;
            positionData->anchors[ANCHOR_ORIGIN].y = positionData->anchors[ANCHOR_CENTER].y - positionData->area.height / 2;

            positionData->anchors[ANCHOR_TOP_RIGHT].x = positionData->anchors[ANCHOR_ORIGIN].x + positionData->area.width;
            positionData->anchors[ANCHOR_TOP_RIGHT].y = positionData->anchors[ANCHOR_ORIGIN].y;
            
            positionData->anchors[ANCHOR_BOTTOM_LEFT].x = positionData->anchors[ANCHOR_ORIGIN].x;
            positionData->anchors[ANCHOR_BOTTOM_LEFT].y = positionData->anchors[ANCHOR_ORIGIN].y + positionData->area.height;
            
            positionData->anchors[ANCHOR_BOTTOM_RIGHT].x = positionData->anchors[ANCHOR_BOTTOM_LEFT].x + positionData->area.width;
            positionData->anchors[ANCHOR_BOTTOM_RIGHT].y = positionData->anchors[ANCHOR_BOTTOM_LEFT].y;
            break;
    }
}

void movePosition(Position* positionData, float x, float y, int anchor) {
   
    anchor = (anchor <= ANCHOR_CENTER) ? anchor : ANCHOR_ORIGIN;

    positionData->anchors[anchor].x = x;
    positionData->anchors[anchor].y = y;
            
    updatePositionAnchors(positionData, anchor);

    positionData->area.x = positionData->anchors[ANCHOR_ORIGIN].x;
    positionData->area.y = positionData->anchors[ANCHOR_ORIGIN].y;
}

void movePositionRelative(Position* positionData, float x, float y, int anchor) {

    anchor = (anchor <= ANCHOR_CENTER) ? anchor : ANCHOR_ORIGIN;

    positionData->anchors[anchor].x += x * UNIT_PIXEL_SIZE;
    positionData->anchors[anchor].y += y * UNIT_PIXEL_SIZE;

    updatePositionAnchors(positionData, anchor);

    positionData->area.x = positionData->anchors[ANCHOR_ORIGIN].x;
    positionData->area.y = positionData->anchors[ANCHOR_ORIGIN].y;
}

void drawSprites() {
    
    for(int i = 0; i < allSprites->size; i++) {

        Sprite *temp = allSprites->items[i];
        
        DrawTexturePro(textureArray[temp->texture], (Rectangle){0,0, textureArray[temp->texture].width, textureArray[temp->texture].height}, (Rectangle) {temp->position.anchors[ANCHOR_CENTER].x , temp->position.anchors[ANCHOR_CENTER].y, temp->position.area.width, temp->position.area.height}, (Vector2){temp->position.area.width / 2,temp->position.area.height / 2}, temp->angle, (Color) {temp->color.r,temp->color.g,temp->color.b,temp->opacity});
        /*
        DrawCircle(temp->topLeftAnchor.x, temp->topLeftAnchor.y, 4, GREEN);
        DrawCircle(temp->topRightAnchor.x, temp->topRightAnchor.y, 4, GREEN);
        DrawCircle(temp->bottomLeftAnchor.x, temp->bottomLeftAnchor.y, 4, GREEN);
        DrawCircle(temp->bottomRightAnchor.x, temp->bottomRightAnchor.y, 4, GREEN);
        */
    }  
    
       /* 
    for(int i = 0; i < allObjects->size; i++) 
    {
        Object *temp = allObjects->items[i];
        
        DrawCircle(temp->topLeftAnchor.x, temp->topLeftAnchor.y, 4, GOLD);
        DrawCircle(temp->topRightAnchor.x, temp->topRightAnchor.y, 4, GOLD);
        DrawCircle(temp->bottomLeftAnchor.x, temp->bottomLeftAnchor.y, 4, GOLD);
        DrawCircle(temp->bottomRightAnchor.x, temp->bottomRightAnchor.y, 4, GOLD);
        
    }
    */   
}

void recalculateAllUnits(double newUnitSize, double oldUnitSize) {

    double difference = newUnitSize / oldUnitSize;
    if (allObjects != NULL) {

        for (int i = 0; i < allObjects->size; i++) {

            Object *object = allObjects->items[i];
            
            object->position.anchors[ANCHOR_ORIGIN].x *= difference;
            object->position.anchors[ANCHOR_ORIGIN].y *= difference;
            object->position.area.width *= difference;
            object->position.area.height *= difference; 

            updatePositionAnchors(&object->position, ANCHOR_ORIGIN);
            object->position.area.x = object->position.anchors[ANCHOR_ORIGIN].x;
            object->position.area.y = object->position.anchors[ANCHOR_ORIGIN].y;
        }
    }

    if (allSprites != NULL) {

        for (int i = 0; i < allSprites->size; i++) {

            Sprite *sprite = allSprites->items[i];
            sprite->position.anchors[ANCHOR_ORIGIN].x *= difference;
            sprite->position.anchors[ANCHOR_ORIGIN].y *= difference;
            sprite->position.area.width *= difference;
            sprite->position.area.height *= difference;

            updatePositionAnchors(&sprite->position, ANCHOR_ORIGIN);
            sprite->position.area.x = sprite->position.anchors[ANCHOR_ORIGIN].x;
            sprite->position.area.y = sprite->position.anchors[ANCHOR_ORIGIN].y;
        }
    }

    if (UNIT_PIXEL_SIZE > 0) {

        double scale = newUnitSize / UNIT_PIXEL_SIZE;

        camera.camera.target.x *= scale;
        camera.camera.target.y *= scale;
        
        recalculateCameraAnchors();
    }
}

void freeObjects() {

    dynarray_free(allObjects);
    free(allObjects);
    allObjects = NULL;
}

void freeSprites() {

    dynarray_free(allSprites);
    free(allSprites);
    allSprites = NULL;
}

int returnObjectAmount() {

    return allObjects->size; // min >> max
}
