/*
OBSTACLE AVOIDANCE:
- In this state, the robot should rotate clockwise of about 90 degrees, move forward for two seconds, and
then rotate anti-clockwise back to the previous heading. If now it senses no obstacles, it goes back to the
“Moving state”. Otherwise, the procedure is repeated for a maximum of three times. If an obstacle is still
sensed, the state is changed to “Halted”.

- The right-side lights should blink at 1 Hz. The low intensity lights should be on. The left-light side should
be off.

- If the button RE8 is pressed, the robot should go to the “Halted” state
    To implement this inside the ISR
*/

#include <stdbool.h>
#include <math.h>

// Obstacle avoidance states
#define INIT 0
#define ROT_CLOCKWISE 1
#define MOVE_FORWARD 2
#define ROT_COUNTERCLOCKWISE 3

// Main variables (or global, depending on the other tasks)
Sensor_DataStruct acc_values = {0};  
float obstacle_yaw;

// State vector
int obs_avoid_state = INIT;

// Counters and flags
bool read_once = false;
int two_sec_counter = 0;
int rep = 0;

// Function to handle yaw difference wrap around
float angle_diff(float a, float b) {
    float diff = a - b;
    // Wrap into [-180, +180]
    while (diff > 180.0f)  diff -= 360.0f;
    while (diff < -180.0f) diff += 360.0f;
    return diff;
}

/*
case OBSTACLE_AVOIDANCE: 
    obstacle_avoidance();
*/

// rep = 0 in the main

void obstacle_avoidance(){
    if(!read_once){
        obstacle_yaw = sd->accel_data.yaw;  // Read SPI value once when one of the rotation states is triggered
        read_once = true;
        if(obs_avoid_state == INIT){
            obs_avoid_state = ROT_CLOCKWISE;
            pwm_control(0, -100);     // Rotate clockwise about 90 degrees
        } else {
            obs_avoid_state = ROT_COUNTERCLOCKWISE;
            buggy_control(0, 100);      // Rotate counterclockwise about 90 degrees
        }
    }

    if((fabsf(angle_diff(sd->accel_data.yaw, obstacle_yaw)) >= 90.0f) && obs_avoid_state == ROT_CLOCKWISE){
        pwm_control(1, 0);            // Move the buggy with a low speed after rotating it about 90 degrees clockwise
        obs_avoid_state = MOVE_FORWARD;
        two_sec_counter = 0;
    }

    if(obs_avoid_state == MOVE_FORWARD && read_once == true){
        if(two_sec_counter >= 1000){
            read_once = false;
        } else {
            two_sec_counter++;
        }
    }

    if((fabsf(angle_diff(sd->accel_data.yaw, obstacle_yaw)) >= 90.0f) && obs_avoid_state == ROT_COUNTERCLOCKWISE){
        read_once = false;
        obs_avoid_state = INIT;

        // TODO: this part should be combined with the IR reading, the code below is probably wrong
        if(sd->current_car_state == AVOID){ 
            rep++;      // Increase obstacle avoidance attempts
        } else {
            rep = 0;
            sd->current_car_state = MOVE;  // If no obstacle is detected, back to moving state
        }
        if(rep >= 3){ sd->current_car_state = HALT; }   // Goes into HALTED state if more than 3 attempts have been performed
    }
}