//
//  GJ_MainMenu.h
//  Gravity_Jam
//
//  Created by Garry Kling on 11/22/14.
//
//

#ifndef __Gravity_Jam__GJ_MainMenu__
#define __Gravity_Jam__GJ_MainMenu__

#include "cocos2d.h"
#include "GJ_Level.h"
#include <stdio.h>

USING_NS_CC;

class MainMenu : public cocos2d::Layer
{
public:
    
    static Scene * createScene();
    
    virtual bool init();
    
    // create menu elements
    bool loadMainMenu(Layer * activeLayer);
    bool loadBackground(void);
    bool createTitle(void);
    bool createButtons(void);
    bool createTouchListeners(void);
    bool cleanupMenu(void);
    
    // button handlers
    static void playButtonFunc(cocos2d::Ref* sender);
    static void optionsButtonFunc(cocos2d::Ref* sender);
    static void quitButtonFunc(cocos2d::Ref* sender);
    static void titleButtonFunc(cocos2d::Ref* sender);
    
    // macro for parent class init
    CREATE_FUNC(MainMenu);
    
private:
    int numItems;
    Menu * theMenu;
    MenuItem * label;
    MenuItem * playButton;
    MenuItem * optionsButton;
    MenuItem * quitButton;
    
    cocos2d::Vector<cocos2d::MenuItem*> menuButtons;
    LayerColor * colorLayer;
};

#endif /* defined(__Gravity_Jam__GJ_MainMenu__) */
