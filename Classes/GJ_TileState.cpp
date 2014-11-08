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
    TileState t = {Vec2(0,0), false, false, nullptr, nullptr, 0};
    std::vector<TileState> tileVec(1);
    tileVec.push_back(t);
    mapState.push_back(tileVec);
    
    //TileGroup newGroup = {nullptr, nullptr, 0, 0, 0};
    //tileGroups.push_back(newGroup);
}

bool MapState::initMap(Size mapSize)
{
   // if we call initMap(), we should expect it to be cleared
    setMapSize(mapSize.width, mapSize.height);
    mapState = std::vector<std::vector<TileState>>(mapSize.height, std::vector<TileState>(mapSize.width));
    numGroups=0;
    
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

    return true;
}

bool MapState::addTile(Vec2 loc)
{
    TileState * temp = &mapState[loc.x][loc.y];

    if (temp) {
        //temp->coordinates.set(loc);
        temp->occupied = false;
        temp->isStop = false;
    }
    else
    {
        auto newTile = createTileState(loc, false, false);
        mapState[loc.x][loc.y] = newTile;
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
    
    newTile.occupied = occupy;
    newTile.isStop = stop;
    newTile.color = kNoColor;
    newTile.rank = 0;
    // newTile.head = nullptr;
    // newTile.next = nullptr;
    
    return newTile;
}

void MapState::setTileOccupied(Vec2 loc)
{
    mapState[loc.x][loc.y].occupied = true;
}

void MapState::setTileNotOccupied(Vec2 loc)
{
    mapState[loc.x][loc.y].occupied = false;
}

bool MapState::isTileOccupied(Vec2 loc)
{
    return mapState[loc.x][loc.y].occupied;
}

void MapState::setTileStop(Vec2 loc)
{
    mapState[loc.x][loc.y].isStop = true;
}

void MapState::setTileNotStop(Vec2 loc)
{
    mapState[loc.x][loc.y].isStop = false;
}

bool MapState::isTileStop(Vec2 loc)
{
    return mapState[loc.x][loc.y].isStop;
}

// color
void MapState::setTileColor(Vec2 loc, int color)
{
    mapState[loc.x][loc.y].color = color;
}

void MapState::clearTileColor(Vec2 loc)
{
    mapState[loc.x][loc.y].color = kNoColor;
}

int MapState::getTileColor(Vec2 loc)
{
    return mapState[loc.x][loc.y].color;
}


void MapState::setTileState(Vec2 loc, bool occupy, bool stop)
{
    int i = loc.x;
    int j = loc.y;
    TileState  currentTile = mapState[i][j];
    
    TileState newTile = {loc, occupy, stop};
    newTile.color = kNoColor;
    newTile.rank = 0;
    //newTile.head = nullptr;
    //newTile.next = nullptr;
    
    mapState[i][j] = newTile;
    
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

// This will need more work when new tile characteristics are added
void MapState::moveTileState(Vec2 fromLoc, Vec2 toLoc)
{
    // this only moves things related to puzzle tiles
    setTileNotOccupied(fromLoc);
    setTileOccupied(toLoc);
    
    int color = getTileColor(fromLoc);
    clearTileColor(fromLoc);
    setTileColor(toLoc, color);
    
    // have to move TileGroup as well
}

void MapState::copyTileState(Vec2 fromLoc, Vec2 toLoc)
{
    // I'll get to this when/if I need it
}

// for finding connected components

TileState * MapState::makeSet(Vec2 loc)
{
    if(!isLocInRange(loc)) {return NULL;}
    
    // recover the tile from the location
    TileState * curTile = &mapState[loc.x][loc.y];

    curTile->parent = NULL;
    curTile->rank = 0;

}

void MapState::unionSets(TileState * setA, TileState * setB)
{
    if (setA->rank > setB->rank) {
        setB->parent = setA;
    } else if (setB->rank > setA->rank) {
        setA->parent = setB;
    } else { /* they are equal */
        setB->parent = setA;
        setA->rank++;
    }
}

TileState * MapState::findSet(TileState * node)
{
    TileState* temp;
    TileState* root = node;

    while (root->parent != NULL) {
        root = root->parent;
    }
    /* Update the parent pointers */
    while (node->parent != NULL) {
        temp = node->parent;
        node->parent = root;
        node = temp;
    }
    
    return root;
}

bool MapState::sameComponent(Vec2 locA, Vec2 locB)
{
   
}

void MapState::connectComponents(void)
{
    if (sizeX==0 || sizeY==0) {
        CCLOG("Map is empty - no elements to connect!\n");
        return;
    }
    
    // make all the sets for occupied tiles
    for(int i = 0; i< sizeY; i++)
    {
        for (int j = 0; j<sizeX; j++)
        {
            if (mapState[i][j].occupied) {
                makeSet(Vec2(i,j));
                occupiedTiles.push_back(mapState[i][j]);
            }
        }
    }
    
    // for each tile that is occupied, test neighbors for same color
    // +/- 1
    int x = 0 , y = 0;
    int tileColor = kNoColor;
    int tileRank;
    std::vector<TileState*> neighbors;
    
    for (auto &tile : occupiedTiles) {
        x = tile.coordinates.x;
        y = tile.coordinates.y;
        tileColor = tile.color;
        tileRank = tile.rank;
        //setA = findSet(&tile)->rank;
        
        // find all connected neighbors not already merged into the same group, track them
        if (tileColor == mapState[x-1][y].color)// && tileRank!=mapState[x-1][y].rank)
        {
            neighbors.push_back(&mapState[x-1][y]);
            
        } else if (tileColor == mapState[x][y-1].color)// && tileRank!=mapState[x][y-1].rank)
        {
            neighbors.push_back(&mapState[x][y-1]);
            
        } else if (tileColor == mapState[x+1][y].color)// && tileRank!=mapState[x+1][y].rank)
        {
            neighbors.push_back(&mapState[x+1][y]);
            
        } else if (tileColor == mapState[x][y+1].color)// && tileRank!=mapState[x][y+1].rank)
        {
            neighbors.push_back(&mapState[x][y+1]);
        }
        
        // merge all the neighbors we pushed back, if we have any
        if (neighbors.size()>0) {
            
            for (int i=0; i<neighbors.size(); i++) {
                unionSets(&tile, neighbors[i]);
                // if we merge any tiles, we need to delete
                findSet(&tile)->markedForDelete = true;
            }
            
        }
        
        // delete neighbors
        neighbors.clear();
        // move on to the next tile
    }
    
    // clean up occupied tiles - we will reconstruct this.
//    
//    occupiedTiles.clear();
    
}

int MapState::deleteComponents(void)
{
    int tilesDeleted = 0;
    TileState * t;
    
    for (int i=0; i<occupiedTiles.size(); i++)
    {
        if (findSet(&occupiedTiles[i])->markedForDelete) {
            CCLOG("Tile marked for deletion: %f, %f\n", occupiedTiles[i].coordinates.x, occupiedTiles[i].coordinates.y);
            tilesDeleted++;
        }
    }

    return tilesDeleted;
}