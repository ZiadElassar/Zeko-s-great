#include <ESP32Servo.h>
#include <analogWrite.h>
//#include <tone.h>
#include <ESP32Tone.h>
#include <ESP32PWM.h>

//#include <Servo.h>


#include <L298N.h>
#include <analogWrite.h>
#include <WiFi.h>
#include <Wire.h>
int mode =2;
#define S1 39
#define S2 36
#define S1Trig 22
#define S2Trig 32
#define S3Trig 25
#define S1Echo 23
#define S2Echo 33
#define S3Echo 26
#define IN11  16
#define IN12  17
#define EN1 4
#define IN21  5
#define IN22  18
#define EN2  19
L298N motor1(EN1, IN11, IN12);
L298N motor2(EN2, IN21, IN22);
Servo gripper; 
Servo servo1;
Servo servo2;   
int pos1;
int pos1up = 100;
int pos1down = -90;    
int pos2;
int pos2up =120;
int pos2down=50;
int holding;
int counter;

int arm;




void setup() {
  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
  pinMode(S1Trig, OUTPUT);
  pinMode(S2Trig, OUTPUT);
  pinMode(S3Trig, OUTPUT);
  pinMode(S1Echo, INPUT);
  pinMode(S2Echo, INPUT);
  pinMode(S3Echo, INPUT);
 
 Serial.begin(115200);
 

 
   motor1.setSpeed(150);
   motor2.setSpeed(150);

  gripper.attach(14);
  servo1.attach(13); 
  servo2.attach(12);
  

}

void loop() {
if(arm==0){
armreset();
}

  if (mode ==1){


line_follower();


  }
  else if(mode==2) 
  
  {
    object_avoiding();
    
    }
}
void line_follower() {
  int centerSensor = sensorTwo();
  bool value1 = digitalRead(S1);
  bool value2 = digitalRead(S2);
  if (10 >= centerSensor) {
    if(centerSensor==10 & arm==0){
   
    Stop();
    Armstart();
    Serial.println("picking object");
    delay(500);
    motor1.setSpeed(150);
    motor2.setSpeed(150);}
    else if(centerSensor<10 & arm==0){
         motor1.setSpeed(100);
         motor2.setSpeed(100);
         Back();
    }
    else{Stop();}
    
    }

  if (value1 == 0 && value2 == 0) {
    motor1.forward();
    motor2.forward();
    if (10 >= centerSensor) {
    if(centerSensor==10 & arm==0){
   
    Stop();
    Armstart();
    Serial.println("picking object");
    delay(500);
    motor1.setSpeed(150);
    motor2.setSpeed(150);}
    else if(centerSensor<10 & arm==0){
         motor1.setSpeed(100);
         motor2.setSpeed(100);
         Back();
    }
    else{Stop();}
    
    }

  } else if (value1 == 1 && value2 == 1) {
    motor1.stop();
    motor2.stop();
    Armend();
  } else if (value1 == 0 && value2 == 1) {
    motor1.backward();
    motor2.forward();
  } else if (value1 == 1 && value2 == 0) {
    motor1.forward();
    motor2.backward();
  }
}

void object_avoiding() {
  
  int centerSensor = sensorTwo();
  Serial.println(centerSensor);
  int leftSensor = sensorOne();
  Serial.println(leftSensor);
  int rightSensor = sensorThree();
  Serial.println(rightSensor);
// Check the distance using the IF condition
  if (15 >= centerSensor|15>= leftSensor|15>=rightSensor) {
    Stop();
    delay(500);
    Back();
    Serial.println("Stop");
    delay(500);
    if (leftSensor > rightSensor) {
      left();
      Serial.println("Left");
      delay(200);
    } else {
      right();
      Serial.println("Right");
      delay(200);
    }
  }
  Serial.println("Forward");
  forward();
}

int sensorOne() {
  //pulse output
  digitalWrite(S1Trig, LOW);
  delayMicroseconds(4);
  digitalWrite(S1Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(S1Trig, LOW);

  long t = pulseIn(S1Echo, HIGH);//Get the pulse
  int cm = t / 29 / 2; //Convert time to the distance
  return cm; // Return the values from the sensor
}

//Get the sensor values
int sensorTwo() {
  //pulse output
  digitalWrite(S2Trig, LOW);
  delayMicroseconds(4);
  digitalWrite(S2Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(S2Trig, LOW);

  long t = pulseIn(S2Echo, HIGH);//Get the pulse
  int cm = t / 29 / 2; //Convert time to the distance
  return cm; // Return the values from the sensor
}

//Get the sensor values
int sensorThree() {
  //pulse output
  digitalWrite(S3Trig, LOW);
  delayMicroseconds(4);
  digitalWrite(S3Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(S3Trig, LOW);

  long t = pulseIn(S3Echo, HIGH);//Get the pulse
  int cm = t / 29 / 2; //Convert time to the distance
  return cm; // Return the values from the sensor
}

/*******************Motor functions**********************/
void forward() {
  motor1.forward();
  motor2.forward();

}
void left() {
  motor1.backward();
  motor2.forward();
 
}
void right() {
  motor1.forward();
  motor2.backward();
}
void Stop() {
  motor1.stop();
  motor2.stop();

}
void Back(){
 
  motor1.backward();
  motor2.backward();

}
void armreset()
{
pos1=pos1up;
servo1.write(pos1);
pos2=pos2up;
servo2.write(pos2);
gripper.write(-30);

}
void Armstart(){
  arm=1;


}

void Armend(){
  arm=0;

}
