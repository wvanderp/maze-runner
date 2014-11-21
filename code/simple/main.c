//include activitybot drive library, servocontrols
#include "abdrive.h" 
//include activitybot ping library, ultrasound.
#include "ping.h"
//include activitybot simpletools library, IR control.
#include "simpletools.h"

/*Documentation
Function drive_goto( , ) is used to finely turn the bot around.
*/

int main(){
//Initialization variables and settings
  double completeturn = 51.12;
  double rightw  = -25.56;
  double leftw = 25.56;
  drive_setRampStep(10);
  int turncounter = 0;
 
//main loop, alter at your own risk :3
 while(1){
   drive_ramp(64, 64);
    
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
    
   while(ping_cm(8) > 8)pause(5);
   drive_ramp(0, 0);
   drive_goto(leftw, rightw);
   turncounter++;
   
   while(ping_cm(8) < 5){
    drive_ramp(-32, -32);
   }      
 }   
}  
