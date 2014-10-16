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
    
    
    Rect theRect = Director::getInstance()->getOpenGLView()->getVisibleRect();
    visibleSize = Director::getInstance()->getVisibleSize();
    visibleRect.initWithRect(&theRect);
    origin = Director::getInstance()->getVisibleOrigin();
    
    Color3B glClear = GJ_Settings::gj_backgroundUIColor;
    glClearColor(glClear.r, glClear.g,glClear.b, 0);
    
    // ---> create off switch
    auto closeItem = MenuItemImage::create("CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(GravityJamRoot::menuCloseCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));
    
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    // ---> get our resolution/scaling straight
    
    
    // ---> load up/create the gravity jam root scene
    
    /*
     * Playing around for now...
     * eventually init will call a menu method and control will go there
     */
        
    // place the first tilemap
    // not using the error flag, just testing level 1
    this->initLevel(1);
    
    // HUD
    this->initHUD();
    
    return true;
}

bool GravityJamRoot::initHUD()
{
    /*
     gj_score=0;
     gj_hours=0;
     gj_minutes=0;
     gj_seconds=0;
     gj_timeInSeconds=0;
     */
    
    // bring up the HUD
    theHUD.init();
    theHUD.theScore.setScore(this->theScore.getScore());
    this->addChild(&theHUD);
    
    return true;
}

bool GravityJamRoot::initLevel(int level)
{
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
    float tm_scale = 0.375*visibleSize.width/tileMap->getContentSize().width;
    
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
    // scale the tileMap to fit into the puzzle area, which is 3/8 * x - this should have design solution
    tileMap->setScale(0.375*visibleSize.width/tileMap->getContentSize().width);
    tileMap->setPosition(Vec2(posX, posY));
    this->addChild(tileMap, 0);
    
    return true;
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
