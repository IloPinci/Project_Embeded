/*
* Correct implementation, let's see it together tomorrow
* Notes:
* - I think that it's worth to declare a new struct for the sub-state machine
* - The state machine function should be called at a higher frequency in my opinion (500 Hz)
*/
#define INIT 0
#define ROT_CLOCKWISE 1
#define MOVE_FORWARD 2
#define ROT_COUNTERCLOCKWISE 3

void finite_state_machine(void* param) {
    shared_data *sd = (shared_data *) param;

    // Sub-state variables: static so they persist across calls
    static int  state  = INIT;
    static int  two_sec_counter  = 0;
    static int  rep              = 0;
    static float obstacle_yaw    = 0.0f;


    switch (sd->current_car_state) {

        // ----- AVOID -----
        case AVOID:
            // --- Sub-state: INIT -> ROT_CLOCKWISE ---
            if (state == INIT) {
                obstacle_yaw = sd->accel_data.yaw;   // snapshot heading
                state = ROT_CLOCKWISE;
                // Positive yaw_rate = anticlockwise, so clockwise = negative
                pwm_control(0, -50);
            }

            // --- Sub-state: ROT_CLOCKWISE ---
            // Wait until ~90 degrees have been swept
            if (state == ROT_CLOCKWISE) {
                if (fabsf(angle_diff(sd->accel_data.yaw, obstacle_yaw)) >= 90.0f) {
                    state = MOVE_FORWARD;
                    two_sec_counter = 0;
                    pwm_control(30, 0);             // move forward at low speed
                }
            }

            // --- Sub-state: MOVE_FORWARD ---
            // FSM at 10 Hz -> 2 s = 20 ticks
            if (state == MOVE_FORWARD) {
                if (++two_sec_counter >= 20) {
                    state = ROT_COUNTERCLOCKWISE;
                    obstacle_yaw  = sd->accel_data.yaw;   // new heading snapshot
                    pwm_control(0, 50);             // rotate anticlockwise
                }
            }

            // --- Sub-state: ROT_COUNTERCLOCKWISE ---
            // Return to the previous heading (~90 deg back)
            if (state == ROT_COUNTERCLOCKWISE) {
                if (fabsf(angle_diff(sd->accel_data.yaw, obstacle_yaw)) >= 90.0f) {
                    pwm_stop_all();
                    state = INIT;

                    if (sd->ir_distance <= ir_threshold) {
                        // Obstacle still there: try again or give up
                        rep++;
                        if (rep >= 3) {
                            rep = 0;
                            sd->current_car_state = HALT;
                        }
                        // else: loop back — next call will re-enter INIT
                    } else {
                        // Clear path: go back to MOVE
                        rep = 0;
                        sd->current_car_state = MOVE;
                    }
                }
            }
            break;
}