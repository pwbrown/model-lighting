
/***************** GPIO PINS ******************/

#define GINGERBREAD_PORCH_PIN 26
#define GINGERBREAD_CANDLE_PIN 25
#define GINGERBREAD_FIRST_PIN 33
#define GINGERBREAT_SECOND_PIN 32

/***************** MQTT TOPICS ****************/

#define BASE_TOPIC "/christmas-village/"
#define PUB_AVAILABLE_TOPIC BASE_TOPIC "available"
#define PUB_STATE_TOPIC BASE_TOPIC "state"

// Lego Gingerbread House (#10267)
#define GINGERBREAD_BASE BASE_TOPIC "gingerbread/"
#define SUB_GINGERBREAD_ALL_TOPIC GINGERBREAD_BASE "all"       // All Lights (off or on)
#define SUB_GINGERBREAD_PORCH_TOPIC GINGERBREAD_BASE "porch"   // Porch lanterns
#define SUB_GINGERBREAD_CANDLE_TOPIC GINGERBREAD_BASE "candle" // Candles on first floor
#define SUB_GINGERBREAD_FIRST_TOPIC GINGERBREAD_BASE "first"   // First floor ceiling lights
#define SUB_GINGERBREAD_SECOND_TOPIC GINGERBREAD_BASE "second" // Second floor ceiling lights