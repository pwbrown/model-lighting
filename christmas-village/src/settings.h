
/***************** GPIO PINS ******************/

#define GINGERBREAD_PIN 16
#define HONEYDUKES_PIN 17
#define THREEBROOMSTICKS_PIN 18
#define TOYSTORE_PIN 19
#define MUSICSTORE_PIN 22
#define TROLLEY_PIN 23
#define TREES_PIN 25
#define LAMPS_PIN 26


/***************** MQTT TOPICS ****************/

#define BASE_TOPIC "/christmas-village/"
#define PUB_AVAILABLE_TOPIC BASE_TOPIC "available"
#define PUB_STATE_TOPIC BASE_TOPIC "state"

#define SUB_ALL_TOPIC BASE_TOPIC "all"                           // All Lights (off or on)
#define SUB_GINGERBREAD_TOPIC BASE_TOPIC "gingerbread"           // Gingerbread House
#define SUB_HONEYDUKES_TOPIC BASE_TOPIC "honeydukes"             // Honeydukes
#define SUB_THREEBROOMSTICKS_TOPIC BASE_TOPIC "threebroomsticks" // Three Broomsticks
#define SUB_TOYSTORE_TOPIC BASE_TOPIC "toystore"                 // Toy Store
#define SUB_MUSICSTORE_TOPIC BASE_TOPIC "musicstore"             // Music Store
#define SUB_TROLLEY_TOPIC BASE_TOPIC "trolley"                   // Trolley
#define SUB_TREES_TOPIC BASE_TOPIC "trees"                       // Trees
#define SUB_LAMPS_TOPIC BASE_TOPIC "lamps"                       // Lamps
