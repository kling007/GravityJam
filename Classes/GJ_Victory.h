//
//  GJ_Victory.h
//  Gravity_Jam
//
//  Created by Garry Kling on 11/24/14.
//
//

#ifndef __Gravity_Jam__GJ_Victory__
#define __Gravity_Jam__GJ_Victory__

#include <stdio.h>
#include "cocos2d.h"
#include "GJ_Settings.h"


USING_NS_CC;


class Victory : public cocos2d::LayerColor
{
public:
    virtual bool init();
    bool createMenu(void);
    void endScreen(void);
    
    //EventListenerTouchOneByOne * touchListener;
    void clickButton(Ref * sender);
    // bool setupTouches();
    
    Vec2 touchBegin;
    Vec2 touchEnd;
    
    CREATE_FUNC(Victory);
private:

    Menu * theMenu;
    MenuItem * theButton;
    Sprite * dialog;
    
};

#endif /* defined(__Gravity_Jam__GJ_Victory__) */
