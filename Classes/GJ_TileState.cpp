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
    sizeX = 0;
    sizeY = 0;
    numTiles = 0;
    TileState t = {nullptr, Vec2(0,0), kNoColor, false, false, nullptr, 0, false, false, false};
    std::vector<TileState> tileVec(1);
    tileVec.push_back(t);
    mapState.push_back(tileVec);

}

bool MapState::initMap(Size mapSize, int tileWidth, int tileHeight)
{
   // if we call initMap(), we should expect it to be cleared
    setMapSize(mapSize.width, mapSize.height);
    mapState = std::vector<std::vector<TileState>>(mapSize.height, std::vector<TileState>(mapSize.width));
    this->tileWidth = tileWidth;
    this->tileHeight = tileHeight;
    
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
    
    sizeX = x;
    sizeY = y;
    numTiles = x*y;

    return true;
}

void MapState::addTile(Vec2 loc)
{
    TileState * temp = &mapState[loc.x][loc.y];

    if (temp) {
        temp->occupied = false;
        temp->isStop = false;
    }
    else
    {
        auto newTile = createTileState(loc, false, false);
        mapState[loc.x][loc.y] = newTile;
    }

}



void MapState::clearMap()
{
    
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
    newTile.markedForDelete = false;
    newTile.deleted = false;
    newTile.moveInProgress = false;
    
    return newTile;
}

// occupy/stop

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

// movement

void MapState::setTileMoving(Vec2 loc)
{
    mapState[loc.x][loc.y].moveInProgress = true;
}

void MapState::setTileNotMoving(Vec2 loc)
{
    mapState[loc.x][loc.y].moveInProgress = false;
}

bool MapState::isTileMoving(Vec2 loc)
{
    return mapState[loc.x][loc.y].moveInProgress;
}


void MapState::setTileState(Vec2 loc, bool occupy, bool stop)
{
    int i = loc.x;
    int j = loc.y;
    TileState * currentTile = &mapState[i][j];
    
    currentTile->coordinates.set(loc);
    currentTile->spritePtr = nullptr;
    currentTile->parent = nullptr;
    currentTile->occupied = occupy;
    currentTile->isStop = stop;
    currentTile->color = kNoColor;
    currentTile->rank = 0;
    currentTile->markedForDelete = false;
    currentTile->deleted = false;
    currentTile->moveInProgress = false;
    
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
    // scorched earth
    setSpriteForTile(toLoc, getSpriteForTile(fromLoc));
    setSpriteForTile(fromLoc, nullptr);
    
    // this only moves things related to puzzle tiles
    setTileNotOccupied(fromLoc);
    setTileOccupied(toLoc);
    
    int color = getTileColor(fromLoc);
    clearTileColor(fromLoc);
    setTileColor(toLoc, color);

}

//void MapState::copyTileState(Vec2 fromLoc, Vec2 toLoc)
//{
//    // I'll get to this when/if I need it
//}

bool MapState::checkTileToDeleteMark(Vec2 loc)
{
    return mapState[loc.x][loc.y].markedForDelete;
}

void MapState::setAsDeleted(Vec2 loc)
{
    mapState[loc.x][loc.y].deleted = true;
}

bool MapState::wasTileDeleted(Vec2 loc)
{
    return mapState[loc.x][loc.y].deleted;
}

void MapState::clearTileState(Vec2 loc)
{
    TileState * tile = &mapState[loc.x][loc.y];
    
    // clear all tile data
    tile->spritePtr = nullptr;
    tile->coordinates.set(loc);
    tile->color = kNoColor;
    tile->occupied = false;
    tile->isStop = false;
    tile->parent = nullptr;
    tile->rank = NULL;
    tile->markedForDelete = false;
    tile->deleted = false;
    tile->moveInProgress = false;
    
    printf("Clear tile state: (%f, %f)\n", loc.x, loc.y);
    printf("Is tile occupied: %i\n", tile->isStop);
    printf("Tile color: %i\n", tile->color);
    
}

// Sprite access
void MapState::setSpriteForTile(Vec2 loc, Sprite * newSprite)
{
    mapState[loc.x][loc.y].spritePtr = newSprite;
}

Sprite * MapState::getSpriteForTile(Vec2 loc)
{
    return mapState[loc.x][loc.y].spritePtr;
}

// for finding connected components

TileState * MapState::makeSet(Vec2 loc)
{
    if(!isLocInRange(loc)) {return NULL;}
    
    // recover the tile from the location
    TileState * curTile = &mapState[loc.x][loc.y];

    curTile->parent = NULL;
    curTile->rank = 0;
    
    return curTile;
}

TileState * MapState::findSet(TileState * node)
{
    TileState * temp;
    TileState * root = node;
    
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

Vec2 MapState::convertPxToCoord(float x, float y)
{
    x = x / tileWidth;
 
    // theMap vector is 0 based, so we need to subtract 1 - why not for x as well?
    y =(((sizeY * tileHeight) - y) / tileHeight) - 1;
    
    return Vec2(x, y);
}

void MapState::printState(void)
{
    int i,j, curCol;
    Vec2 curLoc;
    
    for (i=0; i<sizeX; i++) {
        for (j=0; j<sizeY; j++) {
            curLoc = Vec2(j,i);
            if(isTileStop(curLoc)){
                printf("*");
            } else if (isTileOccupied(Vec2(j,i))){
                curCol = getTileColor(Vec2(j,i));
                if(curCol==kBlue){printf("B");} else if (curCol==kRed){printf("R");} else if (curCol==kGreen){printf("G");} else if(curCol==kYellow){printf("Y");} else if(curCol==kOrange){ printf("O");} else {printf("T");}
            } else {
                printf("-");
            }
        }
        printf("\n");
    }
}


void MapState::connectComponents(void)
{
    // Gather occupied tiles, check to see if their neighbors are the same colors
    // if so, group them via union-find
    
    std::vector<TileState> occupiedTiles;
    
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
                printf("Tile at: (%i,%i)\n", i,j);
            }
        }
    }
    
    // for each tile that is occupied, test neighbors for same color
    // push them onto the stack for grouping
    // +/- 1
    int x = 0 , y = 0;
    int tileColor = kNoColor;
    int tileRank;
    int upColor, dwnColor, leftColor, rtColor;
    upColor=dwnColor=leftColor=rtColor = kNoColor;
    std::vector<TileState*> neighbors;
    Vec2 tileLoc;
    
    for (auto &tile : occupiedTiles) {
        
        if(tile.spritePtr==NULL){continue;}
        
        //tileLoc = tile.spritePtr->getPosition();
        //tileLoc = convertPxToCoord(tileLoc.x, tileLoc.y);
        tileLoc = tile.coordinates;
        
        // let's try to invert it simply
        x = tileLoc.x;
        y = tileLoc.y;
        
        tileColor = tile.color;
        tileRank = tile.rank;
        
        rtColor = mapState[x+1][y].color;
        printf("Right: (%i,%i) with color:%i\n", x+1, y, rtColor);
        leftColor = mapState[x-1][y].color;
        printf("Left: (%i,%i) with color:%i\n", x-1, y, leftColor);
        upColor = mapState[x][y-1].color;
        printf("Up: (%i,%i) with color:%i\n", x, y-1, upColor);
        dwnColor = mapState[x][y+1].color;
        printf("Down: (%i,%i) with color:%i\n\n", x, y+1, dwnColor);
        
        // if the tile has no color, no need to check neighbors
        if (tileColor != kNoColor) {
      
            // find all connected neighbors not already merged into the same group, set them aside
            if (tileColor == leftColor)
            {
                neighbors.push_back(&mapState[x-1][y]);
                
            } else if (tileColor == dwnColor)
            {
                neighbors.push_back(&mapState[x][y+1]);
                
            } else if (tileColor == rtColor)
            {
                neighbors.push_back(&mapState[x+1][y]);
                
            } else if (tileColor == upColor)
            {
                neighbors.push_back(&mapState[x][y-1]);
            }
        }
        
        // the second pass
        // merge all the neighbors we pushed back, if we have any
        if (neighbors.size()>0) {
            printf("Color matches:");
            for (int i=0; i<neighbors.size(); i++) {
                unionSets(&tile, neighbors[i]);
                tileLoc = tile.coordinates;
                // if we merge any tiles, we need to delete - mark the group's parent
                findSet(&tile)->markedForDelete = true;
                // mark this tile as well
                neighbors[i]->markedForDelete = true;
                Vec2 neighborLoc = neighbors[i]->coordinates;
                printf("Group: tile (%f,%f), neighbor (%f,%f)\n", tileLoc.x, tileLoc.y, neighborLoc.x, neighborLoc.y);
                
            }
        } else {printf("No color match\n");}
        
        // clear the neighbors
        neighbors.clear();
    }
    
    // clean up occupied tiles - we will reconstruct this data.
    
    occupiedTiles.clear();
    
}

