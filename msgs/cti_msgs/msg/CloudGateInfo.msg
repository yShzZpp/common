string id
string name
string ssid
int8 type
bool enabled

string[] tags

float64 wait
float64 exit

geometry_msgs/Pose coordinate 

float32 loraFrequency
int32 loraModeID
int32 loraNetID

int8 GATE_TYPE_TURNSTILE=0	#常规小型自动闸口
int8 GATE_TYPE_DOOR=1		#自动门
int8 GATE_TYPE_CROSSING=2	#特殊人行横道路口闸口

float32 turnstile_width
float32 turnstile_height
float32 door_width

# lora通信的加密key和解密key
string lora_encryption_key
string lora_decryption_key
