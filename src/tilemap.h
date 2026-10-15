#ifndef TILEMAP_H
#define TILEMAP_H
#include "object.h"
#define TILE_SIZE 4


enum {

    TILE_DEBUG,
    TILE_GRASS,
};

typedef struct {

    Sprite* sprite;
} Tile;

typedef struct {

    char *mapName;
    unsigned short* mapData;
    unsigned short width;
    unsigned short height;
} Map;

extern Map CURRENTLY_LOADED_MAP; 

void initTiles();
void loadMap(char* mapName);
void drawMap();
void freeMapData();
#endif 
