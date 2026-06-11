#ifndef DATA_TYPES_H
#define DATA_TYPES_H

#include "spi.h"

#define IR_THRESHOLD 30     // 30cm threshhold for obstacle detection
#define CTRL_DT 0.002f      // 500 Hz control loop -> 2 ms per tick

// Structs
typedef struct {
    int speed;
    int yawRate;
} pwm_variables;            // Motor control commands

typedef enum {
    HALT = 0,
    MOVE = 1,
    AVOID = 2
} car_state_enum;                    // Car states

typedef enum {
    INIT = 0,
    ROT_CLOCKWISE = 1,
    MOVE_FORWARD = 2,
    ROT_COUNTERCLOCKWISE = 3
} avoid_state;              // Obstacle avoidance states

typedef struct {
    avoid_state state;
    int two_sec_counter;        // Forward-phase tick counter
    int rep;                    // Consecutive avoidance attempts
    float swept;                // Integrated turn angle (deg)
} obs_avoid;                    // Obstacle avoidance FSM

typedef struct {
    car_state_enum state;
    obs_avoid avoid;
} car_state;                // Overall car and obstacle avoidance states

// Task-specific structs
typedef struct {
    float *distance;
    car_state *fsm;
} distance_sensing;

typedef struct {
    pwm_variables *pwm;
    float *distance;
    car_state *fsm;
} pwm_ctrl;

typedef struct {
    float *distance;
    AccelData *accel;
    float *battery;
} uart_send;

#endif
