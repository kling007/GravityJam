//
//  VisibleRect.h
//  GravityJam
//
//  Created by Garry Kling on 9/29/14.
//
//

#ifndef __GravityJam__VisibleRect__
#define __GravityJam__VisibleRect__

#include "cocos2d.h"

class VisibleRect
{
public:
    static cocos2d::Rect getVisibleRect();
    
    static cocos2d::Vec2 left();
    static cocos2d::Vec2 right();
    static cocos2d::Vec2 top();
    static cocos2d::Vec2 bottom();
    static cocos2d::Vec2 center();
    static cocos2d::Vec2 leftTop();
    static cocos2d::Vec2 rightTop();
    static cocos2d::Vec2 leftBottom();
    static cocos2d::Vec2 rightBottom();
    static void initWithRect(cocos2d::Rect * theRect);

private:
    static cocos2d::Rect * s_visibleRect;
};

#endif /* defined(__GravityJam__VisibleRect__) */
