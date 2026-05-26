/*
* Correct implementation, let's see it together tomorrow
* Notes:
* - I think that it's worth to declare a new struct for the sub-state machine
* - The state machine function should be called at a higher frequency in my opinion (500 Hz)
*/


void finite_state_machine(void* param) {
    shared_data *sd = (shared_data *) param;

    // Sub-state variables: static so they persist across calls
    static int  obs_avoid_state  = INIT;
    static bool read_once        = false;
    static int  two_sec_counter  = 0;
    static int  rep              = 0;
    static int  blink_counter    = 0;
    static float obstacle_yaw    = 0.0f;

    switch (sd->current_car_state) {

        // ----- HALT -----
        case HALT:
            LATBbits.LATB8 = !LATBbits.LATB8;  // left side blink
            LATFbits.LATF1 = !LATFbits.LATF1;  // right side blink
            LATGbits.LATG1 = 0;                 // low intensity off
            pwm_stop_all();
            // Reset avoidance state for next entry
            obs_avoid_state = INIT;
            read_once       = false;
            two_sec_counter = 0;
            rep             = 0;
            blink_counter   = 0;
            break;

        // ----- MOVE -----
        case MOVE:
            LATBbits.LATB8 = 0;                 // left off
            LATFbits.LATF1 = 0;                 // right off
            LATGbits.LATG1 = 1;                 // low intensity on
            pwm_control(sd->pwm.speed, sd->pwm.yaw_rate);

            // Transition to AVOID if IR detects obstacle
            if (sd->ir_distance < ir_threshold) {
                sd->current_car_state = AVOID;
                // Reset avoidance sub-state on entry
                obs_avoid_state = INIT;
                read_once       = false;
                two_sec_counter = 0;
                rep             = 0;
                blink_counter   = 0;
            }
            break;

        // ----- AVOID -----
        case AVOID:
            LATBbits.LATB8 = 0;                 // left side off
            LATGbits.LATG1 = 1;                 // low intensity on

            // Right side blinks at 1 Hz.
            // FSM runs at 10 Hz -> toggle every 5 calls = 500 ms period = 1 Hz
            if (++blink_counter >= 5) {
                LATFbits.LATF1 = !LATFbits.LATF1;
                blink_counter = 0;
            }

            // --- Sub-state: INIT -> ROT_CLOCKWISE ---
            if (obs_avoid_state == INIT) {
                obstacle_yaw    = sd->accel_data.yaw;   // snapshot heading
                read_once       = true;
                obs_avoid_state = ROT_CLOCKWISE;
                // Positive yaw_rate = anticlockwise, so clockwise = negative
                pwm_control(0, -50);
            }

            // --- Sub-state: ROT_CLOCKWISE ---
            // Wait until ~90 degrees have been swept
            if (obs_avoid_state == ROT_CLOCKWISE) {
                if (fabsf(angle_diff(sd->accel_data.yaw, obstacle_yaw)) >= 90.0f) {
                    obs_avoid_state = MOVE_FORWARD;
                    two_sec_counter = 0;
                    pwm_control(30, 0);             // move forward at low speed
                }
            }

            // --- Sub-state: MOVE_FORWARD ---
            // FSM at 10 Hz -> 2 s = 20 ticks
            if (obs_avoid_state == MOVE_FORWARD) {
                if (++two_sec_counter >= 20) {
                    obs_avoid_state = ROT_COUNTERCLOCKWISE;
                    obstacle_yaw    = sd->accel_data.yaw;   // new heading snapshot
                    pwm_control(0, 50);             // rotate anticlockwise
                }
            }

            // --- Sub-state: ROT_COUNTERCLOCKWISE ---
            // Return to the previous heading (~90 deg back)
            if (obs_avoid_state == ROT_COUNTERCLOCKWISE) {
                if (fabsf(angle_diff(sd->accel_data.yaw, obstacle_yaw)) >= 90.0f) {
                    pwm_stop_all();
                    obs_avoid_state = INIT;
                    read_once       = false;

                    if (sd->ir_distance < ir_threshold) {
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

        default:
            pwm_stop_all();
            LATBbits.LATB8 = 0;
            LATFbits.LATF1 = 0;
            LATGbits.LATG1 = 0;
            break;
    }
}