//
//  GJ_Level.cpp
//  Gravity_Jam
//
//  Created by Garry Kling on 10/16/14.
//
//

#include "GJ_Level.h"
#include <string>
#include <iostream>

using string = std::basic_string<char>;

// initialization
Scene* Level::createScene()
{
    // 'scene', 'layer' are autorelease objects
    Scene * scene = Scene::create();
    auto layer = Level::create();
    scene->retain();
    layer->retain();
    scene->addChild(layer);
    
    // return the scene
    return scene;
}


bool Level::init()
{
    // init the super
    if ( !Layer::init() ) {
        
        return false;
    }
    
    parentVisibleSize = Director::getInstance()->getVisibleSize();
    parentOrigin = Director::getInstance()->getVisibleOrigin();
    
    // setup touch handling
    if(!setupTouches())
    {
        printf("Error setting up touch!\n");
    }
    
    // this is where we should load any persistent data for a player
    // particularly the score and level number
    
    curLevel = 1;
    curDirection = 0;
    posX = 0.0f;
    posY = 0.0f;
    tm_scale = 0.0f;
    levelComplete = false;
    
    loadLevel(curLevel);
    
    return true;
}

// Touch handling

bool Level::setupTouches()
{
    // set up touch listening - one at a time
    touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->retain();
    
    touchListener->onTouchBegan = [this](Touch* touch, Event* event)
    {
        
        // get the node out of the event, cast it to Node
        auto target = static_cast<Node*>(event->getCurrentTarget());
        
        //Get the position of the current point relative to node space of this
        Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());
        Size s = target->getContentSize();
        Rect rect = Rect(0, 0, s.width, s.height);
        
        //Check the click area
        if (rect.containsPoint(locationInNode))
        {
            // set the touch starting point
            this->touchBegin.set(locationInNode);
            return true;
        }
        return false;
    };
    
    touchListener->onTouchMoved = [](Touch* touch, Event* event){
        // Do nothing. perhaps modify a compass to show swipe direction?
    };
    
    
    //Process the touch end event
    touchListener->onTouchEnded = [&](Touch* touch, Event* event){
        // reject touch ended if we are in a faulted state so we don't crash
        // alternatively, we should disable the listener and prompt the user to return to main menu, etc.
//        if (faulted) {
//            return;
//        }
        auto target = static_cast<Node*>(event->getCurrentTarget());
        Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());
        this->touchEnd.set(locationInNode);
        
        //
        int dir = this->getTouchDirection();
        CCLOG("Direction detected: %i \n", dir);
        
        moveTiles(dir);
        
        // check for end of level condition, additional moves
        endOfMoveChecks(dir);
        
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    return true;
}

int Level::getTouchDirection()
{
    // **** need to add a policy for extra short gestures?
    
    Vec2 touchVector = Vec2(touchBegin, touchEnd);
    float swipeX = fabsf(touchEnd.x - touchBegin.x);
    float swipeY = fabsf(touchEnd.y - touchBegin.y);
    
    
    if(swipeX>swipeY) /* swipe left/right */
    {
        if (touchEnd.x>touchBegin.x) {
            // right swipe
            return RIGHT;
        } else {
            // left swipe
            return LEFT;
        }
        
    } else { /* swipe up/down */
        
        if (touchEnd.y>touchBegin.y) {
            // up swipe
            return UP;
        } else {
            // down swipe
            return DOWN;
        }
    }
    
    // default to 0==NO direction
    return NO_DIRECTION;
}



bool Level::createPuzzleTiles()
{
    ValueMap * v;
    int x, y = 0;
    
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
        temp->retain();
        tileMap->addChild(temp, 0);
        theMap->setTileOccupied(tileCoords);
        theMap->setTileColor(tileCoords, color);
        theMap->setSpriteForTile(tileCoords, temp);
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
bool Level::loadLevel(int levelNum)
{

    // How do we check the map to see if we need to clear it, or will this only be called on a fresh level?
    // *******************
    
    // create the filename
    string filePrefix = "Graphics/GJ_Level";
    string fileSuffix = ".tmx";
    string levelString = std::to_string(levelNum);
    string levelFileName = filePrefix+levelString+fileSuffix;
    
    std::cout<<levelFileName;
    
    
    // place the tilemap
    // need to match the incoming levelNumber with the .tmx filename
    
    theMap = new MapState();
    tileMap = TMXTiledMap::create(levelFileName);
    if(!tileMap){return false;}
    else {curLevel = levelNum;}
    
    background = tileMap->getLayer("Background");
    metaLayer = tileMap->getLayer("Meta");
    metaLayer->setVisible(false);
    
    // *******************
    // UH-OH! MAGIC NUMBER ALERT!!!
    // *******************
    // old landscape version
    // the puzzle area was rooted at (3/16 * x, 1/6 * y)
    //    posX = parentOrigin.x + 0.1875*parentVisibleSize.width;
    //    posY = parentOrigin.y + parentVisibleSize.height/6.0;
    
    // portrait version
    posX = parentOrigin.x;
    posY = parentOrigin.y + parentVisibleSize.height/4.0;
    
    tileMap->setAnchorPoint(Vec2(0, 0));
    Size mapSize = tileMap->getMapSize();
    numTiles = ceil(mapSize.height*mapSize.width);
    
    // going to have to find a better way to scale based on size
//    tm_scale = 0.375*parentVisibleSize.width/tileMap->getContentSize().width;
    tm_scale = parentVisibleSize.width/tileMap->getContentSize().width;
    
    // init our map state tracking data structure
    // actual initial state is loaded/created in the create funcs below
    theMap->initMap(mapSize, tileMap->getTileSize().width, tileMap->getTileSize().height);
    
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
    this->addChild(tileMap, 0);
    levelComplete = false;
    
    theMap->printState();
    
    return true;
}

bool Level::unloadLevel()
{
    // THIS NEEDS TO BE FIXED TO UNLOAD THE LEVEL PROPERLY!!!
    //*******************************************************
    
    // har har - this is far from complete
    tileMap->removeAllChildrenWithCleanup(true);
    tileMap->removeFromParentAndCleanup(true);
    
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
    // calculate the pixel location of the next tile towards the specified direction
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
    // calculate the map coordinates of the next tile towards the specified direction
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
            CCLOG("Bad direction value passed: %d\n", direction);
            return inCoord;
            break;
    }
    
    return newPos;
}


bool Level::createPuzzleTileMove(Sprite * theTile, int direction)
{
    // create the actions for the tile moving in the specified direction and schedule/run them
    // accommodating more complicated and special tile actions here may require creating stacks of actions
    // before wrapping them in a sequence and sending them to runAction.
    // For now the simple construction below will suffice.
    
    bool stopped = false;
    Vec2 currentPos = theTile->getPosition();
    Vec2 oldPosCoords = tileCoordForPosition(currentPos);
    Vec2 nextPos, nextPosCoords;
    int numTilesInMove = 0;
    
    while (!stopped) {
        // we calc next location in terms of px, check it in tileCoords, do move in px
        nextPos.set(getAdjacentPxCoord(currentPos, direction));
        nextPosCoords.set(tileCoordForPosition(nextPos));
        
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
            numTilesInMove++;
        }
        
    endMove: ;
        
    }
    
    // create a lamdba to execute at the end of the action so our tile can delete itself from the map
    // yay thread un-safeness
    
    auto checkDelete = CallFuncN::create([&](Node * theTile)
    {
        Vec2 tileLoc = tileCoordForPosition(theTile->getPosition());
        
        CCLOG("Tile check delete pos = %f, %f", tileLoc.x, tileLoc.y);
        bool doWeDelete = theMap->checkTileToDeleteMark(tileLoc);
        // if it is marked for deletion, do it here.
        if(doWeDelete)
        {
            // run delete animation actions
            FadeOut * fade = FadeOut::create(0.5);
            
            auto delAction = CallFuncN::create([&, tileLoc](Node * tile){
                tile->removeFromParentAndCleanup(true);
                theMap->clearTileState(tileLoc);
            });
            
            auto updateAction = CallFunc::create([&](){
                // this triggers all updates needed for moving remaining tiles to collapse into deleted areas
                // I don't know why I have to explicitely call it,
                // scheduleOnce complains about scheduling the selector twice
                // scheduleOnce(schedule_selector(Level::update), 0.0);
                update(0.0);
            });
            
            Sequence * deleteSeq = Sequence::create(fade, delAction, updateAction, NULL);
            theTile->runAction(deleteSeq);
            
            printf("Tile scheduled for delete: (%f,%f)\n", tileLoc.x, tileLoc.y);
            
        }
        // give moveTiles the go ahead to poke around here now that we are done mucking around.
        // I'm concerned that the deleteSeq action may still be running when movetiles does its thing,
        // and may try to move a tile just as it is being deleted.
        theMap->setTileNotMoving(tileLoc);
    }
    );
    
   
    // if we move nothing, let the caller know and leave
    // check the delete status first in case adjacent tile moves have marked the tile
    
    if (numTilesInMove==0) {
        theTile->runAction(checkDelete);
        return false;
    }
    
    // else, we move
    MoveTo * move = MoveTo::create(0.05*numTilesInMove, currentPos);
    
    // do a sequence of actions, the last of which will check we need to delete the tile after moving
    Sequence * seq = Sequence::create(move, checkDelete, NULL);
    
    Vec2 newCoords = this->tileCoordForPosition(currentPos);
    theMap->moveTileState(oldPosCoords, newCoords);
    
    // set tile as moving and run the action so moveTiles won't try to move it again while it is enroute.
    theMap->setTileMoving(newCoords);
    theTile->runAction(seq);
    
    // let the caller know we moved something
    return true;
}

void Level::moveTiles(int dir)
{
    // for each puzzle tile, check if they should move and how far.
    // construct the movement action in direction dir
    // don't move tiles that are already moving until they stop.
    
    if (levelComplete) { // buzzoff, you
        return;
    }
    
    //printf("Before: \n");
    //theMap->printState();
    
    Sprite * theTile;
    curDirection = dir;
    
    int x, y;
    int maxX = theMap->sizeX - 1;
    int maxY = theMap->sizeY - 1;
    bool moving = false;
    bool delMar = false;
    
    Vec2 curLoc;
    // these sentinels indicate whether we need to check for grouped colors and/or if we need to delete tiles and recurse
    movesDone = false;
    
    // this will be the site of many step throughs as puzzles get more complex
    if(dir==UP)
    {
        for (x=maxX; x>=0; x--) {
            for (y=0; y<=maxY; y++) {
                curLoc = Vec2(x,y);
                theTile = theMap->getSpriteForTile(curLoc);
                moving = theMap->isTileMoving(curLoc);
                delMar = theMap->checkTileToDeleteMark(curLoc);
                if (theTile != nullptr && !moving && !delMar) {
                    if(createPuzzleTileMove(theTile, dir))
                    {
                    movesDone = true;
                    }
                }
            }
        };
    } else if(dir==DOWN  ){
        for (x=maxX; x>=0; x--) {
            for (y=maxY; y>=0; y--) {
                curLoc = Vec2(x,y);
                theTile = theMap->getSpriteForTile(curLoc);
                moving = theMap->isTileMoving(curLoc);
                delMar = theMap->checkTileToDeleteMark(curLoc);
                if (theTile != nullptr && !moving && !delMar) {
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
                curLoc = Vec2(x,y);
                theTile = theMap->getSpriteForTile(curLoc);
                moving = theMap->isTileMoving(curLoc);
                delMar = theMap->checkTileToDeleteMark(curLoc);
                if (theTile != nullptr && !moving && !delMar) {
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
                curLoc = Vec2(x,y);
                theTile = theMap->getSpriteForTile(curLoc);
                moving = theMap->isTileMoving(curLoc);
                delMar = theMap->checkTileToDeleteMark(curLoc);
                if (theTile != nullptr && !moving && !delMar) {
                    if(createPuzzleTileMove(theTile, dir))
                    {
                        movesDone = true;
                    }
                }
            }
        };
    }
    
    theMap->printState();
    
    // if we moved anything, check for connected components
    if(movesDone)
    {
        theMap->connectComponents();
    }

    endOfMoveChecks(dir);

}

void Level::update(float dt)
{
    if(isCurrentLevelComplete() && !faulted)
    {
        // run end of level function]
        printf("Level %i complete. Preparing new level...", curLevel);
        scheduleOnce(schedule_selector(Level::endLevel), 0.0);
    }
    moveTiles(curDirection);
}

void Level::endOfMoveChecks(int dir)
{
    // TODOs: do scoring?, if level is complete, ask parent to unload the level
    // check to see if level is complete
    
    bool remainingOccupiedTiles = false;
    
    printf("movesDone = %i\n", movesDone);
    
    // clear deleted tiles
    for (int j=0; j<theMap->sizeX; j++) {
        for (int i=0; i<theMap->sizeY; i++) {

            // check if there are any occupied tiles
            if (theMap->isTileOccupied(Vec2(i,j))) {
                remainingOccupiedTiles = true;
            }
        }
    }
    
    if (!remainingOccupiedTiles) {
        // Our end-of-level code goes here
        levelComplete = true;
        printf("Level %i Complete!\n", curLevel);

        // run end of level function]
        printf("Preparing new level...");
        tileMap->getParent()->scheduleUpdate();
    }
    
}

void Level::closeLevel(void)
{
    // To do: what needs to happen between levels?
    // probably unload level data, update event listeners, etc.
    
    unloadLevel();
}

bool Level::nextLevel(void)
{
    curLevel++;
    
    // we should check the level against a max, but I don't know how to determine that yet.
    // for now, we'll let that fail happen in Level::initLevel()
    return loadLevel(curLevel);
}

void Level::endLevel(float dt)
{
    // we will need all of the end of level graphics here
    // for now, just unload the level and load the next one
    closeLevel();
    if(!nextLevel())
    {
        printf("Error opening level %i", curLevel);
        faulted = true;
    }
}

bool Level::isCurrentLevelComplete(void)
{
    return levelComplete;
}
