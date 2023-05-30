Header header


geometry_msgs/Twist target_vel
geometry_msgs/Twist output_vel
geometry_msgs/Twist odom_vel

bool input_active

uint8 target_vel_zero
uint8 last_pull_to_odom_vel
bool last_eq_target_vel

float64 period
float64 v_inc
float64 w_inc
float64 max_v_inc
float64 max_w_inc

bool max_v_over
float64 max_v_inc2
float64 max_w_inc2
bool max_w_over

float64 add_v
float64 add_w

float64 v_deviation_lower_bound
float64 v_deviation_upper_bound
float64 w_deviation_lower_bound
float64 angular_max_deviation


float64 acc_v
float64 acc_w

float64 theta


