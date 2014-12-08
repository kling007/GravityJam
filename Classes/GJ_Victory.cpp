//
//  GJ_Victory.cpp
//  Gravity_Jam
//
//  Created by Garry Kling on 11/24/14.
//
//

#include "GJ_Victory.h"
#include "GJ_Level.h"

bool Victory::init()
{
    // init the super
    if ( !LayerColor::initWithColor(Color4B(205, 203, 166, 120))) {
        
        return false;
    }
    
    Size parentVisibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // this->setVisible(false);
    
    dialog = Sprite::create("Graphics/levelComplete.png");
    if (!dialog) {
        printf("Error opening levelComplete.png\n");
        return false;
    }
    
    Size rawSize = dialog->getBoundingBox().size;
    
    dialog->setAnchorPoint(Vec2(0,0));
    createMenu();
    
    dialog->setScale((rawSize.width/parentVisibleSize.width)*0.66);
    
    Size dialogSize = dialog->getBoundingBox().size;
    
    posX = (parentVisibleSize.width-dialogSize.width)/2.0;
    posY = (parentVisibleSize.height-dialogSize.height)/2.0;
    
    auto move = MoveTo::create(1.0, Vec2(posX, posY));
    auto bounceIn = EaseElasticOut::create(move->clone());
    dialog->setPosition(Vec2(posX, -dialogSize.height));
    
    
    // is there any way to fade this in nicely?
    // How do we scale to fit the phone size?
    this->addChild(dialog, 1);
    dialog->runAction(bounceIn);
    return true;
}

bool Victory::createMenu(void)
{
    Vec2 position;
    Size winSize = dialog->getBoundingBox().size;
    
    
    position = Vec2(winSize.width/2.0, winSize.height/6.0);
    
    theButton = MenuItemImage::create(GJ_Settings::continueButtonNormPath, GJ_Settings::continueButtonSelPath, CC_CALLBACK_1(Victory::clickButton, this));
    
    theMenu = Menu::create(theButton, NULL);
    theMenu->setPosition(position);
    
    if(theMenu)
    {
        dialog->addChild(theMenu);
        return true;
    }
    
    return false;
    
}

void Victory::endScreen(float dt)
{
    Node * parent = this->getParent();
    Level * theLevel = dynamic_cast<Level*>(parent);
    if(theLevel != nullptr)
    {
        theLevel->endVictoryScreen(0.0);
    }
}

void Victory::clickButton(Ref * sender)
{
    
    Size dialogSize = dialog->getBoundingBox().size;
    auto move = MoveTo::create(1.0, Vec2(posX, -dialogSize.height));
    auto bounceOut = EaseElasticOut::create(move->clone());
    
    
    dialog->runAction(bounceOut);
    this->runAction(FadeOut::create(1.0));
    scheduleOnce(schedule_selector(Victory::endScreen), 0.5);
    
    // else, don't do much else
    // printf("Click!!!\n");
    
}