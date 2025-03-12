
#include <Arduino.h>
#include "serial_communication.h"
#include <Servo.h>

void setupTimers();
void setup();
void loop();

// Placeholder functions for actual tasks
void serialCommTask();
void actorControlTask();
void readSensorTask();

// Interrupt Service Routine (ISR) for Timer 2
ISR(TIMER2_COMPA_vect) {
    sensor_actor_task();
}

void setupTimers() {
    cli(); // Disable global interrupts

    // Timer2 - Sensor & Actor Control Task (10ms)
    TCCR2A = 0;
    TCCR2B = 0;
    TCNT2 = 0;
    
    OCR2A = 249;  // 10ms interval at 16MHz with prescaler 64
    TCCR2A |= (1 << WGM21);  // CTC Mode
    TCCR2B |= (1 << CS22);   // Prescaler 64 (CS22=1, CS21=0, CS20=0)
    TIMSK2 |= (1 << OCIE2A); // Enable Timer Compare Match Interrupt

    sei(); // Enable global interrupts
}


// Arduino setup function is executed once after power on
void setup() {
  // initialize both serial ports:
  Serial.begin(9600);
  Serial1.begin(9600);
  //servo_1.attach(8);
  //servo_2.attach(9);
  pinMode(2, OUTPUT); 
  pinMode(3, OUTPUT); 
  pinMode(4, OUTPUT); 
  pinMode(5, OUTPUT); 
  pinMode(6, OUTPUT); 
  pinMode(7, OUTPUT); 

  for(int i=0;i<BUFFER_SIZE;i++)
  {
    in_buffer[i]='\0';
  }

  setupTimers();
}

void loop() {
    // Execute serial communication task continuously
    serial_communication_task();
}

void sensor_actor_task() {
    // Read data from sensors and compute / apply control outputs
    Serial.println("Actor Control Task Executed");
}



