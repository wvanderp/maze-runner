/*
HC-SR04 Ping distance sensor]
VCC to arduino 5v GND to arduino GND
Echo to Arduino pin 13 Trig to Arduino pin 12
Red POS to Arduino pin 11
Green POS to Arduino pin 10
560 ohm resistor to both LED NEG and GRD power rail
More info at: http://goo.gl/kJ8Gl
Original code improvements to the Ping sketch sourced from Trollmaker.com
Some code and wiring inspired by http://en.wikiversity.org/wiki/User:Dstaub/robotcar
*/

#define trigPin 13
#define echoPin 12

int startdist = 0;

void setup() {
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

}

void loop() {
  long dis;
   dis = getDist();
  if(startdist == 0){
    startdist = dis;
    Serial.print("starting distance is: ");
    Serial.print(startdist);
    Serial.println(" cm");
  }
  
  if(dis - startdist < 5){
    comands(1);
  }
  if(dis - startdist > 5){
     comands(2);
  } 
  
  delay(500);
}

void comands(int com){
   switch(com){
      case 1:
       //drive
       Serial.println("drive");
     case 2: 
        //stop
        Serial.println("stop");
     case 3:
        //last drive
        Serial.println("last drive");
   } 
}

long getDist(){
  long duration, distance;
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
  //  delayMicroseconds(1000); - Removed this line
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  Serial.print(distance);
  Serial.println(" cm"); 
  return distance;
}

