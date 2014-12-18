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

//Setup pin variables for oversight
static int pinU = 8;
static int pinIRL = 9;
static int pinIRR = 10;
static int pinIRin0 = 2;
static int pinIRin1 = 3; 

//More memory than needed is assigned!
//The CPU should have enough registers and other memory sources for this. 
unsigned int cogStack = 32;
unsigned int ExtraStackCog1 = 24;
unsigned int ExtraStackCog2 = 32;
static volatile int distance;
static volatile int irLstate = 0;
static volatile int irRstate = 0;


void printDistance(void){
  while(1)
  {   
    print("distance = " + distance);
    pause(500);          
  }
}

  void irSensorL(void){
    while(1){
      //freqout (int pin, int msTime, int frequency)
      freqout(pinIRL, 1, 38000);
      irLstate = input(pinIRin0);
    }
  }

  void irSensorR(void){
    while(1){
      //freqout (int pin, int msTime, int frequency)
      freqout(pinIRR, 1, 38000);
      irRstate = input(pinIRin1);
    }
  }

void irPrint(void){
  while(1){
    print("L = " + irLstate);
  }
}  

void uSensor(void){
  while(1){
    
    drive_ramp(64, 64);
    distance = ping_cm(pinU);

    while(ping_cm(pinU) < 10){

      if(ping_cm(pinU) < 5){

        while(ping_cm(pinU) < 5){

        drive_ramp(-32, -32);
        pause(500);
        }
      }
      drive_ramp(0, 0);
      drive_goto(leftw, rightw);
      turncounter++;
    }
  } 
}

void turnCount(void){
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

//main loop, alter at your own risk :3
int main(){
  cogstart(&uSensor, NULL, cogStack+ExtraStackCog1, sizeof(cogStack));
  cogstart(&irSensorL, NULL, cogStack, sizeof(cogStack));
  cogstart(&irSensorR, NULL, cogStack, sizeof(cogStack));
  cogstart(&printDistance, NULL, cogStack, sizeof(cogStack));
  cogstart(&turnCount, NULL, cogStack+ExtraStackCog2, sizeof(cogStack));
}  
