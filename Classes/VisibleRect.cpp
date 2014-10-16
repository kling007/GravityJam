//
//  VisibleRect.cpp
//  GravityJam
//
//  Created by Garry Kling on 9/29/14.
//
//

#include "VisibleRect.h"

USING_NS_CC;

Rect * VisibleRect::s_visibleRect;

void VisibleRect::initWithRect(cocos2d::Rect * theRect)
{
    s_visibleRect = theRect;
}

Rect VisibleRect::getVisibleRect()
{
    return *s_visibleRect;
}

Vec2 VisibleRect::left()
{
    
    return Vec2(s_visibleRect->origin.x, s_visibleRect->origin.y+s_visibleRect->size.height/2);
}

Vec2 VisibleRect::right()
{
    
    return Vec2(s_visibleRect->origin.x+s_visibleRect->size.width, s_visibleRect->origin.y+s_visibleRect->size.height/2);
}

Vec2 VisibleRect::top()
{
    
    return Vec2(s_visibleRect->origin.x+s_visibleRect->size.width/2, s_visibleRect->origin.y+s_visibleRect->size.height);
}

Vec2 VisibleRect::bottom()
{
    
    return Vec2(s_visibleRect->origin.x+s_visibleRect->size.width/2, s_visibleRect->origin.y);
}

Vec2 VisibleRect::center()
{
    
    return Vec2(s_visibleRect->origin.x+s_visibleRect->size.width/2, s_visibleRect->origin.y+s_visibleRect->size.height/2);
}

Vec2 VisibleRect::leftTop()
{
    
    return Vec2(s_visibleRect->origin.x, s_visibleRect->origin.y+s_visibleRect->size.height);
}

Vec2 VisibleRect::rightTop()
{
    
    return Vec2(s_visibleRect->origin.x+s_visibleRect->size.width, s_visibleRect->origin.y+s_visibleRect->size.height);
}

Vec2 VisibleRect::leftBottom()
{
    
    return s_visibleRect->origin;
}

Vec2 VisibleRect::rightBottom()
{
    
    return Vec2(s_visibleRect->origin.x+s_visibleRect->size.width, s_visibleRect->origin.y);
}