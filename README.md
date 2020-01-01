## What 
LEDLightBoxAlnitak - PC controlled lightbox implmented using the Alnitak (Flip-Flat/Flat-Man) command set found here: https://www.optecinc.com/astronomy/catalog/alnitak/resources/Alnitak_GenericCommandsR4.pdf

## Who
* Created By: Jared Wellman - jared@mainsequencesoftware.com
* Adapted to V4 protocol By: Igor von Nyssen - igor@vonnyssen.com


## When
  Last modified:  2019/December/19

Typical usage on the command prompt below. Please note that commands end in CR '\r' and responses end in LF '\n'. Neither end in both, so do not use Serial.println. The character in `OOO` is the letter O, not the digit 0.

Send     : `>SOOO\r`      //request state\
Recieve  : `*S19OOO\n`    //returned state\
Send     : `>B128\r`      //set brightness 128\
Recieve  : `*B19128\n`    //confirming brightness set to 128\
Send     : `>JOOO\r`      //get brightness\
Recieve  : `*B19128\n`    //brightness value of 128 (assuming as set from above)\
Send     : `>LOOO\r`      //turn light on (uses set brightness value)\
Recieve  : `*L19OOO\n`    //confirms light turned on\
Send     : `>DOOO\r`      //turn light off (brightness value does not change, just the state of the light)\
Recieve  : `*D19OOO\n`    //confirms light turned off
