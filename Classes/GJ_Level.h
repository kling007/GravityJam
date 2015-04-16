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
#include "GJ_Victory.h"

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
    static cocos2d::Scene* createScene();
    virtual bool init();
    
    // Touch handling
    bool setupTouches();
    int getTouchDirection();
    EventListenerTouchOneByOne * touchListener;
    Vec2 touchBegin;
    Vec2 touchEnd;
    
    // tile creation
    bool createPuzzleTiles();
    bool createPassiveTiles();
    
    // level mgmt
    int currentLevel();
    bool setlevel(int newLevel);
    bool reloadLevel(Layer * activeLayer);
    bool loadLevel(int levelNum);
    bool unloadLevel();
    void closeLevel(void);
    bool nextLevel(void);
    void endLevel(float dt);
    void endVictoryScreen(float dt);
   
    // position - should some/all of this be pushed into MapState class?
    Vec2 tileCoordForPosition(Vec2 position);
    Vec2 getPxforCoord(Vec2 inCoord);
    Vec2 getAdjacentPxCoord(Vec2 inCoord, int direction);
    Vec2 getAdjacentCoord(Vec2 inCoord, int direction);
    
    // moving tiles
    bool createPuzzleTileMove(Sprite * theTile, int direction);
    void setActionForPuzzleTile(Sprite * theTile, Action * theAction);
    void moveTiles(int dir);
    void update(float dt);
    void endOfMoveChecks(int dir);
    bool isCurrentLevelComplete(void);
    
    // instance vars
    int curLevel;
    int numTiles;
    int curDirection;
    float posX, posY, tm_scale;
    Size parentVisibleSize;
    Vec2 parentOrigin;
    bool movesDone;
    int  movesQueued;
    bool levelComplete;
    bool faulted;
    float timeElapsed;
    
    // Cocos2d macro
    CREATE_FUNC(Level);
    
private:
    // tileMap related
    cocos2d::TMXTiledMap * tileMap;
    cocos2d::TMXLayer * background;
    cocos2d::TMXLayer * metaLayer;
    
    // MapState
    MapState * theMap;
    Victory * victoryScreen;
};

#endif /* defined(__Gravity_Jam__GJ_Level__) */
