/*
What: LEDLightBoxAlnitak - PC controlled lightbox implmented using the 
  Alnitak (Flip-Flat/Flat-Man) command set found here:
  https://www.optecinc.com/astronomy/catalog/alnitak/resources/Alnitak_GenericCommandsR4.pdf

Who: 
  Created By: Jared Wellman - jared@mainsequencesoftware.com
  Adapted to V4 protocol By: Igor von Nyssen - igor@vonnyssen.com

When: 
  Last modified:  2019/December/19


Typical usage on the command prompt:
Send     : >SOOO\r      //request state
Recieve  : *S19OOO\n    //returned state

Send     : >B128\r      //set brightness 128
Recieve  : *B19128\n    //confirming brightness set to 128

Send     : >JOOO\r      //get brightness
Recieve  : *B19128\n    //brightness value of 128 (assuming as set from above)

Send     : >LOOO\r      //turn light on (uses set brightness value)
Recieve  : *L19OOO\n    //confirms light turned on

Send     : >DOOO\r      //turn light off (brightness value should not be changed)
Recieve  : *D19OOO\n    //confirms light turned off.
*/

volatile int ledPin = 13;      // the pin that the LED is attached to, needs to be a PWM pin.
int brightness = 0;

enum devices
{
  FLAT_MAN_L = 10,
  FLAT_MAN_XL = 15,
  FLAT_MAN = 19,
  FLIP_FLAT = 99
};

enum motorStatuses
{
  STOPPED = 0,
  RUNNING
};

enum lightStatuses
{
  OFF = 0,
  ON
};

enum shutterStatuses
{
  NEITHER_OPEN_NOR_CLOSED = 0, // ie not open or closed...could be moving
  CLOSED,
  OPEN,
  TIMED_OUT
};


int deviceId = FLAT_MAN;
int motorStatus = STOPPED;
int lightStatus = OFF;
int coverStatus = NEITHER_OPEN_NOR_CLOSED;

void setup()
{
  // initialize the serial communication:
  Serial.begin(9600);
  // initialize the ledPin as an output:
  pinMode(ledPin, OUTPUT);
  analogWrite(ledPin, 0);
}

void loop() 
{
  handleSerial();
}


void handleSerial()
{
  if( Serial.available() >= 6 )  // all incoming communications are fixed length at 6 bytes including the \n
  {
    char* cmd;
    char* data;
    char temp[10];
    
    int len = 0;

    char str[20];
    memset(str, 0, 20);
    Serial.readBytesUntil('\r', str, 20);

  cmd = str + 1;
  data = str + 2;
  
  // useful for debugging to make sure your commands came through and are parsed correctly.
    if( false )
    {
      sprintf( temp, "cmd = >%c%s\n", cmd, data);
      Serial.write(temp);
    } 
    


    switch( *cmd )
    {
    /*
    Ping device
      Request: >POOO\r
      Return : *PiiOOO\n
        id = deviceId
    */
      case 'P':
      sprintf(temp, "*P%dOOO\n", deviceId);
      Serial.write(temp);
      break;

      /*
    Open shutter
      Request: >OOOO\r
      Return : *OiiOOO\n
        id = deviceId

      This command is only supported on the Flip-Flat!
    */
      case 'O':
      sprintf(temp, "*O%dOOO\n", deviceId);
      SetShutter(OPEN);
      Serial.write(temp);
      break;


      /*
    Close shutter
      Request: >COOO\r
      Return : *CiiOOO\n
        id = deviceId

      This command is only supported on the Flip-Flat!
    */
      case 'C':
      sprintf(temp, "*C%dOOO\n", deviceId);
      SetShutter(CLOSED);
      Serial.write(temp);
      break;

    /*
    Turn light on
      Request: >LOOO\r
      Return : *LiiOOO\n
        id = deviceId
    */
      case 'L':
      sprintf(temp, "*L%dOOO\n", deviceId);
      Serial.write(temp);
      lightStatus = ON;
      analogWrite(ledPin, brightness);
      break;

    /*
    Turn light off
      Request: >DOOO\r
      Return : *DiiOOO\n
        id = deviceId
    */
      case 'D':
      sprintf(temp, "*D%dOOO\n", deviceId);
      Serial.write(temp);
      lightStatus = OFF;
      analogWrite(ledPin, 0);
      break;

    /*
    Set brightness
      Request: >Bxxx\r
        xxx = brightness value from 000-255
      Return : *Biiyyy\n
        id = deviceId
        yyy = value that brightness was set from 000-255
    */
      case 'B':
      brightness = atoi(data);    
      if( lightStatus == ON ) 
        analogWrite(ledPin, brightness);   
      sprintf( temp, "*B%d%03d\n", deviceId, brightness );
      Serial.write(temp);
        break;

    /*
    Get brightness
      Request: >JOOO\r
      Return : *Jiiyyy\n
        id = deviceId
        yyy = current brightness value from 000-255
    */
      case 'J':
        sprintf( temp, "*J%d%03d\n", deviceId, brightness);
        Serial.write(temp);
        break;
      
    /*
    Get device status:
      Request: >SOOO\r
      Return : *SidMLC\n
        id = deviceId
        M  = motor status( 0 stopped, 1 running)
        L  = light status( 0 off, 1 on)
        C  = Cover Status( 0 moving, 1 closed, 2 open, 3 timed out)
    */
      case 'S': 
        sprintf( temp, "*S%d%d%d%d\n",deviceId, motorStatus, lightStatus, coverStatus);
        Serial.write(temp);
        break;

    /*
    Get firmware version
      Request: >VOOO\r
      Return : *Vii001\n
        id = deviceId
    */
      case 'V': // get firmware version
      sprintf(temp, "*V%d002\n", deviceId);
      Serial.write(temp);
      break;
    }    

  while( Serial.available() > 0 )
    Serial.read();

  }
}

void SetShutter(int val)
{
  if( val == OPEN && coverStatus != OPEN )
  {
    coverStatus = OPEN;
    // TODO: Implement code to OPEN the shutter.
  }
  else if( val == CLOSED && coverStatus != CLOSED )
  {
    coverStatus = CLOSED;
    // TODO: Implement code to CLOSE the shutter
  }
  else
  {
    // TODO: Actually handle this case
    coverStatus = val;
  }
  
}
