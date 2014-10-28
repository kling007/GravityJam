//
//  GJ_MapState.cpp
//  Gravity_Jam
//
//  Created by Garry Kling on 10/20/14.
//
//

#include "GJ_TileState.h"


MapState::MapState()
{
    mapFull = false;
    sizeX = 0;
    sizeY = 0;
    numTiles = 0;
    TileState t = {Vec2(0,0), false, false};
    std::vector<TileState> tileVec(1);
    tileVec.push_back(t);
    mapState.push_back(tileVec);
    
}

bool MapState::initMap(Size mapSize)
{
   // if we call initMap(), we should expect it to be cleared
    setMapSize(mapSize.width, mapSize.height);

    mapState = std::vector<std::vector<TileState>>(mapSize.height, std::vector<TileState>(mapSize.width));
   //if(mapFull||!mapState[0].empty()){ clearMap(); }
    
  
    for(int i = 0; i< mapSize.height; i++)
    {
        for (int j = 0; j<mapSize.width; j++)
        {
            setTileState(Vec2(i,j), false, false);
        }
    }
    
    return true;
}

bool MapState::setMapSize(int x, int y)
{
    // very basic check - is the map in use or is the size stupid
    // if(mapFull || (x<1 || y<1)){ return false; }
    
    sizeX = x;
    sizeY = y;
    numTiles = x*y;
    mapFull = true;
    
    // see if we need to empty first - mapFull is just to keep honest ppl honest
    // reserve

    for(int i=0; i<sizeY; i++)
    {
//        if(!mapState[i].empty())
//        {
//            mapState[i].clear();
//        }
//        
//        mapState[i].reserve(numTiles);
    }

    
    return true;
}

bool MapState::addTile(Vec2 loc)
{
    TileState * temp = &mapState[loc.y][loc.x];

    if (temp) {
        //temp->coordinates.set(loc);
        temp->occupied = false;
        temp->isStop = false;
    }
    else
    {
        auto newTile = createTileState(loc, false, false);
        mapState[loc.y][loc.x] = newTile;
    }

}

bool MapState::mapValid()
{
    return mapFull;
}

void MapState::clearMap()
{
    mapFull = false;
    
    if (sizeY>0)
    {
        for(int i=0; i<sizeY; i++)
        {
            if(!mapState[i].empty())
            {
                mapState[i].clear();
            }
            
        }
    }
    
    numTiles = sizeX = sizeY = 0;
}

TileState MapState::createTileState(Vec2 loc, bool occupy, bool stop)
{
    TileState newTile;
    
    //newTile.coordinates = Vec2(loc);
    newTile.occupied = occupy;
    newTile.isStop = stop;
    
    return newTile;
}

void MapState::setTileOccupied(Vec2 loc)
{
    mapState[loc.y][loc.x].occupied = true;
}

void MapState::setTileNotOccupied(Vec2 loc)
{
    mapState[loc.y][loc.x].occupied = false;
}

bool MapState::isTileOccupied(Vec2 loc)
{
    return mapState[loc.y][loc.x].occupied;
}

void MapState::setTileStop(Vec2 loc)
{
    mapState[loc.y][loc.x].isStop = true;
}

void MapState::setTileNotStop(Vec2 loc)
{
    mapState[loc.y][loc.x].isStop = false;
}

bool MapState::isTileStop(Vec2 loc)
{
    return mapState[loc.y][loc.x].isStop;
}

void MapState::setTileState(Vec2 loc, bool occupy, bool stop)
{
    int i = loc.y;
    int j = loc.x;
    TileState  currentTile = mapState[i][j];
    
    TileState newTile = {loc, occupy, stop};
    mapState[i][j] = newTile;
        //mapState[i][j]->coordinates = Vec2(loc);
        //mapState[i][j].isStop = stop;
        //mapState[i][j].occupied = occupy;
    
}

bool MapState::isLocInRange(Vec2 loc)
{
    float x = loc.x;
    float y = loc.y;
    
    if (x<0 || y<0)
    {
        return false;
    } else if (x>sizeX || y>sizeY){
        return false;
    } else {
        return true;
    }
    
}
