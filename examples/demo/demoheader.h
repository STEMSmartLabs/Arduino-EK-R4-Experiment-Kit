// ───── PIN DEFINITIONS ─────
#define SERVO_PIN       9
#define FAN_PIN        10
#define US_TRIG_PIN     4
#define US_ECHO_PIN     7
#define DHT_PIN         8
#define NEO_PIN         5
#define LDR_PIN        A0
#define IR_PIN          6
#define POT_PIN        A1
#define BUZZER_PIN     11
#define LED_PIN         3
#define SWITCH_PIN     A3
#define EXT1_PIN        2
#define EXT2_PIN        A2
#define EXT3_RX_PIN     0  // Serial1 RX
#define EXT3_TX_PIN     1  // Serial1 TX
#define BUTTON_PIN     12  

// ───── IRREMOTE v3+ CODES ─────
static const uint32_t IR_KEY_0     = 0xAD52FF00UL;
static const uint32_t IR_KEY_1     = 0xE916FF00UL;
static const uint32_t IR_KEY_2     = 0xE619FF00UL;
static const uint32_t IR_KEY_3     = 0xF20DFF00UL;
static const uint32_t IR_KEY_4     = 0xF30CFF00UL;
static const uint32_t IR_KEY_5     = 0xE718FF00UL;
static const uint32_t IR_KEY_6     = 0xA15EFF00UL;
static const uint32_t IR_KEY_7     = 0xF708FF00UL;
static const uint32_t IR_KEY_8     = 0xE31CFF00UL;
static const uint32_t IR_KEY_9     = 0xA55AFF00UL;
static const uint32_t IR_KEY_STAR  = 0xBD42FF00UL;
static const uint32_t IR_KEY_HASH  = 0xB54AFF00UL;
static const uint32_t IR_KEY_OK    = 0xBF40FF00UL;
static const uint32_t IR_KEY_UP    = 0xB946FF00UL;
static const uint32_t IR_KEY_DOWN  = 0xEA15FF00UL;
static const uint32_t IR_KEY_LEFT  = 0xBB44FF00UL;
static const uint32_t IR_KEY_RIGHT = 0xBC43FF00UL;

// ───── OLED SETUP ─────
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT  64
#define OLED_ADDR     0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ───── SENSOR INSTANCES ─────
DHT dht(DHT_PIN, DHT11);
Adafruit_NeoPixel strip(8, NEO_PIN, NEO_GRB + NEO_KHZ800);

// Indian-flag colors
#define COLOR_SAFFRON  strip.Color(255, 25,  0)  
#define COLOR_WHITE    strip.Color(255, 255, 255)  
#define COLOR_GREEN    strip.Color( 19, 136,   8)  

// ───── MENU CONFIG ─────
const char* menuItems[] = {
  "Servo", "NeoPixel", "Fan", 
  "Temp & Hum", "LED", "LDR","Ultrasonic",
  "Buzzer", "Button","Potentiometer","Switch", 
  "Ext 1", "Ext 2", "Ext 3", "Ext 4"
};
const uint8_t MENU_LEN = sizeof(menuItems)/sizeof(menuItems[0]);
int currentIndex = 0;

// ───── DIGIT BUFFER ─────
int digitBuffer = 0;
unsigned long lastDigitTime = 0;
const unsigned long DIGIT_TIMEOUT = 2000;  // ms

RTC_DS1307 rtc;


