//include activitybot drive library, servocontrols
#include "abdrive.h" 
//include activitybot ping library, ultrasound.
#include "ping.h"
//include activitybot simpletools library, IR control.
#include "simpletools.h"
//include activitybot mstimer library, time monitoring.
#include "mstimer.h"
/*Documentation of general methods:
Method drive_goto() is used to finely turn the bot around.
drive_speed() is used to set the drive speed and to make the activitybot stop.
*/


//Initialization global variables and settings
static volatile double completeturn = 51.12;
static volatile double rightw  = 25.56;
static volatile double leftw = 25.56;
static volatile int _time;
static int openViewC = 0;
//static volatile int turncounter = 0;

//Setup pin variables for oversight and abstract logic.
static int pinU0 = 8;
static int pinU1 = 9;

//Define base stack size 
int stack = 32;
//general purpose static integer values for navigational functions.
static volatile int distanceFront;
static volatile int distanceLeft;

//printDistance prints the main sensor readings to the console in a human readable format.
//The data is refreshed every 100ms (miliseconds).
void printDistance(void *v){
  while(1){ 
    simpleterm_open();  
    print("%c Ultrasound distance F = %d cm \n distance L = %d%c cm", HOME, distanceFront, distanceLeft, CLREOL);
    simpleterm_close();
    pause(200);          
  }
}

//uSensor invokes the ping.h library to drive the ping sensor and to get the distance back in centimeters.
//The distance is stores in the field named distance.
//The ping is executed every 200ms this is default and by design
//The sensor will give more inaccurate readings(as if it gets overloaded) when the delay is lowered below 200ms. 
void uSensor(void *v){
  while(1){
    distanceFront = ping_cm(pinU0);
    pause(100);
    distanceLeft = ping_cm(pinU1);
    pause(100);
  }
} 
//openSpaceCheck simply extends the navigation function, by making the main algorithem aware of large open spaces.
//It checks for large open spaces periodically.
//and navigates to them if one is found.
void openSpaceCheck(){
  if(_time = 60000){
    _time = 0;
    mstime_set(0);
    if(distanceLeft > 18){
      if(openViewC >= 1){
        openViewC = 0;
      }else{
        openViewC++;
        drive_speed(0, 0);
        pause(1000);
        drive_goto(leftw, -rightw);
        pause(10);
        drive_speed(32, 32);
        pause(1500);
      }
    }
  }  
}

//navigate incorperates all the movement functions, it makes the activitybot physically move, stop and turn.
//This method contains the primary decision making algorithem for movement.
//Therefore it requires the most memory on the stack.
void navigate(void *v){
  int openViewC = 0;
  while(1){
    drive_speed(32, 32);

    if(distanceFront < 5){
      if(distanceLeft < 8){
      drive_speed(0, 0);
      pause(1000);
      drive_goto(-6, -6);
      pause(1000);
      drive_goto(-leftw, rightw);
      pause(10);
      }else{
        drive_speed(0, 0);
        pause(1000);
        drive_goto(-6, -6);
        pause(1000);
        drive_goto(leftw, -rightw);
        pause(10);
      }
    }
    openSpaceCheck();
  }
}

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
  int time;
  mstime_start();
  int* coginfo0 = cog_run(&uSensor, sizeof(stack+32));
  //int* coginfo1 = cog_run(&printDistance, sizeof(stack+16));
  int* coginfo2 = cog_run(&navigate, sizeof(stack+96));
  while(1){
    time = mstime_get();
    _time = time;
    if(_time = 60000){
      _time = 0;
      time = 0;
    }
    pause(100);
  }
}  