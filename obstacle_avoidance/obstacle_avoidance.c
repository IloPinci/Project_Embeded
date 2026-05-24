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

// TODO: UPDATE THE STRUCT IN THE spi.h FILE INCLUDING YAW
typedef struct{
    float axis_x;
    float axis_y;
    float axis_z;

    float roll;         //accelerometer: computed roll angle in degrees
    float pitch;        //accelerometer: computed pitch angle in degrees
    float yaw;          //accelerometer: computed yaw angle in degrees
} Sensor_DataStruct;

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
        obstacle_yaw = acc_values.yaw;  // Read SPI value once when one of the rotation states is triggered
        read_once = true;
        if(obs_avoid_state == INIT){
            obs_avoid_state = ROT_CLOCKWISE;
            buggy_control(0, -100);         // Rotate clockwise about 90 degrees
        } else {
            obs_avoid_state = ROT_COUNTERCLOCKWISE;
            buggy_control(0, 100);          // Rotate counterclockwise about 90 degrees
        }
    }

    if((fabsf(angle_diff(acc_values.yaw, obstacle_yaw)) >= 90.0f) && obs_avoid_state == ROT_CLOCKWISE){
        buggy_control(1, 0);            // Move the buggy with a low speed after rotating it about 90 degrees clockwise
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

    if((fabsf(angle_diff(acc_values.yaw, obstacle_yaw)) >= 90.0f) && obs_avoid_state == ROT_COUNTERCLOCKWISE){
        read_once = false;
        obs_avoid_state = INIT;

        // TODO: this part should be combined with the IR reading, the code below is probably wrong
        if(robot_state == OBSTACLE_AVOIDANCE){ 
            rep++;      // Increase obstacle avoidance attempts
        } else {
            rep = 0;
            robot_state = MOVING;  // If no obstacle is detected, back to moving state
        }
        if(rep >= 3){ robot_state = HALTED; }   // Goes into HALTED state if more than 3 attempts have been performed
    }

    // LEDs
    LATBbits.LATB8 = 0;     // Left side lights off
    LATGbits.LATG1 = 1;     // Low intensity lights on
    if (++timer_count >= 250) {
            LATFbits.LATF1 = !LATFbits.LATF1;   // Right side lights blinking at 1 Hz
            timer_count = 0;
        }

}