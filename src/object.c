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
    //textureArray[TEXTURE_PLAYER] = LoadTexture("assets/player.png");
    //textureArray[TEXTURE_PIPE1] = LoadTexture("assets/pipe1.png");
    //textureArray[TEXTURE_PIPE2] = LoadTexture("assets/pipe2.png");
    //textureArray[TEXTURE_PARTICLE] = LoadTexture("assets/particle4.png");

    //sounds[SOUND_DEATH] = LoadSound("assets/death.wav");
    //sounds[SOUND_SCORE] = LoadSound("assets/score.wav");
}

Object* createObject(Rectangle collider, Sprite *sprite) {

    Object *object = malloc(sizeof(Object));
    object->originalCollider = collider;

    object->collider = (Rectangle){collider.x * UNIT_PIXEL_SIZE, collider.y * UNIT_PIXEL_SIZE, collider.width * UNIT_PIXEL_SIZE, collider.height * UNIT_PIXEL_SIZE};

    object->anchors[ANCHOR_ORIGIN].x = collider.x;
    object->anchors[ANCHOR_ORIGIN].y = collider.y;

    object->sprite = sprite;
    object->angle = 0;
    
    object->spriteOffset = (Vector2){0,0};

    updateObjectAnchors(object, ANCHOR_ORIGIN);

    moveSprite(object->sprite, object->anchors[ANCHOR_CENTER].x + object->spriteOffset.x, object->anchors[ANCHOR_CENTER].y + object->spriteOffset.y, ANCHOR_CENTER);  
    dynarray_push(allObjects, object);
    
    return object;
}

Sprite* createSprite(Rectangle textureArea, unsigned int texture, unsigned int layer) {

    Sprite* sprite = malloc(sizeof(Sprite));    
    sprite->originalArea = textureArea;
    sprite->color = WHITE;
    
    sprite->textureArea = (Rectangle) {
        
        textureArea.x * UNIT_PIXEL_SIZE,
        textureArea.y * UNIT_PIXEL_SIZE,
        textureArea.width * UNIT_PIXEL_SIZE,
        textureArea.height * UNIT_PIXEL_SIZE
    };

    sprite->texture = texture; 
    sprite->layer = layer;
    sprite->angle = 0;
    sprite->opacity = 255;

    updateSpriteAnchors(sprite, ANCHOR_ORIGIN);
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

void updateObjectAnchors(Object *object, int anchor) {

    anchor = (anchor <= ANCHOR_CENTER) ? anchor : ANCHOR_ORIGIN;

    switch(anchor) {
        
        case ANCHOR_ORIGIN:

            object->anchors[ANCHOR_TOP_RIGHT].x = object->anchors[ANCHOR_ORIGIN].x + object->collider.width;
            object->anchors[ANCHOR_TOP_RIGHT].y = object->anchors[ANCHOR_ORIGIN].y;

            object->anchors[ANCHOR_BOTTOM_LEFT].x = object->anchors[ANCHOR_ORIGIN].x;
            object->anchors[ANCHOR_BOTTOM_LEFT].y = object->anchors[ANCHOR_ORIGIN].y + object->collider.height;

            object->anchors[ANCHOR_BOTTOM_RIGHT].x = object->anchors[ANCHOR_BOTTOM_LEFT].x + object->collider.width;
            object->anchors[ANCHOR_BOTTOM_RIGHT].y = object->anchors[ANCHOR_BOTTOM_LEFT].y;

            object->anchors[ANCHOR_CENTER].x = object->anchors[ANCHOR_ORIGIN].x + object->collider.width / 2;
            object->anchors[ANCHOR_CENTER].y = object->anchors[ANCHOR_ORIGIN].y + object->collider.height / 2;
            break;

        case ANCHOR_TOP_RIGHT:

            object->anchors[ANCHOR_ORIGIN].x = object->anchors[ANCHOR_TOP_RIGHT].x - object->collider.width;
            object->anchors[ANCHOR_ORIGIN].y = object->anchors[ANCHOR_TOP_RIGHT].y;
            
            object->anchors[ANCHOR_BOTTOM_LEFT].x = object->anchors[ANCHOR_ORIGIN].x;
            object->anchors[ANCHOR_BOTTOM_LEFT].y = object->anchors[ANCHOR_ORIGIN].y + object->collider.height;

            object->anchors[ANCHOR_BOTTOM_RIGHT].x = object->anchors[ANCHOR_BOTTOM_LEFT].x + object->collider.width;
            object->anchors[ANCHOR_BOTTOM_RIGHT].y = object->anchors[ANCHOR_BOTTOM_LEFT].y;

            object->anchors[ANCHOR_CENTER].x = object->anchors[ANCHOR_ORIGIN].x + object->collider.width / 2;
            object->anchors[ANCHOR_CENTER].y = object->anchors[ANCHOR_ORIGIN].y + object->collider.height / 2;   
            break;

        case ANCHOR_BOTTOM_LEFT:

            object->anchors[ANCHOR_ORIGIN].x = object->anchors[ANCHOR_BOTTOM_LEFT].x;
            object->anchors[ANCHOR_ORIGIN].y = object->anchors[ANCHOR_BOTTOM_LEFT].y - object->collider.height;

            object->anchors[ANCHOR_TOP_RIGHT].x = object->anchors[ANCHOR_ORIGIN].x + object->collider.width;
            object->anchors[ANCHOR_TOP_RIGHT].y = object->anchors[ANCHOR_ORIGIN].y;
            
            object->anchors[ANCHOR_BOTTOM_RIGHT].x = object->anchors[ANCHOR_BOTTOM_LEFT].x + object->collider.width;
            object->anchors[ANCHOR_BOTTOM_RIGHT].y = object->anchors[ANCHOR_BOTTOM_LEFT].y;

            object->anchors[ANCHOR_CENTER].x = object->anchors[ANCHOR_ORIGIN].x + object->collider.width / 2;
            object->anchors[ANCHOR_CENTER].y = object->anchors[ANCHOR_ORIGIN].y + object->collider.height / 2;   
            
            break;

        case ANCHOR_BOTTOM_RIGHT:

            object->anchors[ANCHOR_ORIGIN].x = object->anchors[ANCHOR_BOTTOM_RIGHT].x - object->collider.width;
            object->anchors[ANCHOR_ORIGIN].y = object->anchors[ANCHOR_BOTTOM_RIGHT].y - object->collider.height;

            object->anchors[ANCHOR_TOP_RIGHT].x = object->anchors[ANCHOR_ORIGIN].x + object->collider.width;
            object->anchors[ANCHOR_TOP_RIGHT].y = object->anchors[ANCHOR_ORIGIN].y;

            object->anchors[ANCHOR_BOTTOM_LEFT].x = object->anchors[ANCHOR_ORIGIN].x;
            object->anchors[ANCHOR_BOTTOM_LEFT].y = object->anchors[ANCHOR_ORIGIN].y + object->collider.height;

            object->anchors[ANCHOR_CENTER].x = object->anchors[ANCHOR_ORIGIN].x + object->collider.width / 2;
            object->anchors[ANCHOR_CENTER].y = object->anchors[ANCHOR_ORIGIN].y + object->collider.height / 2;   

        case ANCHOR_CENTER:
            object->anchors[ANCHOR_ORIGIN].x = object->anchors[ANCHOR_CENTER].x - object->collider.width / 2;
            object->anchors[ANCHOR_ORIGIN].y = object->anchors[ANCHOR_CENTER].y - object->collider.height / 2;

            object->anchors[ANCHOR_TOP_RIGHT].x = object->anchors[ANCHOR_ORIGIN].x + object->collider.width;
            object->anchors[ANCHOR_TOP_RIGHT].y = object->anchors[ANCHOR_ORIGIN].y;
            
            object->anchors[ANCHOR_BOTTOM_LEFT].x = object->anchors[ANCHOR_ORIGIN].x;
            object->anchors[ANCHOR_BOTTOM_LEFT].y = object->anchors[ANCHOR_ORIGIN].y + object->collider.height;
            
            object->anchors[ANCHOR_BOTTOM_RIGHT].x = object->anchors[ANCHOR_BOTTOM_LEFT].x + object->collider.width;
            object->anchors[ANCHOR_BOTTOM_RIGHT].y = object->anchors[ANCHOR_BOTTOM_LEFT].y;
            break;
    }
}

void updateSpriteAnchors(Sprite *sprite, int anchor) {
   
    anchor = (anchor <= ANCHOR_CENTER) ? anchor : ANCHOR_ORIGIN;

    switch(anchor) {
        
        case ANCHOR_ORIGIN:

            sprite->anchors[ANCHOR_TOP_RIGHT].x = sprite->anchors[ANCHOR_ORIGIN].x + sprite->textureArea.width;
            sprite->anchors[ANCHOR_TOP_RIGHT].y = sprite->anchors[ANCHOR_ORIGIN].y;

            sprite->anchors[ANCHOR_BOTTOM_LEFT].x = sprite->anchors[ANCHOR_ORIGIN].x;
            sprite->anchors[ANCHOR_BOTTOM_LEFT].y = sprite->anchors[ANCHOR_ORIGIN].y + sprite->textureArea.height;

            sprite->anchors[ANCHOR_BOTTOM_RIGHT].x = sprite->anchors[ANCHOR_BOTTOM_LEFT].x + sprite->textureArea.width;
            sprite->anchors[ANCHOR_BOTTOM_RIGHT].y = sprite->anchors[ANCHOR_BOTTOM_LEFT].y;

            sprite->anchors[ANCHOR_CENTER].x = sprite->anchors[ANCHOR_ORIGIN].x + sprite->textureArea.width / 2;
            sprite->anchors[ANCHOR_CENTER].y = sprite->anchors[ANCHOR_ORIGIN].y + sprite->textureArea.height / 2;
            break;

        case ANCHOR_TOP_RIGHT:

            sprite->anchors[ANCHOR_ORIGIN].x = sprite->anchors[ANCHOR_TOP_RIGHT].x - sprite->textureArea.width;
            sprite->anchors[ANCHOR_ORIGIN].y = sprite->anchors[ANCHOR_TOP_RIGHT].y;
            
            sprite->anchors[ANCHOR_BOTTOM_LEFT].x = sprite->anchors[ANCHOR_ORIGIN].x;
            sprite->anchors[ANCHOR_BOTTOM_LEFT].y = sprite->anchors[ANCHOR_ORIGIN].y + sprite->textureArea.height;

            sprite->anchors[ANCHOR_BOTTOM_RIGHT].x = sprite->anchors[ANCHOR_BOTTOM_LEFT].x + sprite->textureArea.width;
            sprite->anchors[ANCHOR_BOTTOM_RIGHT].y = sprite->anchors[ANCHOR_BOTTOM_LEFT].y;

            sprite->anchors[ANCHOR_CENTER].x = sprite->anchors[ANCHOR_ORIGIN].x + sprite->textureArea.width / 2;
            sprite->anchors[ANCHOR_CENTER].y = sprite->anchors[ANCHOR_ORIGIN].y + sprite->textureArea.height / 2;   
            break;

        case ANCHOR_BOTTOM_LEFT:

            sprite->anchors[ANCHOR_ORIGIN].x = sprite->anchors[ANCHOR_BOTTOM_LEFT].x;
            sprite->anchors[ANCHOR_ORIGIN].y = sprite->anchors[ANCHOR_BOTTOM_LEFT].y - sprite->textureArea.height;

            sprite->anchors[ANCHOR_TOP_RIGHT].x = sprite->anchors[ANCHOR_ORIGIN].x + sprite->textureArea.width;
            sprite->anchors[ANCHOR_TOP_RIGHT].y = sprite->anchors[ANCHOR_ORIGIN].y;
            
            sprite->anchors[ANCHOR_BOTTOM_RIGHT].x = sprite->anchors[ANCHOR_BOTTOM_LEFT].x + sprite->textureArea.width;
            sprite->anchors[ANCHOR_BOTTOM_RIGHT].y = sprite->anchors[ANCHOR_BOTTOM_LEFT].y;

            sprite->anchors[ANCHOR_CENTER].x = sprite->anchors[ANCHOR_ORIGIN].x + sprite->textureArea.width / 2;
            sprite->anchors[ANCHOR_CENTER].y = sprite->anchors[ANCHOR_ORIGIN].y + sprite->textureArea.height / 2;   
            
            break;

        case ANCHOR_BOTTOM_RIGHT:

            sprite->anchors[ANCHOR_ORIGIN].x = sprite->anchors[ANCHOR_BOTTOM_RIGHT].x - sprite->textureArea.width;
            sprite->anchors[ANCHOR_ORIGIN].y = sprite->anchors[ANCHOR_BOTTOM_RIGHT].y - sprite->textureArea.height;

            sprite->anchors[ANCHOR_TOP_RIGHT].x = sprite->anchors[ANCHOR_ORIGIN].x + sprite->textureArea.width;
            sprite->anchors[ANCHOR_TOP_RIGHT].y = sprite->anchors[ANCHOR_ORIGIN].y;

            sprite->anchors[ANCHOR_BOTTOM_LEFT].x = sprite->anchors[ANCHOR_ORIGIN].x;
            sprite->anchors[ANCHOR_BOTTOM_LEFT].y = sprite->anchors[ANCHOR_ORIGIN].y + sprite->textureArea.height;

            sprite->anchors[ANCHOR_CENTER].x = sprite->anchors[ANCHOR_ORIGIN].x + sprite->textureArea.width / 2;
            sprite->anchors[ANCHOR_CENTER].y = sprite->anchors[ANCHOR_ORIGIN].y + sprite->textureArea.height / 2;   

        case ANCHOR_CENTER:
            sprite->anchors[ANCHOR_ORIGIN].x = sprite->anchors[ANCHOR_CENTER].x - sprite->textureArea.width / 2;
            sprite->anchors[ANCHOR_ORIGIN].y = sprite->anchors[ANCHOR_CENTER].y - sprite->textureArea.height / 2;

            sprite->anchors[ANCHOR_TOP_RIGHT].x = sprite->anchors[ANCHOR_ORIGIN].x + sprite->textureArea.width;
            sprite->anchors[ANCHOR_TOP_RIGHT].y = sprite->anchors[ANCHOR_ORIGIN].y;
            
            sprite->anchors[ANCHOR_BOTTOM_LEFT].x = sprite->anchors[ANCHOR_ORIGIN].x;
            sprite->anchors[ANCHOR_BOTTOM_LEFT].y = sprite->anchors[ANCHOR_ORIGIN].y + sprite->textureArea.height;
            
            sprite->anchors[ANCHOR_BOTTOM_RIGHT].x = sprite->anchors[ANCHOR_BOTTOM_LEFT].x + sprite->textureArea.width;
            sprite->anchors[ANCHOR_BOTTOM_RIGHT].y = sprite->anchors[ANCHOR_BOTTOM_LEFT].y;
            break;
    }
}

void moveSprite(Sprite *sprite, float x, float y, int anchor) {
   
    anchor = (anchor <= ANCHOR_CENTER) ? anchor : ANCHOR_ORIGIN;

    sprite->anchors[anchor].x = x;
    sprite->anchors[anchor].y = y;
            
    updateSpriteAnchors(sprite, anchor);
}

void moveSpriteRelative(Sprite *sprite, float x, float y, int anchor) {

    anchor = (anchor <= ANCHOR_CENTER) ? anchor : ANCHOR_ORIGIN;

    sprite->anchors[anchor].x += x * UNIT_PIXEL_SIZE;
    sprite->anchors[anchor].y += y * UNIT_PIXEL_SIZE;

    updateSpriteAnchors(sprite, anchor);


    sprite->textureArea.x = sprite->anchors[ANCHOR_ORIGIN].x;
    sprite->textureArea.y = sprite->anchors[ANCHOR_ORIGIN].y;
}

void moveObject(Object *object, float x, float y, int anchor) {

    object->anchors[anchor].x = x * UNIT_PIXEL_SIZE;
    object->anchors[anchor].y = y * UNIT_PIXEL_SIZE;

    updateObjectAnchors(object, anchor);

    object->collider.x = object->anchors[ANCHOR_ORIGIN].x;
    object->collider.y = object->anchors[ANCHOR_ORIGIN].y;

    if(object->sprite != NULL)     
        moveSprite(object->sprite, object->anchors[ANCHOR_CENTER].x + object->spriteOffset.x, object->anchors[ANCHOR_CENTER].y + object->spriteOffset.y, ANCHOR_CENTER);
    
}

void moveObjectRelative(Object *object, float x, float y, int anchor) {

    object->anchors[anchor].x += x * UNIT_PIXEL_SIZE;
    object->anchors[anchor].y += y * UNIT_PIXEL_SIZE;

    updateObjectAnchors(object, anchor);

    if(object->sprite != NULL)     
        moveSprite(object->sprite, object->anchors[ANCHOR_CENTER].x + object->spriteOffset.x, object->anchors[ANCHOR_CENTER].y + object->spriteOffset.y, ANCHOR_CENTER);
}

void drawSprites() {
    
    for(int i = 0; i < allSprites->size; i++) {

        Sprite *temp = allSprites->items[i];
        
        DrawTexturePro(textureArray[temp->texture], (Rectangle){0,0, textureArray[temp->texture].width, textureArray[temp->texture].height}, (Rectangle) {temp->anchors[ANCHOR_CENTER].x , temp->anchors[ANCHOR_CENTER].y, temp->textureArea.width, temp->textureArea.height}, (Vector2){temp->textureArea.width / 2,temp->textureArea.height / 2}, temp->angle, (Color) {temp->color.r,temp->color.g,temp->color.b,temp->opacity});
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
            
            object->anchors[ANCHOR_ORIGIN].x *= difference;
            object->anchors[ANCHOR_ORIGIN].y *= difference;
            object->collider.width *= difference;
            object->collider.height *= difference; 

            updateObjectAnchors(object, ANCHOR_ORIGIN);
            object->collider.x = object->anchors[ANCHOR_ORIGIN].x;
            object->collider.y = object->anchors[ANCHOR_ORIGIN].y;
        }
    }

    if (allSprites != NULL) {

        for (int i = 0; i < allSprites->size; i++) {

            Sprite *sprite = allSprites->items[i];
            sprite->anchors[ANCHOR_ORIGIN].x *= difference;
            sprite->anchors[ANCHOR_ORIGIN].y *= difference;
            sprite->textureArea.width *= difference;
            sprite->textureArea.height *= difference;

            updateSpriteAnchors(sprite, ANCHOR_ORIGIN);
            sprite->textureArea.x = sprite->anchors[ANCHOR_ORIGIN].x;
            sprite->textureArea.y = sprite->anchors[ANCHOR_ORIGIN].y;
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
