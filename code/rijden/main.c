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

//Define base stack size 
int stack = 32;
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
    simpleterm_open();  
    print("%cUltrasound distance = %d cm \n IR detection L = %d R = %d%c", HOME, distance, irLstate, irRstate, CLREOL);
    simpleterm_close();
    pause(100);          
  }
}

  void irSensorL(void *v){
    while(1){
      //freqout(int pin, int msTime, int frequency)
      freqout(pinIRL, irDelay, irFreq);
      irLstate = input(pinIRinL);
      pause(100);
    }
  }

  void irSensorR(void *v){
    while(1){
      //freqout(int pin, int msTime, int frequency)
      freqout(pinIRR, irDelay, irFreq);
      irRstate = input(pinIRinR);
      pause(100);
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
        return;
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
  simpleterm_close();
  int* coginfo0 = cog_run(&uSensor, sizeof(stack));
  int* coginfo1 = cog_run(&irSensorL, sizeof(stack));
  int* coginfo2 = cog_run(&irSensorR, sizeof(stack));
  int* coginfo3 = cog_run(&printDistance, sizeof(stack));
  int* coginfo4 = cog_run(&navigate, sizeof(stack+32));
  int* coginfo5 = cog_run(&turnCount, sizeof(stack+16));
}  
