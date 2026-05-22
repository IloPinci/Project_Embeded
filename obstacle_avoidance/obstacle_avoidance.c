// GLOBAL VARIABLES NEEDED

// UPDATE THE STRUCT INCLUDING YAW
float acc_values = Sensor_DataStruct();  // Take this from the spi.h file
float obstacle_yaw;
bool read_once = false;
bool rotate_clockwise = false;
bool rotate_counterclockwise = false;
int two_sec_counter = 0;
int rep = 0;

/*
case OBSTACLE_AVOIDANCE: 
    obstacle_avoidance();
*/

void obstacle_avoidance(){
    if(!read_once){
        obstacle_yaw = acc_values.yaw;  // Read SPI value
        read_once = true;
        buggy_control(0, -100);         // Rotate clockwise about 90 degrees
    }

    if((obstacle_yaw - acc_values.yaw >= 90) && !rotate_clockwise){
        buggy_control(1, 0);            // Move the buggy with a low speed after rotating it about 90 degrees clockwise
        rotate_clockwise = true;
    }

    if(++two_sec_counter >= 1000){
        buggy_control(0, -100);         // Rotate counterclockwise about 90 degrees
    }

    if((obstacle_yaw - acc_values.yaw <= -90) && !rotate_counterclockwise){
        bool read_once = false,
        bool rotate_clockwise = false;
        bool rotate_counterclockwise = false;

        if(robot_state == OBSTACLE_AVOIDANCE){ rep++; } // Increase obstacle avoidance attempts
        if(rep >= 3){ robot_state = HALTED; }           // Goes into HALTED state if more than 3 attempts have been performed
    }

}