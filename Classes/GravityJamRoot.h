#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "GJ_Settings.h"
#include "GJ_HUD.h"
#include "VisibleRect.h"
#include "GJ_Level.h"

USING_NS_CC;

class GravityJamRoot : public cocos2d::Layer {
    
public:
    
    // the Scene object that this class manages
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    // Touch handling
    bool setupTouches();
    int getTouchDirection();
    EventListenerTouchOneByOne * touchListener;
    Vec2 touchBegin;
    Vec2 touchEnd;
    
    // theHUD
    GJ_HUD theHUD;
    bool initHUD();
    
    // level maps
    Level theLevel;
    bool initLevel(int levelNum);
    void closeLevel(void);
    bool nextLevel(void);
    void endLevel(float dt);
    
    void update(float dt);
    
    // off switch callback method (func)
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // clock tick method
    void tickClock();
    void resetClock();
    
    // cached contextual info
    VisibleRect visibleRect;
    Size visibleSize;
    Vec2 origin;
    
    // haven't started using this...
    Size designResolutionSize;
    
    // data for internal tracking and HUD
    
    Score theScore;
    int gj_level, gj_timeInSeconds;
    unsigned short gj_hours, gj_minutes, gj_seconds;
    bool faulted;
    
    // implement the "static create()" method manually
    CREATE_FUNC(GravityJamRoot);
    
    // apparently cocos2d-x example code doesn't "do" ctor/dtor methods... interesting
    
private:
    
    // tileMap related
    cocos2d::TMXTiledMap * tileMap;
    cocos2d::TMXLayer * background;
    
    // the cache of currently active TileSprites
    std::vector<Sprite*> tileSprites;

};

#endif // __HELLOWORLD_SCENE_H__
