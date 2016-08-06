/* Sweep
 by BARRAGAN <http://barraganstudio.com> 
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://arduino.cc/en/Tutorial/Sweep
*/ 

#include <Servo.h> 
Servo myservo2;
Servo myservo;  // create servo object to control a servo 
                // twelve servo objects can be created on most boards
 
void setup() 
{ 
  myservo.attach(10);  // attaches the servo on pin 9 to the servo object 
  myservo2.attach(6);
 } 
 
void loop() 
{ 
  /*
  for(pos = 0; pos <= 180; pos += 1) // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(1);                       // waits 15ms for the servo to reach the position 
  } 
  for(pos = 180; pos>=0; pos-=1)     // goes from 180 degrees to 0 degrees 
  {                                
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(1);                       // waits 15ms for the servo to reach the position 
  }
 */ 
     myservo.write(-180);              // tell servo to go to position in variable 'pos' 
     myservo2.write(180);
} 

