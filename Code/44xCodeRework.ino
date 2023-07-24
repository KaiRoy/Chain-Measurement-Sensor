/*
Team 15 Chain stretch monitor Updated
2022-2023
Corbin: Calculation part
Tianhao: CAN transmission part
*/

/*
Open the code with Arduino IDE 1.
Set the environment: https://github.com/SpenceKonde/megaTinyCore
Reference: https://www.electronics-lab.com/project/getting-started-with-the-new-attiny-chips-programming-the-microchips-0-series-and-1-series-attiny-with-the-arduino-ide/
*/

#include <SPI.h>
#include <mcp_can.h> // source from: https://github.com/Seeed-Studio/Seeed_Arduino_CAN/tree/old
#include <Wire.h>


#define Uh_Oh 7  //chain too long!

// CAN VARIABLE
unsigned char stmp[14] {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

//Microcontroller VARIABLE
int OuterL = A5; // PWM
int InnerIR = A6; // ASYNC INTERRUPT
int OuterR = A7;

int Chainlength = 0;
const int exp_length = 6; // need to be changed according to chain model


// ------THE DECLARATION ZONE
float percent_error = 1.03;

unsigned long rightNow = micros(); //(current time)
const unsigned long TimeKeeper = 1000000; //1s -> 1,000,000 us
unsigned long Timelastseen = 0;

//Speed sensor time variables
unsigned long Time1 = 0; //(last time IR beam was “seen”)
unsigned long Time2 = 0; //(most recent “sighting” time of beam)

//Outer sensor time variables
unsigned long TimeAtR = 0;
unsigned long TimeAtL = 0;
unsigned long TimeLROffset = 0;

unsigned long howlong; //(“length” of time since most recent link)

bool chainStateSpeed = 0;
bool chainStateSpeedOld = 0;
bool chainStateLeft = 0;
bool chainStateLeftOld = 0;
bool chainStateRight = 0;
bool chainStateRightOld = 0;
bool beenawhile = 0;

//  NEW MEAT AND POTATOES VALS
float chainSpeed = 0; //in um/us
unsigned long offset = 0; //in um
long Total_Length = 0; //150um + offset
long IdealLength = 152; //38um*4 links

MCP_CAN CAN(SS);

void setup()
{
  pinMode(OuterL, INPUT);
  pinMode(OuterR, INPUT);
  pinMode(InnerIR, INPUT);
 
  pinMode(SS, OUTPUT);
    
  while (CAN_OK != CAN.begin(CAN_500KBPS))
  {
      Serial.println("CAN BUS init Failed");
      delay(100);
  }
  Serial.println("CAN BUS Shield Init OK!");
}

void loop()
{ 
  ///Getting chain states
  chainStateSpeed = digitalRead(InnerIR);
  chainStateRight = digitalRead(OuterR);
  chainStateLeft = digitalRead(OuterL);
  
  // Get deltaTime between OuterR and OuterL
  if (chainStateLeft == 1 && (chainStateLeft!=chainStateLeftOld)) {
    TimeAtL = micros();
    Timelastseen = micros(); //used to know if chain is likely outta here/stopped...
  }
  if (chainStateRight == 1 && (chainStateRight!=chainStateRightOld)) {
    TimeAtR = micros();
    Timelastseen = micros(); //...so that old sample thrown out if so
  }
  if (TimeAtR != 0 && TimeAtL != 0) {
    //both have a reading!
    TimeLROffset = abs(TimeAtL - TimeAtR);
  }

  // Calculate Speed from InnerIR
  if (chainStateSpeed != chainStateSpeedOld) {
    if(chainStateSpeed==1){
      Time1 = micros();
    }
    if(chainStateSpeed==0){
      Time2 = micros();
      howlong = Time2-Time1; //finding how long it took to travel 1000um
    }
    
    //situation where chain likely stopped (1000 may be too small)
    if (howlong > TimeKeeper) {
      chainSpeed = 0;
    }
    //situation where chain probably isnt stopped
    else {
      chainSpeed = 1000.0/howlong; // 1um / howlong(us)
    }
    
    stmp[0] = (howlong & 0xff000000UL) >> 24; //used for CAN troubleshooting
    stmp[1] = (howlong & 0x00ff0000UL) >> 16;
    stmp[2] = (howlong & 0x0000ff00UL) >> 8;
    stmp[3] = (howlong & 0x000000ffUL);

	offset = ((TimeLROffset * 1000) / howlong); // us x (um/us)

    if (chainSpeed != 0){
      stmp[4] = (TimeLROffset & 0xff000000UL) >> 24;
      stmp[5] = (TimeLROffset & 0x00ff0000UL) >> 16;
      stmp[6] = (TimeLROffset & 0x0000ff00UL) >> 8;
      stmp[7] = (TimeLROffset & 0x000000ffUL);
	  
      stmp[8] = (offset & 0xff000000UL) >> 24;
      stmp[9] = (offset & 0x00ff0000UL) >> 16;
      stmp[10] = (offset & 0x0000ff00UL) >> 8;
      stmp[11] = (offset & 0x000000ffUL);

      // if (Total_Length > IdealLength*percent_error) { //IdealLength = 152000um, percent_error is 1.03 is 3%
      //   //digitalWrite(Uh_Oh, HIGH);
      //   stmp[3] = 199; // send 199 if chain needs to be replaced
      // }
      // else{
      //   stmp[3] = 200; // send 200 if chain is good
      // }

      // TimeLROffset = 0; //reset
      // chainSpeed = 0;   //reset
    }
    //   else{
    //   Total_Length = 0;
    //   stmp[2] = 0;
    // }
  }


  // if(abs(micros()-Timelastseen)>TimeKeeper){ //again, 1000 may be too big
  //   beenawhile = 1;
  // }
  // if(abs(micros()-Timelastseen)<TimeKeeper){
  //   beenawhile = 0;
  // }
  // //this section is to make sure we aren't holding onto "outdated" info for LR stuff
  // if(beenawhile==1){
  //   chainSpeed = 0;
  //   TimeLROffset = 0;
  // }
  

  chainStateSpeedOld = chainStateSpeed;
  chainStateRightOld = chainStateRight;
  chainStateLeftOld = chainStateLeft;
  
  
  // Transmit part (CAN)
  CAN.sendMsgBuf(0x15, 0, 14, stmp);
  //delay(500);
}
