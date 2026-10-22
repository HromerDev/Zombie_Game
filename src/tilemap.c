#include "../scripts/game_manager.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "tilemap.h"
#include "raylib.h"
#include "raymath.h"
#include "camera_handler.h"
#include <time.h>
Tile tiles[10];
unsigned short* loadedMap;

Map CURRENTLY_LOADED_MAP; 

void initTiles() {

    tiles[TILE_GRASS].sprite = createSprite((Rectangle){0,0, TILE_SIZE, TILE_SIZE}, TEXTURE_DEBUG, LAYER_TILES, 0);
    tiles[TILE_GRASS].sprite->opacity = 0;

    CURRENTLY_LOADED_MAP.mapData = NULL;
}

void loadMap(char* mapName) {

    if(CURRENTLY_LOADED_MAP.mapData != NULL)
        freeMapData();

    FILE* map = fopen(TextFormat("maps/%s", mapName), "r");

    if(map == NULL)
        return;

    char buffer[2048];
    char* dimensions = fgets(buffer, 2048, map);

    CURRENTLY_LOADED_MAP.mapName = strdup(mapName);

    CURRENTLY_LOADED_MAP.width = atoi(strtok(dimensions, "x"));
    CURRENTLY_LOADED_MAP.height = atoi(strtok(NULL, "\r\n"));

    CURRENTLY_LOADED_MAP.mapData = malloc(CURRENTLY_LOADED_MAP.width * CURRENTLY_LOADED_MAP.height * sizeof(unsigned short)); 
    
    int i = 0;
    while(fgets(buffer, 2048, map) != NULL) {
        
        char* token = strtok(buffer, ",\r\n");
        while (token != NULL) {
            
            CURRENTLY_LOADED_MAP.mapData[i++] = atoi(token);
            token = strtok(NULL, ",\r\n");
        }
    }

    fclose(map);
}

void drawMap() {
    
    if(CURRENTLY_LOADED_MAP.mapData == NULL)
        return;

    int tileSize = TILE_SIZE * UNIT_PIXEL_SIZE;
    int mapWidth = CURRENTLY_LOADED_MAP.width;
    int mapHeight = CURRENTLY_LOADED_MAP.height;
    
    int cameraLeftSide = camera.target.x;
    int cameraRightSide = camera.target.x + SCREENWIDTH;
    int cameraTopSide = camera.target.y;
    int cameraBottomSide = camera.target.y + SCREENHEIGHT;
    
    int drawStartX = cameraLeftSide / tileSize;
    int drawStartY = cameraTopSide / tileSize;

    int drawEndX = (cameraRightSide + tileSize - 1) / tileSize;
    int drawEndY = (cameraBottomSide + tileSize - 1) / tileSize;

    drawStartX = Clamp(drawStartX, 0, mapWidth);
    drawStartY = Clamp(drawStartY, 0, mapHeight);
    drawEndX = Clamp(drawEndX, 0, mapWidth);
    drawEndY = Clamp(drawEndY, 0, mapHeight);
    
    for(int y = drawStartY; y < drawEndY; y++) {

        for(int x = drawStartX; x < drawEndX; x++) {

            int mapDataIndex = y * mapWidth + x;

            DrawTextureEx(textureArray[CURRENTLY_LOADED_MAP.mapData[mapDataIndex]], (Vector2){x * tileSize, y * tileSize}, 0, (float)tileSize / textureArray[CURRENTLY_LOADED_MAP.mapData[mapDataIndex]].width, WHITE);
        }
    }
}

void freeMapData() 
{
    free(CURRENTLY_LOADED_MAP.mapData);
    CURRENTLY_LOADED_MAP.mapData = NULL;
}
