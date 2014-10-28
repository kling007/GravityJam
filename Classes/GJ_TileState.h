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

// the state of each tile on the map at its properties
// perhaps properties should be a hash? seems like that is a bit heavyweight

struct TileState {
    Vec2 coordinates;
    bool occupied;
    bool isStop;
};

typedef struct TileState TileState;

class MapState {

public:
    MapState();
    bool mapFull;
    int sizeX, sizeY, numTiles;
    
    // global operations
    bool initMap(Size mapSize);
    bool addTile(Vec2 loc);
    
    void setTileOccupied(Vec2 loc);
    void setTileNotOccupied(Vec2 loc);
    bool isTileOccupied(Vec2 loc);
    
    void setTileStop(Vec2 loc);
    void setTileNotStop(Vec2 loc);
    bool isTileStop(Vec2 loc);
    
    void setTileLocation(Vec2 loc);
    
    void setTileState(Vec2 loc, bool occupy, bool stop);
    
    bool isLocInRange(Vec2 loc);
    
private:
    
    // manage tiles
    bool setMapSize(int x, int y);
    bool mapValid();
    void clearMap();
    TileState createTileState(Vec2 loc, bool occupy, bool stop);
    
    std::vector<std::vector<TileState>> mapState;
    
};

#endif /* defined(__Gravity_Jam__GJ_TileState__) */
