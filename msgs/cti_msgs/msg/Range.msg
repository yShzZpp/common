std_msgs/Header header

uint8 radiation_type
#uint8 ULTRASOUND=0
#uint8 INFRARED=1
float32 min_range
float32 max_range
float32 field_of_view   # the size of the arc that the distance reading is
                        # valid for [rad]  the object causing the range reading may have
                        #been anywhere within -field_of_view/2 and
                        #field_of_view/2 at the measured range. 
                        #0 angle corresponds to the x-axis of the sensor.

float32 range
