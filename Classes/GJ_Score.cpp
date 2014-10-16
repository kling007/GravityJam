//
//  GJ_Score.cpp
//  GravityJam
//
//  Created by Garry Kling on 10/3/14.
//
//

#include "GJ_Score.h"

int Score::getScore()
{
    return theScore;
}

void Score::addToScore(int toAdd)
{
    theScore += toAdd;
}

void Score::deductFromScore(int toDeduct)
{
    theScore -= toDeduct;
}

void Score::setScore(int newScore)
{
    theScore = newScore;
}

std::string Score::getString()
{
    return std::to_string(theScore);
}