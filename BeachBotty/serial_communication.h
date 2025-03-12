#ifndef SERIAL_COMMUNICATION_H
#define SERIAL_COMMUNICATION_H

#include <Arduino.h>
//#include <Servo.h>  // Include Servo library

// Buffer for receiving serial port messages
#define BUFFER_SIZE 10
extern char in_buffer[BUFFER_SIZE];
extern int dest_index;

// Motor structure and instances
struct Motor {
    int dir_pin_1;
    int dir_pin_2;
    int pwm_pin;
};

extern Motor motor_right;
extern Motor motor_left;

extern int stearing;
extern int motor_power;

// Watchdog variables
extern int watchdog_last_value;
extern int watchdog_current_value;
extern bool controlling_application_alive;
extern unsigned long last_wd_change;

// Declare servo objects globally
//extern Servo servo_1;
//extern Servo servo_2;

// Function prototypes
void confirm_command();
void confirm_error();
void set_motor_power(Motor* motor, int val);
void analyze_command();
void serial_communication_task();

#endif
