
#include <Servo.h>

Servo s1;
Servo s2;
Servo s3;
Servo s4;
Servo s5;
Servo s6;
Servo s7;
Servo s8; 


void setup() {
  s1.attach(2);
  s2.attach(3);
  s3.attach(4);
  s4.attach(5);
  s5.attach(6);
  s6.attach(7);
  s7.attach(8);
  s8.attach(9); 


  // initialize serial interface
  Serial.begin(9600);
}


void STAND() {
  s1.write(159);
  delay(15);
   s2.write(18);
  delay(15);
   s3.write(115);
  delay(15);
   s4.write(50);
  delay(15);
   s5.write(57);
  delay(15);
   s6.write(180);
  delay(15);
   s7.write(43);
  delay(15);
   s8.write(18);
  delay(15);
}


void loop() {

 

if (Serial.available() > 0){
  String jx = Serial.readString();

  String servoID = jx.substring(0,2);
  String servoAngleValue = jx.substring(3);
  int servoAngle = servoAngleValue.toInt();

Serial.println(servoID);
    Serial.println(servoAngle);
  
   if(servoID == "s1"){
    s1.write(servoAngle);
    delay(200);
   }
    if(servoID == "s2"){
    s2.write(servoAngle);
    delay(200);
   }
    if(servoID == "s3"){
    s3.write(servoAngle);
    delay(200);
   }
    if(servoID == "s4"){
    s4.write(servoAngle);
    delay(200);
   }
    if(servoID == "s5"){
    s5.write(servoAngle);
    delay(200);
   }
    if(servoID == "s6"){
    s6.write(servoAngle);
    delay(200);
   }
    if(servoID == "s7"){
    s7.write(servoAngle);
    delay(200);
   }
    if(servoID == "s8"){
    s8.write(servoAngle);
    delay(200);
   }
   
 }else{
  STAND();
 }


}
