#!/bin/bash
# this script is for outputting all of the needed resolutions (that I know of) for an iOS app in Cocos2d-x

mkdir icon_output

filenames=(icon-29 icon-40 icon-50 icon-57 icon-58 icon-60@2x icon-72 icon-76 icon-76@2x icon-80 icon-100 icon-114 icon-120 icon-144 icon-152 icon-180 Icon-Small-40 Icon-Small-40@2x Icon-Small Icon-Small@2x iTunesArtwork iTunesArtwork@2x)
icon_res=(29 40 50 57 58 120 72 76 152 80 100 114 120 144 152 180 40 80 29 58 512 1024)

for((i=0; i<${#icon_res[@]}; i++)) 
do 
  /Applications/Inkscape.app/Contents/Resources/bin/inkscape -z -e ./icon_output/${filenames[$i]}.png -w ${icon_res[i]} -h ${icon_res[i]} $1
done

# $2 is the wide iPhone 5/6 version
filenamesWide=(Default-568h Default-568h@2x Default-667 Default-667h@2x Default-736h Default-736h@3x)
icon_resX=(320 640 375 750 414 1224)
icon_resY=(568 1136 667 1334 736 2208)

for((i=0; i<${#icon_resX[@]}; i++)) 
do 
  /Applications/Inkscape.app/Contents/Resources/bin/inkscape -z -e ./icon_output/${filenamesWide[$i]}.png -w ${icon_resX[i]} -h ${icon_resY[i]} $2
done