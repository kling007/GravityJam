//
//  GJ_Settings.h
//  Gravity_Jam
//
//  Created by Garry Kling on 10/13/14.
//
//

#include "cocos2d.h"
#include <string>

USING_NS_CC;

#ifndef Gravity_Jam_GJ_Settings_h
#define Gravity_Jam_GJ_Settings_h


namespace GJ_Settings {

    // UI colors

    //http://www.colourlovers.com/palette/3058628/Flat_UI_Pallate

    extern Color3B gj_backgroundUIColor;
    extern Color3B gj_hudUIColor;
    extern Color3B gj_greenUIColor;
    extern Color3B gj_blueUIColor;
    extern Color3B gj_darkUIColor;

    // http://www.colourlovers.com/palette/101556/Flat

    extern Color3B gj_redTileColor;
    extern Color3B gj_blueTileColor;
    extern Color3B gj_greenTileColor;
    extern Color3B gj_yellowTileColor;
    extern Color3B gj_orangeTileColor;

    // file paths
    extern std::string playButtonNormPath;
    extern std::string playButtonSelPath;
    extern std::string optButtonNormPath;
    extern std::string optButtonSelPath;
    extern std::string quitButtonNormPath;
    extern std::string quitButtonSelPath;
    extern std::string gravityJamLabelPath;
    extern std::string continueButtonNormPath;
    extern std::string continueButtonSelPath;

}

#endif
