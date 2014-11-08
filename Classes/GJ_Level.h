//
//  GJ_Level.h
//  Gravity_Jam
//
//  Created by Garry Kling on 10/16/14.
//
//

/*
    Managing Levels/Sprites
    --
    Levels are designed in Tiled and saved as TMX maps
    
    There are two types of tiles, passive and active. Active tiles are the tiles cleared by the player.
    Passive tiles are not clearable, generally barriers and have puzzl-e uses. There is a vector for each. 

    Tiles are loaded onto the active layer by passing the level number and a Layer* to loadLevel();
    This implementation is brittle; a smarter method/class could place them in a layer with arbitrary size.
*/

#ifndef __Gravity_Jam__GJ_Level__
#define __Gravity_Jam__GJ_Level__

#include "cocos2d.h"
#include "GJ_TileState.h"

USING_NS_CC;

#define NO_DIRECTION 0
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

class Level : public cocos2d::Layer
{
    
public:
    // standard methods to implement
    virtual bool init();
    
    // tile management
    bool clearAllTiles();
    bool clearPuzzleTiles();
    bool clearPassiveTiles();
    bool createPuzzleTiles();
    bool createPassiveTiles();
    
    // levels
    bool loadLevel(int levelNum, Layer * activeLayer);
    bool unloadLevel();
    
   
    // movement
    Vec2 tileCoordForPosition(Vec2 position);
    Sprite * getTileForCoord(Vec2 positionCoord);
    Vec2 getPxforCoord(Vec2 inCoord);
    Vec2 getAdjacentPxCoord(Vec2 inCoord, int direction);
    Vec2 getAdjacentCoord(Vec2 inCoord, int direction);
    bool createPuzzleTileMove(Sprite * theTile, int direction);
    void setActionForPuzzleTile(Sprite * theTile, Action * theAction);
    bool checkForTileGroups(void);
    void moveTiles(int dir);
    
    // instance vars
    int curLevel;
    int numTiles;
    float posX, posY, tm_scale;
    Size parentVisibleSize;
    Vec2 parentOrigin;
    
    // visible to the Director, where the nodes are put into the mix
    Layer * theLayer;
    
    // Cocos2d macro
    CREATE_FUNC(Level);
    
private:
    // tileMap related
    cocos2d::TMXTiledMap * tileMap;
    cocos2d::TMXLayer * background;
    cocos2d::TMXLayer * metaLayer;
    
    // MapState
    MapState * theMap;
    
    // the cache of currently active "game" tileSprites
    // these are the ones that the player is trying to get rid of
    std::vector<Sprite*> puzzleTiles;
    std::vector<Sprite*> passiveTiles;
  
};

#endif /* defined(__Gravity_Jam__GJ_Level__) */
