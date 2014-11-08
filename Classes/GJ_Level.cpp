//
//  GJ_Level.cpp
//  Gravity_Jam
//
//  Created by Garry Kling on 10/16/14.
//
//

#include "GJ_Level.h"


// standard methods
bool Level::init()
{
    // init the super
    if ( !Layer::init() ) {
        
        return false;
    }
    
    // not much to initialize here
    
    curLevel = 0;
    posX = 0.0f;
    posY = 0.0f;
    tm_scale = 0.0f;
    
    return true;
}

// tile management
bool Level::clearAllTiles()
{
    // unload puzzle tiles
    if (!clearPuzzleTiles()) {
        CCLOG("Error clearing puzzle tiles");
        return false;
    }
    
    // unload passive tiles
    if (!clearPassiveTiles()) {
        
        CCLOG("Error clearing passive tiles");
        return false;
    }

    return true;
}

bool Level::clearPuzzleTiles()
{
    // unload puzzle tiles
    if (!puzzleTiles.empty()) {
        while (!puzzleTiles.empty()) {
            puzzleTiles.pop_back();
        }
    }
    
    return true;
}

bool Level::clearPassiveTiles()
{
    // unload passive tiles
    if (!passiveTiles.empty()) {
        while (!passiveTiles.empty()) {
            passiveTiles.pop_back();
        }
    }
    return true;
}

bool Level::createPuzzleTiles()
{
    ValueMap * v;
    int x, y = 0;
    puzzleTiles.reserve(numTiles);
    
    TMXObjectGroup * puzzleTileGroup = tileMap->getObjectGroup("Objects");
    ValueVector objectTiles = puzzleTileGroup->getObjects();
    
    if (puzzleTileGroup == NULL) {
        CCLOG("Puzzle tile map is NULL");
        return false;
    }
    
    // This is where different tiles are found in the map and their sprites created
    // range-based for loop, lambda to create, set properties, and push back tiles
    
    auto createPuzzleTile = [&](std::string tileType, Vec2 loc, int color){
        
        Sprite * temp = Sprite::create(tileType);
        Vec2 tileCoords = tileCoordForPosition(loc);
        temp->setPosition(loc);
        temp->setAnchorPoint(Vec2(0,0));
        tileMap->addChild(temp, 0);
        puzzleTiles.push_back(temp);
        theMap->setTileOccupied(tileCoords);
        theMap->setTileColor(tileCoords, color);
    };
    
    // determine color property, get coords, create tile
    for (auto& tile : objectTiles) {
        v = &tile.asValueMap();
        x = v->at("x").asInt();
        y = v->at("y").asInt();
        auto tileType = v->at("name").asString();
       
        if(tileType == "BlueSpawn")
        {
            createPuzzleTile("blueTile.png", Vec2(x,y), kBlue);
        } else if (tileType == "RedSpawn"){
            createPuzzleTile("redTile.png", Vec2(x,y), kRed);
        } else if (tileType == "GreenSpawn"){
            createPuzzleTile("greenTile.png", Vec2(x,y), kGreen);
        } else if (tileType == "YellowSpawn"){
            createPuzzleTile("yellowTile.png", Vec2(x,y), kYellow);
        } else if (tileType == "OrangeSpawn"){
            createPuzzleTile("orangeTile.png", Vec2(x,y), kOrange);
        } else {
            CCLOG("Unrecognized Tile type '%s' at %i, %i \n", v->at("name").asString().c_str(), x, y);
        }
    }
    
    return true;
}

bool Level::createPassiveTiles()
{

    Size mapSize = tileMap->getMapSize();
    passiveTiles.reserve(numTiles);
    
    // passive tiles are on the meta layer
    // Passive tiles don't get sprites if they are in the design
    
    // this will make more sense once we have more types of passive tiles
    auto createPassiveTile = [&](std::string tileType, Vec2 loc){
        
        if(tileType=="Stop"){
            theMap->setTileStop(loc);
        };
    };
    
    ValueMap tileProperties;
    int theGID;
    
    // determine properties, get coords, add tile
    // for each in meta layer ( same size as mapState obj )
    // createPassiveTile per the properties therein
    for (int i=0;i<mapSize.height; i++)
    {
        for (int j=0; j<mapSize.width; j++)
        {
            Vec2 testLoc = Vec2(i, j);
            theGID = metaLayer->getTileGIDAt(testLoc);
            tileProperties = (tileMap->getPropertiesForGID(theGID)).asValueMap();
            CCLOG("metaLayer[%i][%i]", i,j);
            CCLOG("GID: %i\n", theGID);
            if (theGID != 0) {
                // if it isn't empty lets see what properties it has
                // only passive tile type at the moment is "Stop"
                
                if (tileProperties.at("Stop").asString()=="True") {
                    createPassiveTile("Stop", Vec2(i,j));
                }
            }
        }
    }
    
    return true;
}

// Top-level level management
bool Level::loadLevel(int levelNum, Layer * activeLayer)
{
    parentVisibleSize = Director::getInstance()->getVisibleSize();
    parentOrigin = Director::getInstance()->getVisibleOrigin();
    
    // perhaps a call to unload level would be better?
    if(!puzzleTiles.empty()||!passiveTiles.empty())
    {
        this->clearAllTiles();
    }
    
    // place the tilemap
    // need to match the incoming levelNumber with the .tmx filename
    
    theMap = new MapState();
    tileMap = TMXTiledMap::create("Graphics/GJ_Level1.tmx");
    background = tileMap->getLayer("Background");
    metaLayer = tileMap->getLayer("Meta");
    metaLayer->setVisible(false);
    
    
    // the puzzle area is rooted at (3/16 * x, 1/6 * y)
    posX = parentOrigin.x + 0.1875*parentVisibleSize.width;
    posY = parentOrigin.y + parentVisibleSize.height/6.0;
    
    tileMap->setAnchorPoint(Vec2(0, 0));
    Size mapSize = tileMap->getMapSize();
    numTiles = ceil(mapSize.height*mapSize.width);
    tm_scale = 0.375*parentVisibleSize.width/tileMap->getContentSize().width;
    
    // init our map state tracking data structure
    // actual initial state is created in the create funcs below
    theMap->initMap(mapSize);
    
    if (!createPassiveTiles()) {
        CCLOG("Error creating passive tiles!");
        return false;
    }
    
    if(!createPuzzleTiles()){
        CCLOG("Error creating puzzle tiles!");
        return false;
    }

    // scale the tileMap to fit into the puzzle area, which is 3/8 * x
    // will change after we have a design fix
    
    tileMap->setScale(tm_scale);
    tileMap->setPosition(Vec2(posX, posY));
    activeLayer->addChild(tileMap, 0);
    
    return true;
}

bool Level::unloadLevel()
{
    if(!puzzleTiles.empty()||!passiveTiles.empty())
    {
        this->clearAllTiles();
    }
    
    delete theMap;
    
    return true;
}

// movement

Vec2 Level::tileCoordForPosition(Vec2 position)
{
    int tileWidth = tileMap->getTileSize().width;
    int tileHeight = tileMap->getTileSize().height;

    int x = position.x / tileWidth;
    
    // theMap vector is 0 based, so we need to subtract 1 - why not for x as well?
    int y =(((tileMap->getMapSize().height * tileHeight) - position.y) / tileHeight) - 1;
    
    return Vec2(x, y);
}

Vec2 Level::getPxforCoord(Vec2 inCoord)
{
    int xIncrement = tileMap->getTileSize().width;
    int yIncrement = tileMap->getTileSize().height;
    
    return Vec2((inCoord.x)*xIncrement, (inCoord.y)*yIncrement);
}

Vec2 Level::getAdjacentPxCoord(Vec2 inCoord, int direction)
{
    int xIncrement = tileMap->getTileSize().width;
    int yIncrement = tileMap->getTileSize().height;
    
    Vec2 newPos;
    
    switch (direction) {
        case UP:
            newPos.set(Vec2(inCoord.x, inCoord.y+yIncrement));
            break;
        
        case DOWN:
            newPos.set(Vec2(inCoord.x, inCoord.y-yIncrement));
            break;
        
        case LEFT:
            newPos.set(Vec2(inCoord.x-xIncrement, inCoord.y));
            break;
        
        case RIGHT:
            newPos.set(Vec2(inCoord.x+xIncrement, inCoord.y));
            break;
            
        default:
            // is there something else I should do if I get a bad direction? Doubtful I will
            CCLOG("Bad direction value passed: %d\n", direction);
            return inCoord;
            break;
    }
    
    return newPos;
}

Vec2 Level::getAdjacentCoord(Vec2 inCoord, int direction)
{
    int xIncrement = 1;
    int yIncrement = 1;
    
    Vec2 newPos;
    
    switch (direction) {
        case UP:
            newPos.set(Vec2(inCoord.x, inCoord.y+yIncrement));
            break;
            
        case DOWN:
            newPos.set(Vec2(inCoord.x, inCoord.y-yIncrement));
            break;
            
        case LEFT:
            newPos.set(Vec2(inCoord.x-xIncrement, inCoord.y));
            break;
            
        case RIGHT:
            newPos.set(Vec2(inCoord.x+xIncrement, inCoord.y));
            break;
            
        default:
            // is there something else I should do if I get a bad direction? Doubtful I will
            CCLOG("Bad direction value passed: %d\n", direction);
            return inCoord;
            break;
    }
    
    return newPos;
}

Sprite * Level::getTileForCoord(Vec2 positionCoord)
{
    
    Vec2 tilePosInPx = getPxforCoord(positionCoord);
    for(auto &s : puzzleTiles)
    {
        if (s->getPosition()==tilePosInPx) {
            return s;
        }
    }
    
    // if there is nothing there, return a null ptr
    return nullptr;
}

bool Level::createPuzzleTileMove(Sprite * theTile, int direction)
{
    bool stopped = false;
    Vec2 currentPos = theTile->getPosition();
    Vec2 oldPosCoords = tileCoordForPosition(currentPos);
    Vec2 nextPos, nextPosCoords;
    int numTilesInMove = 0;
    
    // while we haven't found a stop condition, keep it moving
    while (!stopped) {
        // we calc next location in terms of px, check it in tileCoords, do move in px
        nextPos.set(getAdjacentPxCoord(currentPos, direction));
        nextPosCoords.set(this->tileCoordForPosition(nextPos));
        
        if (!theMap->isLocInRange(nextPosCoords)) {
            CCLOG("Tile reached edge of map! Check the level design for gaps in the stop property.\n");
            CCLOG("Tile location: %f, %f\n", nextPosCoords.x, nextPosCoords.y);
            stopped = true;
            goto endMove;
        }
        
        if (theMap->isTileStop(nextPosCoords)|| theMap->isTileOccupied(nextPosCoords)) {
            stopped = true; // if it is stop, currentPos is the end of the move
            goto endMove;
        } else {
            currentPos.set(nextPos); // otherwise we move ahead
        }
        
    endMove: ;
        numTilesInMove++;
    }
    
    // The duration of this move should a be unit of time for each tile width moved so that tiles move at the same speed
    theTile->runAction(MoveTo::create(0.05*numTilesInMove, currentPos));
    
    Vec2 newCoords = this->tileCoordForPosition(currentPos);
    theMap->moveTileState(oldPosCoords, newCoords);
    
    // if we move nothing, let the caller know
    if (numTilesInMove==0) {
        return false;
    }
    
    // let the caller know we moved something
    return true;
}

void Level::moveTiles(int dir)
{
    // for each puzzle tile
    // construct the movement action in direction dir
    
    Sprite * theTile;
    
    int x, y;
    int maxX = theMap->sizeX - 1;
    int maxY = theMap->sizeY - 1;
 
    // these sentinels indicate whether we need to check for grouped colors or if we need to delete tiles and recur
    bool movesDone = false;
    bool tilesDeleted = false;
    
    // this will be the site of many step throughs as puzzles get more complex
    if(dir==UP)
    {
        for (x=maxX; x>=0; x--) {
            for (y=maxY; y>=0; y--) {
                theTile = getTileForCoord(Vec2(x,y));
                if (theTile != nullptr) {
                    if(createPuzzleTileMove(theTile, dir))
                    {
                    movesDone = true;
                    }
                }
            }
        };
    } else if(dir==DOWN  ){
        for (x=maxX; x>=0; x--) {
            for (y=0; y<=maxY; y++) {
                theTile = getTileForCoord(Vec2(x,y));
                if (theTile != nullptr){
                    if(createPuzzleTileMove(theTile, dir))
                    {
                        movesDone = true;
                    }
                }
            }
        };
    } else if (dir==LEFT){
        for (y=0; y<=maxY; y++) {
            for (x=0; x<=maxX; x++) {
                theTile = getTileForCoord(Vec2(x,y));
                if (theTile != nullptr){
                    if(createPuzzleTileMove(theTile, dir))
                    {
                        movesDone = true;
                    }
                }
            }
        };
    } else if (dir==RIGHT){
        for (y=0; y<=maxY; y++) {
            for (x=maxX; x>=0; x--) {
                theTile = getTileForCoord(Vec2(x,y));
                if (theTile != nullptr){
                    if(createPuzzleTileMove(theTile, dir))
                    {
                        movesDone = true;
                    }
                }
            }
        };
    }

    // at this point in the story, we check for color adjacency and delete some tiles
    if(movesDone)
    {
        theMap->connectComponents();
        theMap->deleteComponents();
        // if any are found, set tilesDeleted = true
    }
    if(tilesDeleted)
    {
        // delete tiles
        // re-run this method
    }
}


