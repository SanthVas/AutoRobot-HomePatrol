#include <HCSR04.h>
#include <Servo.h> 
// 0 is Right & 180 is left 

#include <AFMotor.h>

AF_DCMotor motorLEFT(1);
AF_DCMotor motorRIGHT(2);

Servo servoLook;  // create servo object to control a servo
//int pos = 0;  // variable to store the servo position Tri4, Echo 2

HCSR04 hc(4,2);
//  Serial.println( hc.dist() ); 

byte motorSpeed = 120;                            //The maximum motor speed
byte lag = 2;                                     // Factor to account for slightly different Left & right motor speeds

byte turnSpeed = 170;                             //Amount to add to motor speed when turning
int maxDist = 50;                                 //Maximum sensing distance (Objects further than this distance are ignored)
int stopDist = 30;                                //Minimum distance from an object to stop in cm


void setup() {
//  Serial.begin(9600); 
  servoLook.attach(10);   // attaches the servo on pin 10 to the servo object
  motorLEFT.setSpeed(motorSpeed);
  motorRIGHT.setSpeed(motorSpeed+75);
  motorLEFT.run(RELEASE);
  motorRIGHT.run(RELEASE);
  pinMode(A0, OUTPUT);
  digitalWrite(A0, HIGH);                         // For buzzer beep 
  pinMode(A1, OUTPUT);
  analogWrite(A1, 0);                             // Blue LED blink
  pinMode(A2, OUTPUT);
  analogWrite(A2, 0);                             // Red LED blink
  servoLook.write(110);                           // Servo Test
  delay(750);
  servoLook.write(70);                            // Servo Test 
  delay(750);
  servoLook.write(90);                            // Set the servo to look straight ahead
  delay(750);

}

void loop() {
  int count=2; 
  int dL, dM, dR;
  int distance = hc.dist();                       //Check that there are no objects ahead
//  Serial.println("Distance:");
//  Serial.println(distance);
//  if(distance >= stopDist)                      //If there are no objects within the stopping distance, move forward
//  {
//    moveForward();
//  }
  moveForward (count);
  while(distance >= stopDist)                     //Keep checking the object distance until it is within the minimum stopping distance
  {
    servoLook.write(140);
    digitalWrite(A0, LOW);                        // Buzzer Beep
    analogWrite(A1, 200);                         // Blue LED blink
    delay(50); 
    digitalWrite(A0, HIGH);
    analogWrite(A1, 0);
    delay(50);
    digitalWrite(A0, LOW);
    analogWrite(A1, 200);
    delay(50); 
    digitalWrite(A0, HIGH);
    analogWrite(A1, 0);
    dL = hc.dist();                               // Clearance range on left side
    servoLook.write(90); 
//    digitalWrite(A0, LOW);
    delay(150);
//    digitalWrite(A0, HIGH);
    dM = hc.dist();                               // Clearance range straight ahead
    servoLook.write(40);
    analogWrite(A2, 200);                         // Red LED blink
    delay(50); 
    analogWrite(A2, 0);
    delay(50);
    analogWrite(A2, 200);
    delay(50); 
    analogWrite(A2, 0);

    
//    digitalWrite(A0, LOW);
//    delay(150);
//    digitalWrite(A0, HIGH); 
    dR = hc.dist();                               // Clearance range on right side
    distance = min(dL, min( dM, dR));                   //Check which side has lowest clearance
//    delay(250);
    
//    Serial.println("DistanceIn:");
//    Serial.println(distance);
//    Serial.println("Count:");
//    Serial.println(count);
//  count++; 
   }
  stopMove();                                     //Stop the motors
  digitalWrite(A0, LOW);
  delay(500);
  digitalWrite(A0, HIGH); 
  delay(500);
  digitalWrite(A0, LOW);
  delay(500);
  digitalWrite(A0, HIGH); 
  unsigned long StartTime = millis();
  int turnDir = checkDirection();                 //Check the left and right object distances and get the turning instruction
  Serial.println("turnDir");
  Serial.print(turnDir);
  switch (turnDir)                                //Turn left, turn around or turn right depending on the instruction
  {
    case 0:                                       //Turn left
      turnLeft (1000);
//      digitalWrite(A0, HIGH);
//      delay(10);
//      digitalWrite(A0, LOW); 
      break;
    case 1:                                       //Turn around
      Uturn (1000);
//      digitalWrite(A0, HIGH);
//      delay(10);
//      digitalWrite(A0, LOW); 
      break;
    case 2:                                       //Turn right
      turnRight (1000);
//      digitalWrite(A0, HIGH);
//      delay(10);
//      digitalWrite(A0, LOW); 
      break;
  }
}

//void accelerate()                                 //Function to accelerate the motors from 0 to full speed
//{
//  for (int i=0; i<motorSpeed; i++)                //Loop from 0 to full speed
//  {
//    motorLEFT.setSpeed(i);                        //Set the motors to the current loop speed
//    motorRIGHT.setSpeed(i);
//    delay(10);
//  }
//}
//
//void decelerate()                                 //Function to decelerate the motors from full speed to zero
//{
//  for (int i=motorSpeed; i!=0; i--)               //Loop from full speed to 0
//  {
//    motorLEFT.setSpeed(i);                        //Set the motors to the current loop speed
//    motorRIGHT.setSpeed(i);
//    
//    delay(10);
//  }
//}

void moveForward(int count)                                //Set all motors to run forward
{
  if (count==1){
    for (int i=0; i<255; i++) {
      motorLEFT.setSpeed(i);
      motorRIGHT.setSpeed(i);
      motorLEFT.run(FORWARD);
      motorRIGHT.run(FORWARD);   
      delay(10);
    }
  }
  motorLEFT.setSpeed(motorSpeed);                           //Set the motors to the motor speed
  motorRIGHT.setSpeed(motorSpeed+lag);
  motorLEFT.run(FORWARD);
  motorRIGHT.run(FORWARD);
  
}

void stopMove()                                   //Set all motors to stop
{
  motorLEFT.run(RELEASE);
  motorRIGHT.run(RELEASE);

}

void turnLeft(int duration)                                 //Set motors to turn left for the specified duration then stop
{
  motorLEFT.setSpeed(turnSpeed-lag);                 //Set the motors to the motor speed
  motorRIGHT.setSpeed(turnSpeed);
  motorLEFT.run(BACKWARD);
  motorRIGHT.run(BACKWARD);
  delay (500);
  motorLEFT.run(RELEASE);
  motorRIGHT.run(FORWARD);
  delay(duration);
  motorLEFT.setSpeed(motorSpeed);                           //Set the motors to the motor speed
  motorRIGHT.setSpeed(motorSpeed+lag);
  motorLEFT.run(RELEASE);
  motorRIGHT.run(RELEASE);
  delay (500);
 }

void turnRight(int duration)                                //Set motors to turn right for the specified duration then stop
{
  motorLEFT.setSpeed(turnSpeed-lag);                 //Set the motors to the motor speed
  motorRIGHT.setSpeed(turnSpeed);
  motorLEFT.run(BACKWARD);
  motorRIGHT.run(BACKWARD);
  delay (500);
  motorLEFT.run(FORWARD);
  motorRIGHT.run(RELEASE);
  delay(duration);
  motorLEFT.setSpeed(motorSpeed);                           //Set the motors to the motor speed
  motorRIGHT.setSpeed(motorSpeed+lag);
  motorLEFT.run(RELEASE);
  motorRIGHT.run(RELEASE);
  delay (500);
}

void Uturn(int duration)                                //Set motors to turn right for the specified duration then stop
{
  motorLEFT.setSpeed(turnSpeed);                 //Set the motors to the motor speed
  motorRIGHT.setSpeed(turnSpeed);
  motorLEFT.run(BACKWARD);
  motorRIGHT.run(BACKWARD);
  delay (1000);
  motorLEFT.run(FORWARD);
  motorRIGHT.run(BACKWARD);
  delay(duration);
  motorLEFT.setSpeed(motorSpeed);                           //Set the motors to the motor speed
  motorRIGHT.setSpeed(motorSpeed+lag);
  motorLEFT.run(RELEASE);
  motorRIGHT.run(RELEASE);
  delay (500);
  
}


int checkDirection()                                            //Check the left and right directions and decide which way to turn
{
  int distances [2] = {0,0};                                    //Left and right distances
  int turnDir = 1;                                              //Direction to turn, 0 left, 1 reverse, 2 right
  servoLook.write(180);                                         //Turn servo to look left
  delay(500);
  distances [0] = hc.dist();                                //Get the left object distance
  servoLook.write(0);                                           //Turn servo to look right
  delay(500);
  distances [1] = hc.dist();                                //Get the right object distance
  if (distances[0]>=maxDist && distances[1]>=maxDist)                   //If both directions are clear, turn left
    turnDir = 0;
  else if (distances[0]<=stopDist && distances[1]<=stopDist)    //If both directions are blocked, turn around
    turnDir = 1;
  else if (distances[0]>=distances[1])                          //If left has more space, turn left
    turnDir = 0;
  else if (distances[0]<distances[1])                           //If right has more space, turn right
    turnDir = 2;
    servoLook.write(90);
  return turnDir;
  
}
