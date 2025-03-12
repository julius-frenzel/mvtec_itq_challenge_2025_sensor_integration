#include <Arduino.h>  // Include for Serial and digitalWrite
#include "serial_communication.h"

// Buffer for receiving serial port messages
char in_buffer[BUFFER_SIZE];
int dest_index = 0;

// Related to drive motors
Motor motor_right = {3, 4, 2};
Motor motor_left = {5, 6, 7};
int stearing = 0;
int motor_power = 0;

// Watchdog variables
int watchdog_last_value = 0;
int watchdog_current_value = 0;
bool controlling_application_alive = false;
unsigned long last_wd_change = 0;

// Declare servos from main file
//extern Servo servo_1;
//extern Servo servo_2;

/////////////////////////////////////////////////////////////////////
// Helper functions
/////////////////////////////////////////////////////////////////////

void confirm_command() {
    Serial.write(&in_buffer[1], 6);
    Serial.write(" OK\n");
}

void confirm_error() {
    Serial.write(&in_buffer[0], 10);
    Serial.write(" NOK\n");
}

void set_motor_power(Motor* motor, int val) {
    if (val == 0) {
        digitalWrite(motor->dir_pin_1, LOW);
        digitalWrite(motor->dir_pin_2, LOW);
        analogWrite(motor->pwm_pin, 0);
    } else if (val > 0) {
        if (!controlling_application_alive) return;
        digitalWrite(motor->dir_pin_1, HIGH);
        digitalWrite(motor->dir_pin_2, LOW);
        analogWrite(motor->pwm_pin, (abs(val) + 100 > 255) ? 255 : abs(val) + 100);
    } else {
        if (!controlling_application_alive) return;
        digitalWrite(motor->dir_pin_1, LOW);
        digitalWrite(motor->dir_pin_2, HIGH);
        analogWrite(motor->pwm_pin, (abs(val) + 100 > 255) ? 255 : abs(val) + 100);
    }
}

void analyze_command() {
    int val = 0;
    if (strncmp(&in_buffer[1], "MR=", 3) == 0) {
        val = atoi(&in_buffer[4]) - 155;
        set_motor_power(&motor_right, val);
        confirm_command();
    } else if (strncmp(&in_buffer[1], "ML=", 3) == 0) {
        val = atoi(&in_buffer[4]) - 155;
        set_motor_power(&motor_left, val);
        confirm_command();
//    } else if (strncmp(&in_buffer[1], "S1=", 3) == 0) {
//        val = atoi(&in_buffer[4]);
//        servo_1.write(val);
//        confirm_command();
//    } else if (strncmp(&in_buffer[1], "S2=", 3) == 0) {
//        val = atoi(&in_buffer[4]);
//        servo_2.write(val);
//        confirm_command();
    } else if (strncmp(&in_buffer[1], "WD=", 3) == 0) {
        watchdog_current_value = atoi(&in_buffer[4]);
    } else {
        confirm_error();
    }
}

void serial_communication_task() {
    if (Serial1.available() > 0) {
        int new_byte = Serial1.read();
        if (new_byte == '!') {
            in_buffer[dest_index] = new_byte;
            if (in_buffer[0] == ':') {
                analyze_command();
            } else {
                confirm_error();
            }
            dest_index = 0;
        } else {
            if (dest_index < BUFFER_SIZE) {
                in_buffer[dest_index] = new_byte;
                dest_index++;
            }
        }
    }

    if (watchdog_current_value != watchdog_last_value) {
        watchdog_last_value = watchdog_current_value;
        controlling_application_alive = true;
        last_wd_change = millis();
    } else {
        if (millis() - last_wd_change > 500) {
            controlling_application_alive = false;
            set_motor_power(&motor_right, 0);
            set_motor_power(&motor_left, 0);
        }
    }
}
