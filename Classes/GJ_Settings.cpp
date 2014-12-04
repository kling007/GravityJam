//
//  GJ_Settings.cpp
//  Gravity_Jam
//
//  Created by Garry Kling on 10/13/14.
//
//

#include <stdio.h>
#include "GJ_Settings.h"

// I know, I know, this is a bad way to do this.
// I'm still looking for a better way to manage global settings.
// Until then I'm making an ugly namespace to hold these values

namespace GJ_Settings {
    
    //http://www.colourlovers.com/palette/3058628/Flat_UI_Pallate

    Color3B gj_backgroundUIColor = Color3B(231,231,222);
    Color3B gj_hudUIColor = Color3B(205,203,166);
    Color3B gj_greenUIColor = Color3B(0,136,145);
    Color3B gj_blueUIColor = Color3B(0,88,122);
    Color3B gj_darkUIColor = Color3B(15,48,87);

    // http://www.colourlovers.com/palette/101556/Flat

    Color3B gj_redTileColor = Color3B(252,96,66);
    Color3B gj_blueTileColor = Color3B(44,130,201);
    Color3B gj_greenTileColor = Color3B(44,201,144);
    Color3B gj_yellowTileColor = Color3B(238,230,87);
    Color3B gj_orangeTileColor = Color3B(252,185,65);
        
    std::string playButtonNormPath = "Graphics/Buttons/playNormal.png";
    std::string playButtonSelPath = "Graphics/Buttons/playSelected.png";
    std::string optButtonNormPath = "Graphics/Buttons/optionsNormal.png";
    std::string optButtonSelPath = "Graphics/Buttons/optionsSelected.png";
    std::string quitButtonNormPath = "Graphics/Buttons/quitNormal.png";
    std::string quitButtonSelPath = "Graphics/Buttons/quitSelected.png";
    std::string gravityJamLabelPath = "Graphics/Buttons/label.png";
    std::string continueButtonNormPath = "Graphics/Buttons/continueNormal.png";
    std::string continueButtonSelPath = "Graphics/Buttons/continueSelected.png";

}
