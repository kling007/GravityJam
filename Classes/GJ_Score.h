//
//  GJ_Score.h
//  GravityJam
//
//  Created by Garry Kling on 10/3/14.
//
//

#ifndef __GravityJam__GJ_Score__
#define __GravityJam__GJ_Score__

#include <string>

class Score {
 
public:
    // the Score
    int theScore;

    // methods
    int getScore();
    void addToScore(int toAdd);
    void deductFromScore(int toDeduct);
    void setScore(int newScore);
    
    std::string getString();
};
#endif /* defined(__GravityJam__GJ_Score__) */
