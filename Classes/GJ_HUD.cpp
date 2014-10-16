//
//  GJ_HUD.cpp
//  GravityJam
//
//  Created by Garry Kling on 10/1/14.
//
//

#include "GJ_HUD.h"


USING_NS_CC;

bool GJ_HUD::init()
{
    // sizing everything up
    
    Size winSize = Director::getInstance()->getVisibleSize();
    hudSizeX = winSize.width*0.25;
    hudSizeY = winSize.height;
    hudLineSpacing = hudSizeY*1/18;
    textCenter = hudSizeX*0.5;
    textLeftMargin = hudSizeX*0.125;
    textRightMargin = hudSizeX - textLeftMargin;
    labelFontSize = 16;
    
    // intialize the positions with the proportions I've set out
    // if you know of a better way to do positioning, let me know
    
    gameLabelPos = Point(textCenter, hudSizeY-(hudLineSpacing*1));
    scoreLabelPos = Point(textLeftMargin, hudSizeY-(hudLineSpacing*3));
    scorePos= Point(textRightMargin,hudSizeY-(hudLineSpacing*4));
    levelLabelPos = Point(textLeftMargin, hudSizeY-(hudLineSpacing*5));
    levelPos= Point(textRightMargin,hudSizeY-(hudLineSpacing*6));
    timeLabelPos = Point(textLeftMargin, hudSizeY-(hudLineSpacing*7));
    timePos= Point(textRightMargin,hudSizeY-(hudLineSpacing*8));
    bonusLabelPos = Point(textLeftMargin, hudSizeY-(hudLineSpacing*9));
    bonusPos = Point(textRightMargin,hudSizeY-(hudLineSpacing*10));
    
    
    if (LayerColor::initWithColor(Color4B(GJ_Settings::gj_hudUIColor), hudSizeX, hudSizeY))
    {
        this->setAnchorPoint(Vec2(0,0));
        this->setPosition(Vec2(winSize.width-hudSizeX,0));
        
        // Gravity jam gameLabel
        this->gameLabel = Label::createWithTTF("Gravity Jam","fonts/sofachrome_rg.ttf", 18*4, Size(hudSizeX, hudLineSpacing), TextHAlignment::CENTER);
        gameLabel->retain();
        gameLabel->setScale(0.25);
        gameLabel->setColor(GJ_Settings::gj_backgroundUIColor);
        gameLabel->setPosition(gameLabelPos);
        this->addChild(gameLabel);
        
        // the score
        scoreLabelString = "Score: ";
        this->scoreLabel = Label::createWithTTF(scoreLabelString, "fonts/sofachrome_rg.ttf", labelFontSize*4, Size(hudSizeX, hudLineSpacing), TextHAlignment::LEFT);
        scoreLabel->retain();
        scoreLabel->setScale(0.25);
        scoreLabel->setColor(GJ_Settings::gj_backgroundUIColor);
        scoreLabel->setAnchorPoint(Vec2(0,0));
        scoreLabel->setPosition(scoreLabelPos);
        this->addChild(scoreLabel);
        
        // just for now - set the score so we can see a big number
        theScore.setScore(892537291);
        scoreString = theScore.getString();
        this->scoreIndicator = Label::createWithTTF(scoreString, "fonts/sofachrome_rg.ttf", labelFontSize*4, Size(hudSizeX, hudLineSpacing), TextHAlignment::RIGHT);
        scoreIndicator->retain();
        scoreIndicator->setScale(0.25);
        scoreIndicator->setColor(GJ_Settings::gj_backgroundUIColor);
        scoreIndicator->setAnchorPoint(Vec2(1,1));
        scoreIndicator->setPosition(scorePos);
        this->addChild(scoreIndicator);
        
        // the Bonus
        
        this->bonusLabel = Label::createWithTTF("Bonus", "fonts/sofachrome_rg.ttf", labelFontSize*4, Size(hudSizeX, hudLineSpacing), TextHAlignment::LEFT);
        bonusLabel->retain();
        bonusLabel->setScale(0.25);
        bonusLabel->setColor(GJ_Settings::gj_backgroundUIColor);
        bonusLabel->setAnchorPoint(Vec2(0,0));
        bonusLabel->setPosition(bonusLabelPos);
        this->addChild(bonusLabel);
        
        // just for now - set the bonus so we can see a big number
        this->bonusIndicator = Label::createWithTTF("80000", "fonts/sofachrome_rg.ttf", labelFontSize*4, Size(hudSizeX, hudLineSpacing), TextHAlignment::RIGHT);
        bonusIndicator->retain();
        bonusIndicator->setScale(0.25);
        bonusIndicator->setColor(GJ_Settings::gj_backgroundUIColor);
        bonusIndicator->setAnchorPoint(Vec2(1,1));
        bonusIndicator->setPosition(bonusPos);
        this->addChild(bonusIndicator);
        
        // the time (simulated)
        
        this->timeLabel = Label::createWithTTF("Time:", "fonts/sofachrome_rg.ttf", labelFontSize*4, Size(hudSizeX, hudLineSpacing), TextHAlignment::LEFT);
        timeLabel->retain();
        timeLabel->setScale(0.25);
        timeLabel->setColor(GJ_Settings::gj_backgroundUIColor);
        timeLabel->setAnchorPoint(Vec2(0,0));
        timeLabel->setPosition(timeLabelPos);
        this->addChild(timeLabel);
        
        // just for now - set the bonus so we can see a big number
        this->timeIndicator = Label::createWithTTF("00:00:01", "fonts/sofachrome_rg.ttf", labelFontSize*4, Size(hudSizeX, hudLineSpacing), TextHAlignment::RIGHT);
        timeIndicator->retain();
        timeIndicator->setScale(0.25);
        timeIndicator->setColor(GJ_Settings::gj_backgroundUIColor);
        timeIndicator->setAnchorPoint(Vec2(1,1));
        timeIndicator->setPosition(timePos);
        this->addChild(timeIndicator);
        
        // the level (simulated)
        
        this->levelLabel = Label::createWithTTF("Level:", "fonts/sofachrome_rg.ttf", labelFontSize*4, Size(hudSizeX, hudLineSpacing), TextHAlignment::LEFT);
        levelLabel->retain();
        levelLabel->setScale(0.25);
        levelLabel->setColor(GJ_Settings::gj_backgroundUIColor);
        levelLabel->setAnchorPoint(Vec2(0,0));
        levelLabel->setPosition(levelLabelPos);
        this->addChild(levelLabel);
        
        // just for now - set the bonus so we can see a big number
        this->levelIndicator = Label::createWithTTF("1 || 1", "fonts/sofachrome_rg.ttf", labelFontSize*4, Size(hudSizeX, hudLineSpacing), TextHAlignment::RIGHT);
        levelIndicator->retain();
        levelIndicator->setScale(0.25);
        levelIndicator->setColor(GJ_Settings::gj_backgroundUIColor);
        levelIndicator->setAnchorPoint(Vec2(1,1));
        levelIndicator->setPosition(levelPos);
        this->addChild(levelIndicator);
        
        return true;
    }
    else
    {
        return false;
    }
}

GJ_HUD::~GJ_HUD()
{
    if (gameLabel)
    {
        gameLabel->release();
        gameLabel = NULL;
    }
    
    if (scoreLabel)
    {
        gameLabel->release();
        gameLabel = NULL;
    }
    
    return;
}