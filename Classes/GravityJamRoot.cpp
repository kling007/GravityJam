#include "GravityJamRoot.h"
#include "GJ_Settings.h"

USING_NS_CC;

Scene* GravityJamRoot::createScene()
{
    // 'scene', 'layer' are autorelease objects
    auto scene = Scene::create();
    auto layer = GravityJamRoot::create();

    scene->addChild(layer);

    // return the scene
    return scene;
}

bool GravityJamRoot::init()
{
    // ---> super init()
    if ( !Layer::init() )
    {
        return false;
    }
    
    // cache some data from the director, set bkgnd color
    
    visibleSize = Director::getInstance()->getVisibleSize();
        origin = Director::getInstance()->getVisibleOrigin();
    
    Color3B glClear = GJ_Settings::gj_backgroundUIColor;
    glClearColor(glClear.r,glClear.g,glClear.b, 0);
    
    // ---> create off switch - temporary off button for testing only
    auto closeItem = MenuItemImage::create("CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(GravityJamRoot::menuCloseCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));
    
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    
    // TO DO ---> get our resolution/scaling straight for different devices
    
    if(!this->setupTouches())
    {
        CCLOG("Error setting up Touch listeners!");
        return false;
    }
    
    /*
     * Playing around for now...
     * eventually init will create a menu scene and control will go there
     */
        
    // testing level and HUD creation

    theLevel.init();
    if(!this->initLevel(1))
    {
        CCLOG("Error initializing levels.");
        return false;
    }
    
    // HUD
    if(!this->initHUD())
    {
        CCLOG("Error initializing HUD.");
        return false;
    }
    
    this->setTouchEnabled(true);
    return true;
}

bool GravityJamRoot::initHUD()
{
  
    // bring up the HUD
    theHUD.init();
    theHUD.theScore.setScore(this->theScore.getScore());
    this->addChild(&theHUD);
    
    return true;
}

bool GravityJamRoot::initLevel(int levelNum)
{
    // Is there anything we need to do before loading the level? The Level class should handle most of it.
    
    theLevel.loadLevel(levelNum, this);
    
    return true;
}

bool GravityJamRoot::closeLevel()
{
    // To do: what needs to happen between levels?
    // probably unload level data, update event listeners, etc.
  
    return true;
}

//void GravityJamRoot::registerWithTouchDispatcher() {
//    Director::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
//}

bool GravityJamRoot::setupTouches()
{
    // set up touch listening - one at a time
    touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    
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
    touchListener->onTouchEnded = [this](Touch* touch, Event* event){
        
        auto target = static_cast<Node*>(event->getCurrentTarget());
        Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());
        this->touchEnd.set(locationInNode);
        
        //
        int dir = this->getTouchDirection();
        CCLOG("Direction detected: %i \n", dir);
        
        theLevel.moveTiles(dir);
        
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    return true;
}

int GravityJamRoot::getTouchDirection()
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

void GravityJamRoot::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

// Code dump

/*
 // unload a level if it is currently there (clear sprite cache)
 if (!tileSprites.empty()) {
 while (!tileSprites.empty()) {
 tileSprites.pop_back();
 }
 }
 
 // this is where we will choose the appropriate tileMap given the level passed
 
 // place the tilemap
 auto tileMap = TMXTiledMap::create("Graphics/GJ_Level1.tmx");
 background = tileMap->getLayer("Background");
 float posX, posY;
 
 
 // the puzzle area is rooted at (3/16 * x, 1/6 * y)
 posX = origin.x + 0.1875*visibleSize.width;
 posY = origin.y + visibleSize.height/6.0;
 
 tileMap->setAnchorPoint(Vec2(0, 0));
 Size mapSize = tileMap->getMapSize();
 int numTiles = ceil(mapSize.height*mapSize.width);
 float tm_scale = 0.375*visibleSize.width/tileMap->getContentSize().width;
 
 // reserve the Sprite array
 tileSprites.reserve(numTiles);
 
 // spawn puzzle pieces - maybe use another function/method for this
 TMXObjectGroup * objectGroup = tileMap->getObjectGroup("Objects");
 
 if (objectGroup == NULL) {
 CCLOG("Tile map is NULL");
 return false;
 }
 
 // iterate and add sprites according to name "BlueSpawn"
 ValueVector objectTiles = objectGroup->getObjects();
 ValueMap * v;
 int x, y = 0;
 
 // find where to put our active tiles (the ones the play moves around)
 for (auto& tile : objectTiles) {
 v = &tile.asValueMap();
 
 // should we check this more thoroughly?
 if(v->at("name").asString() == "BlueSpawn")
 {
 // get the coordinates of the spawn point we've found
 // these need to be anchored to 0,0 of tile area in Tiled
 x = v->at("x").asInt();
 y = v->at("y").asInt();
 
 // create a blue tile sprite - need to figure out how to do this smartly per color
 Sprite * temp = Sprite::create("blueTile.png");
 // get that out of the for loop you lazy POS
 //temp->setScale(tm_scale);
 temp->setPosition(Vec2(x,y));
 temp->setAnchorPoint(Vec2(0,0));
 tileMap->addChild(temp, 0);
 
 // push it on to the SpriteVector
 tileSprites.push_back(temp);
 }
 }
 
 // scale the tileMap to fit into the puzzle area, which is 3/8 * x
 // will change after we have a design fix
 
 tileMap->setScale(tm_scale);
 tileMap->setPosition(Vec2(posX, posY));
 this->addChild(tileMap, 0);
 */