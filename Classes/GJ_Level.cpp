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
    
    // reserve the Sprite array
    puzzleTiles.reserve(numTiles);
    
    // this is not retained right now (should it be?)
    TMXObjectGroup * puzzleTileGroup = tileMap->getObjectGroup("Objects");
    ValueVector objectTiles = puzzleTileGroup->getObjects();
    
    if (puzzleTileGroup == NULL) {
        CCLOG("Puzzle tile map is NULL");
        return false;
    }
    
    // This is where different tiles are found in the map and their sprites created
    // range-based for loop, lambda to create, set properties, and push back tiles
    
    auto createTile = [&](std::string tileType, Vec2 loc){
        
        Sprite * temp = Sprite::create(tileType);
        temp->setPosition(loc);
        temp->setAnchorPoint(Vec2(0,0));
        tileMap->addChild(temp, 0);
        puzzleTiles.push_back(temp);
    };
    
    // determine color property, get coords, create tile
    for (auto& tile : objectTiles) {
        v = &tile.asValueMap();
        x = v->at("x").asInt();
        y = v->at("y").asInt();
        auto tileType = v->at("name").asString();
       
        // would a switch be more efficient? this seems pretty good
        if(tileType == "BlueSpawn")
        {
            createTile("blueTile.png", Vec2(x,y));
        } else if (tileType == "RedSpawn"){
            createTile("redTile.png", Vec2(x,y));
        } else if (tileType == "GreenSpawn"){
            createTile("greenTile.png", Vec2(x,y));
        } else if (tileType == "YellowSpawn"){
            createTile("greenTile.png", Vec2(x,y));
        } else if (tileType == "OrangeSpawn"){
            createTile("orangeTile.png", Vec2(x,y));
        } else {
            CCLOG("Unrecognized Tile type '%s' at %i, %i \n", v->at("name").asString().c_str(), x, y);
        }
    }
    
    return true;
}

bool Level::createPassiveTiles()
{
    // place tiles that don't dissappear, right now they are painted on the bkgnd
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

    
    if (!createPassiveTiles()) {
        CCLOG("Error creating passive tiles!");
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
    return true;
}

// movement

Vec2 Level::tileCoordForPosition(Vec2 position)
{
// this is suspect
    int tileWidth = tileMap->getTileSize().width;
    int tileHeight = tileMap->getTileSize().height;
    
    int x = position.x / tileWidth;
    
    int y = position.y / tileHeight;
    
    return Vec2(x, y);

    return position;
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
            return inCoord;
            break;
    }
    
    return newPos;
}

bool Level::getStopProp(Vec2 toCoord)
{
    Vec2 tileCoord = this->tileCoordForPosition(toCoord);
    bool stopVal = false;
    int tileGid = metaLayer->getTileGIDAt(tileCoord);
    
    if (tileGid) {
        Value properties = tileMap->getPropertiesForGID(tileGid);
        stopVal = properties.asValueMap().at("Stop").asBool();
    } else if (tileGid == 0)
    {
        // empty tile
        stopVal = false;
    }
    
    // there must be a safer way - what if we pass a bad location?
    
    return stopVal;
}

void Level::createPuzzleTileMove(Sprite * theTile, int direction)
{
    bool stopped = false;
    Vec2 currentPos = theTile->getPosition();
    Vec2 nextPos;
    
    // while we haven't found a stop tile
    while (!stopped) {
        nextPos.set(getAdjacentPxCoord(currentPos, direction));
        
        if (getStopProp(nextPos)) { // find the stop property of the next tile in the direction
            stopped = true; // if it is stop, currentPos is the end of the move
        } else {
            currentPos.set(nextPos); // otherwise we move ahead
        }
    }
    
    theTile->runAction(MoveTo::create(0.25, currentPos));
    
}

void Level::moveTiles(int dir)
{
    // for each puzzle tile
    // construct the movement action in direction dir
    for(auto &s : puzzleTiles)
    {
        createPuzzleTileMove(s, dir);
    }
 
    // for each puzzle tile
    // run the constructed actions
}


