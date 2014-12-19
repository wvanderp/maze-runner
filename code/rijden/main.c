//include activitybot drive library, servocontrols
#include "abdrive.h" 
//include activitybot ping library, ultrasound.
#include "ping.h"
//include activitybot simpletools library, IR control.
#include "simpletools.h"

/*Documentation
Function drive_goto( , ) is used to finely turn the bot around.
*/


//Initialization global variables and settings
static volatile double completeturn = 51.12;
static volatile double rightw  = -25.56;
static volatile double leftw = 25.56;
static volatile int turncounter = 0;

//Setup pin variables for oversight.
static int pinU = 8;
static int pinIRL = 9;
static int pinIRR = 11;
static int pinIRinL = 2;
static int pinIRinR = 10; 

//More memory than needed is assigned!
//The CPU should have enough registers and other memory sources for this. 
int cogStack0 = 40;
int cogStack1 = 40;
int cogStack2 = 40;
int cogStack3 = 40;
int cogStack4 = 64;
int cogStack5 = 48;
//unsigned int ExtraStackCog1 = 24;
//unsigned int ExtraStackCog2 = 32;
//general purpose static integer values for navigational functions.
static volatile int distance;
static volatile int irLstate = 0;
static volatile int irRstate = 0;
//4400Hz seems to detect close walls.
//A polling check has to be defined to counter false positives.
static int irFreq = 4400;
//The standard delay of 1ms is used.
static int irDelay = 1;

void printDistance(void *v){
  while(1)
  {   
    print("distance = " + distance);
    pause(500);          
  }
}

  void irSensorL(void *v){
    while(1){
      //freqout(int pin, int msTime, int frequency)
      freqout(pinIRL, irDelay, irFreq);
      irLstate = input(pinIRinL);
    }
  }

  void irSensorR(void *v){
    while(1){
      //freqout(int pin, int msTime, int frequency)
      freqout(pinIRR, irDelay, irFreq);
      irRstate = input(pinIRinR);
    }
  }

void irPrint(void *v){
  while(1){
    print("L = " + irLstate);
    print("R = " + irRstate);
  }
}  

void uSensor(void *v){
  while(1){
    distance = ping_cm(pinU);
    pause(200);
  }
} 


void navigate(void *v){
  while(1){
    drive_ramp(64, 64);

    while(distance < 10){

      if(distance < 5){

        while(distance < 5){

        drive_ramp(-32, -32);
        pause(200);
        }
      }
      drive_ramp(0, 0);
      drive_goto(leftw, rightw);
      turncounter++;
    }
  }
}

void turnCount(void *v){
  //check the turn counter and reverse direction
   if(turncounter == 2){
     turncounter = 0;
     if(leftw == -25.56){
       abs(leftw);
       rightw = rightw<=0? rightw:0-rightw;
     }else{
        leftw = leftw<=0? leftw:0-leftw;
        abs(rightw);
     }       
   }
}

//main, alter at your own risk :3
int main(){
  int* coginfo0 = cog_run(&uSensor, sizeof(cogStack0));
  int* coginfo1 = cog_run(&irSensorL, sizeof(cogStack1));
  int* coginfo2 = cog_run(&irSensorR, sizeof(cogStack2));
  int* coginfo3 = cog_run(&printDistance, sizeof(cogStack3));
  int* coginfo4 = cog_run(&navigate, sizeof(cogStack4));
  int* coginfo5 = cog_run(&turnCount, sizeof(cogStack5));
}  
