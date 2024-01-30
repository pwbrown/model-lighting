
/***************** GPIO PINS ******************/

#define LEFT_HEADLIGHT_PIN 16
#define RIGHT_HEADLIGHT_PIN 13
#define FOG_LIGHTS_PIN 18
#define RUNNING_LIGHTS_PIN 17
#define INTERIOR_LIGHTS_PIN 19
#define LEFT_INNER_TAILLIGHT_PIN 23
#define LEFT_MIDDLE_TAILLIGHT_PIN 22
#define LEFT_OUTER_TAILLIGHT_PIN 21
#define RIGHT_INNER_TAILLIGHT_PIN 33
#define RIGHT_MIDDLE_TAILLIGHT_PIN 25
#define RIGHT_OUTER_TAILLIGHT_PIN 26
#define REVERSE_LIGHTS_PIN 32

/************** LIGHTING BEHAVIOR ************/

#define RUNNING_BRIGHTNESS 25    // How bright should lights be in running mode
#define LOW_BEAM_BRIGHTNESS 50   // How bright are the low beams when on
#define HIGH_BEAM_BRIGHTNESS 100 // How bright are the high beams when on
#define BLINKING_INTERVAL 500    // What is the blinking interval in ms
#define SEQUENTIAL_INTERVAL 100  // What is the sequential effect delay in ms

/***************** MQTT TOPICS ****************/

#define BASE_TOPIC "/lego/mustang/" // The base topic path for all other topics

#define PUB_STATE_TOPIC BASE_TOPIC "state" // For reporting current state
#define PUB_AVAILABLE_TOPIC BASE_TOPIC "available" // For reporting availability
#define SUB_LIGHTING_TOPIC BASE_TOPIC "lighting"   // Update the lighting mode
#define SUB_HIGH_BEAM_TOPIC BASE_TOPIC "high_beam" // Update the high beams
#define SUB_BRAKING_TOPIC BASE_TOPIC "braking"     // Update the braking state
#define SUB_TURNING_TOPIC BASE_TOPIC "turning"     // Update the turning mode
#define SUB_REVERSE_TOPIC BASE_TOPIC "reverse"     // Update the reverse lights
#define SUB_FOG_TOPIC BASE_TOPIC "fog"             // Update the fog lights
#define SUB_INTERIOR_TOPIC BASE_TOPIC "interior"   // Update the interior lights
#define SUB_HAZARD_TOPIC BASE_TOPIC "hazard"       // Update the hazard lights
#define SUB_ALL_TOPIC BASE_TOPIC "all"             // All lights on/off