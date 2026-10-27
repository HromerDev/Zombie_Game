#ifndef OBJECT_H
#define OBJECT_H
#include "raylib.h"

extern Texture2D textureArray[10];
extern Sound sounds[10];
typedef struct 
{    
    Vector2 anchors[5];
    Rectangle textureArea;
    Rectangle originalArea;
    Color color;
    unsigned short opacity;
    unsigned int texture;
    double angle;
    unsigned int layer;
} Sprite;

typedef struct  
{
    Vector2 anchors[5];
    Vector2 spriteOffset;
    Rectangle originalCollider;
    Rectangle collider;
    Sprite *sprite;
    double angle;
} Object;

enum Sounds {

    SOUND_DEATH,
    SOUND_SCORE,
};
enum Textures {

    TEXTURE_AAAA,
    TEXTURE_DEBUG,
    TEXTURE_CURSOR,
};

enum ObjectLayers {

    LAYER_TILES,
    LAYER_CURSOR,
    LAYER_UI,
};

enum Anchors {

    ANCHOR_ORIGIN,
    ANCHOR_TOP_RIGHT,
    ANCHOR_BOTTOM_LEFT,
    ANCHOR_BOTTOM_RIGHT,
    ANCHOR_CENTER,
};

void initTextures();
Object* createObject(Rectangle collider, Sprite *sprite);
Sprite* createSprite(Rectangle area, unsigned int texture, unsigned int layer);

void destroyObject(Object *object);
void destroySprite(Sprite *sprite);
void updateObjectAnchors(Object *object, int anchor);
void updateSpriteAnchors(Sprite *sprite, int anchor);
void moveObject(Object *object, float x, float y, int anchor);
void moveObjectRelative(Object *object, float x, float y, int anchor);
void moveSprite(Sprite *sprite, float x, float y, int anchor);
void moveSpriteRelative(Sprite *sprite, float x, float y, int anchor);
void drawSprites();
//void allObjectsLayerSort(); 
void recalculateAllUnits(double newUnitSize, double oldUnitSize);
void freeObjects();
void freeSprites();
int returnObjectAmount();
bool layersSortedCorrectly();

#endif
