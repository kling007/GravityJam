//
//  GJ_HUD.h
//  GravityJam
//
//  Created by Garry Kling on 10/1/14.
//
//

#ifndef __GravityJam__GJ_HUD__
#define __GravityJam__GJ_HUD__

#include "cocos2d.h"
#include "GJ_Score.h"
#include <string>
#include "GJ_Settings.h"

USING_NS_CC;

class GJ_HUD : public cocos2d::LayerColor {

public:
    bool init();
    ~GJ_HUD();
    
    std::string scoreLabelString;
    std::string scoreString;
    
    Score theScore;
    int level;
    int stage;
    
    // some size vars 
    GLfloat hudSizeX, hudSizeY, textCenter, textLeftMargin, textRightMargin, hudLineSpacing;
    int labelFontSize;
    
    // positions for the various label nodes
    Point gameLabelPos, scoreLabelPos, levelLabelPos, timeLabelPos, bonusLabelPos;
    Point scorePos, levelPos, timePos, bonusPos;
    
    CREATE_FUNC(GJ_HUD);
    
private:
    Label * gameLabel;
    Label * scoreLabel;
    Label * scoreIndicator;
    Label * levelLabel;
    Label * levelIndicator;
    Label * timeLabel;
    Label * timeIndicator;
    Label * bonusLabel;
    Label * bonusIndicator;
};


#endif /* defined(__GravityJam__GJ_HUD__) */
