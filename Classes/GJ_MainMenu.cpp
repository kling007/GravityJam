//
//  GJ_MainMenu.cpp
//  Gravity_Jam
//
//  Created by Garry Kling on 11/22/14.
//
//

#include "GJ_MainMenu.h"
#include "GJ_Settings.h"
#include "cocos2d.h"

Scene * MainMenu::createScene()
{
    // 'scene', 'layer' are autorelease objects
    Scene * scene = Scene::create();
    auto layer = MainMenu::create();
    scene->retain();
    layer->retain();
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool MainMenu::init()
{
    // init the super
    if ( !Layer::init() ) {
        
        return false;
    }
    
    loadMainMenu(this);
   
    return true;
}

bool MainMenu::loadMainMenu(Layer * activeLayer)
{
    if(!loadBackground())
    {
        CCLOG("Error loading MainMenu Bkgnd!\n");
        return false;
    }
    
    // create the Menu * object
    
    if(!createTitle())
    {
        CCLOG("Error loading MainMenu Title!\n");
        return false;
    }
    
    if(!createButtons())
    {
        CCLOG("Error loading MainMenu buttons!\n");
        return false;
    }
    
    //activeLayer->addChild(this, 0);
    return true;
}

bool MainMenu::loadBackground(void)
{
    Size iSize = Director::getInstance()->getWinSize();
    
    // create background
    colorLayer = LayerColor::create(Color4B(15,48,87,255), iSize.width, iSize.height);
    this->addChild(this->colorLayer, 1);
    
    return true;
}

bool MainMenu::createButtons(void)
{
    numItems = 3;
    menuButtons.reserve(numItems);
    
    // create items and push back on menuButtons vector
    label = MenuItemImage::create(GJ_Settings::gravityJamLabelPath, GJ_Settings::gravityJamLabelPath, MainMenu::titleButtonFunc);
    menuButtons.pushBack(label);
                                 
    playButton = MenuItemImage::create(GJ_Settings::playButtonNormPath, GJ_Settings::playButtonSelPath, MainMenu::playButtonFunc);
    menuButtons.pushBack(playButton);
    
    optionsButton = MenuItemImage::create(GJ_Settings::optButtonNormPath, GJ_Settings::optButtonSelPath, MainMenu::optionsButtonFunc);
    menuButtons.pushBack(optionsButton);
    
    quitButton = MenuItemImage::create(GJ_Settings::quitButtonNormPath, GJ_Settings::quitButtonSelPath, MainMenu::quitButtonFunc);
    menuButtons.pushBack(quitButton);
    
    theMenu = cocos2d::Menu::createWithArray(menuButtons);
    //theMenu->alignItemsInColumns(1, 1, 1, 1);
    theMenu->alignItemsVerticallyWithPadding(32.0);
    theMenu->retain();
    
    this->addChild(theMenu, 1);
    
    // how do we check this?
    if (!theMenu) {
        return false;
    }
    
    return true;
}

bool MainMenu::createTitle(void)
{
    
    return true;
}

bool MainMenu::cleanupMenu(void)
{
    theMenu->release();
    return true;
}

// button handlers

void MainMenu::titleButtonFunc(cocos2d::Ref* sender)
{
    CCLOG("Title button pressed.\n");
}

void MainMenu::playButtonFunc(cocos2d::Ref* sender)
{
    CCLOG("Play button pressed.\n");
    auto game = Level::createScene();
    auto transition = TransitionFade::create(1.0f, game);
    Director::getInstance()->replaceScene(transition);
    
}

void MainMenu::optionsButtonFunc(cocos2d::Ref* sender)
{
    CCLOG("Options button pressed.\n");
}

void MainMenu::quitButtonFunc(cocos2d::Ref* sender)
{
    CCLOG("Quit button pressed.\n");
}
