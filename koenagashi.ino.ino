/*
 KOENAGASHI
 by Anto+Creo
 Antonio Daniele
 MFA Computational Arts - 2015
 http://koenagashi.tumblr.com/
 
 -----------------------------------------
 
 * Requirements
 The requirements for this task is using at least two INPUTS in any combination: two digital, two analogue, or one of each and at least one OUTPUT such as: an actuator of any type OR an on-screen display.
 
 -----------------------------------------
 
 * Analysis
 To make this I will need the following:
 Hardware list 
 1 Arduino 1 board
 1 Raspberry Pi
 2 servo motors
 1 touch sensor
 1 push button
 1 voice sensor
 
 15 wires
 1 USB power supply 5v
 1 USB cable (to connect Arduino and RPi)
 1 webcam
 
 Software
 Arduino IDE
 Openframeworks (please see documentation online for this section - http://koenagashi.tumblr.com/)
 
 Housing
 Please see the documentation online - http://koenagashi.tumblr.com/
 
 -----------------------------------------
 
 *Interactivity Design/Scenario
 The system translates the voice into a paint on water using a traditioanl japanese technique called Suminagashi (litterally floating ink).
 The process is the following:
 The user should swipe his finger on the touch sensor to activate the mic and the whole system.
 The user should speak into the microphone.
 THe voice sensor detects the variation of sound according to a certain threshold.
 The analogic signal is then translated into a binary sequence that is stored into a list (dynamic array, similar to vector but adapted for arduino - see imported library )
 If the mic doesn't receive new inputs for more than 5 seconds,
 I iterate through the binary sequence and I move the servo motors according to the 1s and 0s.
 If the number is one, the left servo is activated and the index is deleted, if the number is 0 the right servo is activated and the index is deleted.
 This happens until the list is empty.
 If the list is empty the system is ready to work again.
 
 THere is also a function to send a serial communication to Raspberry Pi. 
 This will tell to Openframeworks to shoot a picture and to post it on my Twitter profile.
 
 
 
 -----------------------------------------
 
 *Notes and Testing
 The system works nicely and there are no bugs.
 It has been pretty hard to match everything together.
 Some problem I encountered during the coding:
 - Dynamic arrays or Vectors in Arduino are not very supported and I had to find my way around it using this library (QueueArray).
 This library however doesn't allow access to the individual indeces but only the first and the last so I had decided to delete the indeces everytime I was checking them. This solution apparently worked out well.
 - The servo library deactivates pin 9 and 10, so, no matter how many times you try to change your code, you will always have problems. I put a giant note on the code.
 - The initial angle for the servo motors has to be set on 15. This will prevent the servos to vibrate when they are not working.
 - The millis() method is an unsigned long. If you try to use it as a Int, like I did, you will end up with a lot of problems and you will waste a lot of time, like I did.
 Your code will behave in a strange way. I put a note for that as well.
 - The microphone's sensitivity might need to be set beforehand.
 
 
 -----------------------------------------
 
 *Bugs
 None
 
 
 
 */


/*----------------------------------------------------------------------------------
 C O D E  - K O E N A G A S H I
 ----------------------------------------------------------------------------------*/

#include <QueueArray.h>
#include <Servo.h> //////MIND!!! THIS LIBRARY DISABLE PIN 9 AND 10!!!!


const int DO = 4; //Pin for Digital Output - DO //MICROPHONE//this is not really used but it's good to have it
const int micSensor = A5; // Pin for Analog Output - AO //MICROPHONE

const int touchSensor = A3; // Pin for Analog Output - A3)//TOUCH
const int buttonPin = 12; // Pin for digital INPUT - DO //BUTTON


int micValue = 0;  //set mic val to 0
int touchValue = 0;  //set touch val to 0
int butValue = 0;  //set button val to 0

int threshold = 100; //Set minimum threshold for voice sensor
int count = 0;
boolean shoot;
boolean arrayReady;
boolean hearing;
int timeSilence = 3000;
unsigned long silence;    //this has to be unsigned long!!!
int bitVal; //value of the bit

///////////servo variables////////////

Servo myservo;  // create servo objects to control a servo 
Servo myservo2;               
int pos = 0;    // variable to store the servo position 
int pos2 = 0;    // variable to store the servo position 
int angle = 60;    //angle of servo
int angle2 = 60;   //angle of servo

boolean servo1,servo2;
boolean servoMoving;


///////////end servo global///////////


QueueArray <int> queue; //define the queueArray
QueueArray <int> seq; //define the queueArray



/*SETUP------------------------------------------------------------------*/


void setup() {
  Serial.begin(9600);  
  silence = 0;
  touchValue=0;
  micValue=0;
  emptyVector();    //clean up the vector
  arrayReady=false;  //array is not ready
  servoMoving=false; //servo is not moving
  pinMode(buttonPin, INPUT);    // declare pushbutton as input

  queue.setPrinter (Serial);  //setting the printer for the queue

  ///////////servo ////////////

  myservo.attach(9);  // attaches the servo on pin 9 to the servo object 
  myservo2.attach(10);  // attaches the servo on pin 9 to the servo object 


  //the 2 servos are not moving at the moment
  servo1=false;
  servo2=false;
  //////////////////////////////

}


/*LOOP------------------------------------------------------------------*/


void loop() {

  ////BUTTON FOR THE SHOT////
  butValue = digitalRead(buttonPin);
  if (butValue == HIGH) {
    
    // check if the input is HIGH (button released)
    shotPicture();
  }

  //// TOUCH SENSOR////
  // read the touch  value:
  touchValue = analogRead(touchSensor);
  //    Serial.println(touchValue); 
  micValue = analogRead(micSensor);  //Read the mic value
  //    Serial.println(micValue); 

  //when the touch sensor is on
  if(touchValue>100){
    Serial.println("SAY YOUR NAME");
    emptyVector();    //clean up the vector
    hearing=true;    //the mic is now hearing
    silence=0;
  }


  //// MIC SENSOR ////
  //check if the signal is bigger than threshold
  if  ( 
  micValue>threshold
    && hearing == true //mic hearing?
  && arrayReady == false //array ready?
  &&  (servo1 == false || servo2 == false) //servo2 moving?
  ) {
    silence = millis();    //get the millis

      bitVal = bitRead(micValue,0);     //transform analog value into binary
    Serial.println(bitVal);
    queue.enqueue((int)bitVal);    //push the value into the vector
    seq.enqueue((int)bitVal);      //I duplicate this array to use it for serial communication

    Serial.print("size now is"); 
    Serial.println(queue.count());  //print the size of the array
    silence = millis();    //get the millis
    //    Serial.println(millis()); 

  }  

  // now the array is ready
  //let's check if there is enough silence to activate the motors.
  if (millis() - silence >= timeSilence
    && micValue<threshold
    //    && hearing  == false //mic is not hearing
  ) {
    arrayReady = true; // the array is ready
    Serial.println("ready!");
    startServo(); //start servo routine
  }




}

/*FUNCTIONS------------------------------------------------------------------*/


void emptyVector(){

  //empty the vector
  while (!queue.isEmpty ()){
    queue.dequeue ();
  }

  arrayReady = false; // the array is empty again

  Serial.println("empty!");
  Serial.println(queue.count()); //print size of array
  Serial.println("--------");
}


/*------------------------------------------------------------------*/


void startServo(){

  //until the array is empty
  while (!queue.isEmpty ()){
    hearing=false;    //I deactivate the mic
    micValue=0;      //just to be sure I reset the mic Val to 0

      //check if servo2 is moving and the index is 0
    if(queue.front() == 0){

      //activate my servo1
      servo1Go();

    }
    else if (queue.front() == 1){
      //activate my servo2
      servo2Go();

    }

    //    //take the element out
    //    queue.dequeue();
  }

  //check if the counter of the servo movement is equal to the array size, the emmpty the array 
  if(queue.count()==0){

    //at the end of the routine
    //    emptyVector();
    arrayReady = false; // the array is empty again
    silence = millis();    //get the millis
    //    silence = 0;
  }

}

/*------------------------------------------------------------------*/

void servo1Go(){


  //write serial
  //  Serial.write(18); // send a byte with the value 48

  //servo 1 is moving servo 2 not

  servo1 = true;
  servo2 = false;

  //move servo 1
  if (servo1==true && servo2==false){
    for(pos = 15; pos <= angle; pos += 1) // goes from 0 degrees to 180 degrees 
    {                                  // in steps of 1 degree 
      myservo.write(pos);      // tell servo to go to position angle in variable 'pos' 
      Serial.println("s1 start");

    }
    for(pos = angle; pos>=15; pos-=1)     // goes from angle degrees to 0 degrees 
    {                                
      myservo.write(pos);
      Serial.println("s1 end");

    }

    delay(1000);

    //take the element out
    queue.dequeue();
    Serial.println("SERVO 1");
  }
  //stop servo 1
  servo1 = false;

}

/*------------------------------------------------------------------*/


void servo2Go(){

  //write serial
  //  Serial.write(18); // send a byte with the value 18

  //servo 2 is moving servo 1 not
  servo1 = false;
  servo2 = true;

  //move servo 2

  if (servo2==true && servo1==false){

    for(pos2 = 15; pos2 <= angle2; pos2 += 1) // goes from 0 degrees to 180 degrees 
    {                                  // in steps of 1 degree 
      myservo2.write(pos2);      // tell servo to go to position angle in variable 'pos' 
      Serial.println("s2 start");

    }
    for(pos2 = angle2; pos2>=15; pos2-=1)     // goes from angle degrees to 0 degrees 
    {                                
      myservo2.write(pos2);
      Serial.println("s2 end");
    }

    delay(1000);

    //take the element out
    queue.dequeue();
    Serial.println("SERVO 2");

  }

  //stop servo 2
  servo2 = false;

}

/*------------------------------------------------------------------*/


void shotPicture(){

  Serial.write(36); // send a byte with the value 45
  Serial.println("sending 36");

  //  int bytesSent = Serial.write("Cheese!!"); //send the string “hello” and return the length of the string.
  //  Serial.println(bytesSent);


}



/*------------------------------------------------------------------*/


void sendArray(){


  while (!seq.isEmpty ()){


    if(seq.front() == 1){
      Serial.write(18); // send a byte with the value 18
      Serial.println("sending 1");
      delay(1000);
      seq.dequeue ();

    } 
    else if(seq.front() == 0){

      Serial.write(19); // send a byte with the value 48
      Serial.println("sending 0");
      delay(1000);
      seq.dequeue ();

    }

  }
}



























































