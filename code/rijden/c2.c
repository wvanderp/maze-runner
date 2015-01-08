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

//Setup pin variables for oversight and abstract logic.
static int pinU = 8; 

//Define base stack size 
int stack = 32;
//general purpose static integer values for navigational functions.
static volatile int distance;

//printDistance prints the main sensor readings to the console in a human readable format.
//The data is refreshed every 100ms (miliseconds).
void printDistance(void *v){
  while(1)
  { 
    simpleterm_open();  
    print("%cUltrasound distance = %d cm \n IR detection: L = %d R = %d%c", HOME, distance, irLstate, irRstate, CLREOL);
    simpleterm_close();
    pause(100);          
  }
}

//uSensor invokes the ping.h library to drive the ping sensor and to get the distance back in centimeters.
//The distance is stores in the field named distance.
//The ping is executed every 200ms this is default and by design
//The sensor will give more inaccurate readings(as if it gets overloaded) when the delay is lowered below 200ms. 
void uSensor(void *v){
  while(1){
    distance = ping_cm(pinU);
    pause(200);
  }
} 

//navigate incorperates all the movement functions, it makes the activitybot physically move, stop and turn.
//This method contains the primary decision making algorithem for movement.
//Therefore it requires the most memory on the stack.
void navigate(void *v){
  while(1){
    drive_ramp(8, 8);
    while(distance > 5 ){
      drive_ramp(0, 0);
      if(distance < 5){
        return;
      }
    }              
  }
}
//turnCount is an support method for the navigation method, it does not move the Activitybot!
//It only tracks the time of turns it tooks and changes fields associated with this state.
/*
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
*/
//Main, alter at your own risk!
//The cog_run method launches a function into one of the processors (total of eight) of the propeller board
//and assigns it with memory in the form of a stack size.
//The cog_run method enables multithreaded code as you might have noticed.
//The default stack size is 32 memory addresses. 
//Each and every method is an infinite while loop that is launched into an 
//static int variable for later data manipulation.
//simpleterm_close(); is invoked as a failsafe to forekeep the SimpleIDE console from getting confused
//in case some process was still writing to it.
//the printDistance method is responsible for invoking simpleter_open();
//Which in turn opens the terminal connection with the IDE.
int main(){
  simpleterm_close();
  int* coginfo0 = cog_run(&uSensor, sizeof(stack));
  int* coginfo1 = cog_run(&printDistance, sizeof(stack+16));
  int* coginfo2 = cog_run(&navigate, sizeof(stack+64));
}  
