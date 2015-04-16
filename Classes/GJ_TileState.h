//
//  GJ_MapState.h
//  Gravity_Jam
//
//  Created by Garry Kling on 10/20/14.
//
//

#ifndef __Gravity_Jam__GJ_MapState__
#define __Gravity_Jam__GJ_MapState__

#include <vector>
#include "cocos2d.h"

USING_NS_CC;

enum Colors {
    kNoColor,
    kBlue,
    kRed,
    kGreen,
    kYellow,
    kOrange
};

// the state of each tile on the map at its properties

struct TileState {
    
    Sprite * spritePtr;
    
    Vec2 coordinates;
    int color;
    bool occupied;
    bool isStop;
    
    // for union find
    TileState * parent;
    int rank;
    bool markedForDelete;
    bool deleted;
    bool moveInProgress;
};

typedef struct TileState TileState;


class MapState {

public:
    MapState();

    int sizeX, sizeY, numTiles, tileWidth, tileHeight;
    
    bool initMap(Size mapSize, int tileWidth, int tileHeight);
    void addTile(Vec2 loc);
    bool isLocInRange(Vec2 loc);
    
    void setTileOccupied(Vec2 loc);
    void setTileNotOccupied(Vec2 loc);
    bool isTileOccupied(Vec2 loc);
    
    void setSpriteForTile(Vec2 loc, Sprite * newSprite);
    Sprite * getSpriteForTile(Vec2 loc);
    
    void setTileStop(Vec2 loc);
    void setTileNotStop(Vec2 loc);
    bool isTileStop(Vec2 loc);
    
    void setTileColor(Vec2 loc, int color);
    void clearTileColor(Vec2 loc);
    int  getTileColor(Vec2 loc);
    
    void setTileMoving(Vec2 loc);
    void setTileNotMoving(Vec2 loc);
    bool isTileMoving(Vec2 loc);
    
    void setTileState(Vec2 loc, bool occupy, bool stop);
    
    // utility
    void moveTileState(Vec2 fromLoc, Vec2 toLoc);
    // void copyTileState(Vec2 fromLoc, Vec2 toLoc);
    void clearTileState(Vec2 loc);
    bool checkTileToDeleteMark(Vec2 loc);
    void setAsDeleted(Vec2 loc);
    bool wasTileDeleted(Vec2 loc);
    Vec2 convertPxToCoord(float x, float y);
    void printState(void);
    
    // disjoint set methods
    TileState * makeSet(Vec2 loc);
    TileState * findSet(TileState * loc);
    void unionSets(TileState * setA, TileState * setB);
    void connectComponents(void);
    
private:
    
    bool setMapSize(int x, int y);
    bool mapValid();
    void clearMap();
    
    TileState createTileState(Vec2 loc, bool occupy, bool stop);
    std::vector<std::vector<TileState>> mapState;
       
    
};

#endif /* defined(__Gravity_Jam__GJ_TileState__) */
