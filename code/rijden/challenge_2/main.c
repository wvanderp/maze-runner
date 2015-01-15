#include "simpletools.h"
#include "abdrive.h"
#include "ping.h"

int bh = 10;
int spin = 9;
int finalLenght = 2;

int main(){
 while(1){
 drive_ramp(20, 20);
 if (ping_cm(spin) > bh){
   drive_ramp(0, 0);
 finalApproach();
 break;//now its broken
 }
 }
}

int finalApproach(){
 drive_goto(finalLenght, finalLenght);
 return 1;
}