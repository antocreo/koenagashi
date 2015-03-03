# koenagashi
http://koenagashi.tumblr.com/

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
