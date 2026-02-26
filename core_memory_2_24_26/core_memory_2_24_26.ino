// Arduino Nano-ESP32 Program for core memory demo

/*
If using Nano ESP32 - in the *** IDE Tools menu ***, enter Pin Numbering and choose By GPIO number (legacy).
Then sketch should always use labels to refer to Arduino pins (e.g if you used the 
number 2, replace this with the symbol D2).
This is a more library-compatible scheme and avoids confusion.  
Also see pin table at end of this sketch.
*/

// For uint64_t 
#include <stdint.h> 

// for i2c for oled display and PCF8574 keyboard and SPI for 74HC595s
#include <SPI.h>
// SPI Notes - can use ESP32-S3 SPI2 (HSPI) and SPI3 (VSPI)
// SP0 and SP1 used for internal flash memory
// Arduino defaults for its single "SPI" are
// CIPO or MISO = D12;
// COPI or MOSI = D11;
// SCK = D13;
// SS not listed, but can be any pin(s) and operate manually 
#include <Wire.h>  // for i2c display
// TwoWire i2c1 = TwoWire(0);
// TwoWire i2c2 = TwoWire(1);  // if second i2c needed
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define CUSTOM_SDA D3
#define CUSTOM_SCL D2
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels  was 32!!
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
// even though the display has no reset pin, it must be initialized 
//   as follows for the library to work - 4 is the reset pin
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
char oled_line1_str[32];  // oled can display 18 characters per line inside of rectangle box
char oled_line2_str[32];
char oled_line3_str[32];
char oled_line4_str[32];
char oled_line5_str[32];
char oled_line6_str[32];
char oled_text_str[32];
char oled_title_str[32];
char oled_error_str[32];
long int startCnt;
long int endCnt;
char strEndCnt[8];
long int maxCnt = 1000000;  // 1 million default
char strMaxCnt[8] = "1M";
char revIn[9];
char revOut[9];

// for sense read
int sensePin = D4;
int senseIn;

// for inhibit
int inhibitPin0 = D5;
int inhibitPin1 = D6;
int inhibitEnablePin = D7;
int inhibitOut0 = 0;
int inhibitOut1 = 0;
int inhibitFlag = 0;

// for 74HC595 latches
int oePin = D8;
int rclkPin = D9;
uint8_t buffer[4] = {0x00, 0x00, 0x00, 0x00}; // for SDI 
// at 20MHz, 50ns per bit = 1.6us for 32 bits.  Readback takes another 1.6us.
// the mirror received data (all 0s) is stored in the buffer in-place of the data sent and is not used
uint8_t output[4] = {0x00, 0x00, 0x00, 0x00};  // start at clear 
// transmits msbit to lsbit left to right - [0] first

// for SN754410 (or L293) pulses
int enPin = D10; // active high

// for array
uint8_t selBitx = 0; 
uint8_t selBity = 0; 
char selBitx_str[4] = "0"; 
char selBity_str[4] = "0"; 
int memPos = 0;
uint64_t memContents; // uint64_t  64-bit unsigned
uint64_t memRead = 0; // uint64_t  64-bit unsigned
uint64_t stringWrite0[4] = {0x0123456789abcdef, 0xfedcba9876543210, 0xa5a5a5a5a5a5a5a5, 0x5a5a5a5a5a5a5a5a};
int stringNum0 = 0;
// note - limited to 8 characters
char stringWrite1[5][9] = {" Andrew ", "  Cole  ", " Austin ", "  Kim   ", "  John  "}; // add null too
int stringNum1 = 0;
uint64_t testM = 0x00;
int inhWriteM = 0; // used for those functions that require a 1 or 0 inhibit write
// note random inhibit write function 1-10M use complement of then current bit as the inhibit write 

// for errors
uint64_t comparedResult = 0;
int errorCnt = 0;
int errorReportFlag = 1; // for serial print out of error information (rare) 

// for timing
unsigned long decodeStartus = 0;
unsigned long writeStartus = 0;
unsigned long writeEndus = 0;
unsigned long decodeToEndus = 0;
unsigned long writeToEndus = 0;

// for motion
int motionPin = A0; // active high (D19)
int motionState; // state 0-off, 1-on
int prior_motionState = 3;
int motionReportFlag = 0; // for serial print out motion state 

// for thermistor
int thermistorPin = A1; 
int thermistorRead;// 0-4096 - 12 bit
int thermistorValue; 
int thermistorReportFlag = 0; // for serial print out thermistor values 

// for power relay - used to reduce power when display is not active 
// turns off all power except nano-esp32, oled, motion sensor and serial terminal  
//  relay connected to npn 3904 base with 3k resistor with
//    3904 collector connected to relay coil then to 8.0V with 
//    protection diode IN4148 between collector and 8.0V
int relayPin = A2; // active high (D8)
int relayState; // state 0-off, 1-on
int prior_relayState;
int relayReportFlag = 0; // for serial print out relay state 

// for voltage monitors
int inputVoltagePin = A3; // voltage after diode - approx 8 volts
int inputVoltageRead; // 0-4096 - 12 bit
float inputVoltageValue;
int inputVoltageReportFlag = 0; // for serial print out voltage values 
int VsVoltagePin = A4; // voltage from regulator - approx 5.2 volts
int VsVoltageRead; // 0-4096 - 12 bit
float VsVoltageValue;
int VsVoltageReportFlag = 0; // for serial print out voltage values 

// for leds
// configure leds
// Nano ESP32 onboard RGB led
#define LED_R 46  // Note - Boot0  active low
#define LED_B 45  // Note - not an on-board pinout  active low
#define LED_G 0   // Note - B00t1 active low
// PWM channel assignments
#define CH_R 0
#define CH_G 1
#define CH_B 2
// green power led always on with power
// yellow on board led - also called LED_BUILTIN
//   and is on D13 or GPIO48 - SPI Serial Clock

// led state 0-off, 1-on, 2-blinking(1s)
int blueLedPin = A4; // active low - used for indicating motion detected
int redLedPin = A5;  // active low - used for indicating power relay on
int blueLedState = 0; // blueLed off
int redLedState = 0; // redLed off

// for keypad
#define PCF8574_ADDR 0x20 // i2c
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'0','2','4','6'},
  {'1','3','5','7'},
  {'7','8','9','C'}, // row not used
  {'*','0','#','D'}  // row not used
};
/*char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
*/
byte rowPins[ROWS] = {0, 1, 2, 3}; // P0–P3
byte colPins[COLS] = {4, 5, 6, 7}; // P4–P7
unsigned long lastPressTime = 0;
unsigned long now = 0;
char key = 'x'; // no key to start
int readyFlag = 1; // require key release before next key entry

// for menu
int menuState = 99;
int executeState = 99;
unsigned long int startTimeOut = 0;

// for void loop timers
unsigned long cm; // current time for timer
unsigned long pmTherm = 0; // previous time for timer
unsigned long pmVoltage = 0; // previous time for timer
unsigned long pmMotion = 0; // previous time timer
unsigned long pmLeds = 0;  // prevous time for LED timer
unsigned long pmDisplayActive = 0; // previous time for display active timer
unsigned long pmRelayActive = 0; // previous time for relay active timer
unsigned long lmMotion = 0; // last time motion for display active
unsigned long lmCommand = 0; // last time command for display active
int displayActiveFlag = 1; // for blanking the display to preserve OLED life

void setup()
{
Serial.begin(115200); // for serial monitor
//while (!Serial) {}; // wait for serial port to connect.

// for relay
pinMode(relayPin, OUTPUT);  
relayOn();

// for SN754410 drivers
pinMode(enPin, OUTPUT); // active high
digitalWrite(enPin, LOW); // active high
pinMode(inhibitEnablePin, OUTPUT); // active high
digitalWrite(inhibitEnablePin, LOW); // active high

// for 74HC595 output enable and rclk latch
pinMode(oePin, OUTPUT); // active low with pullup to 3.3V (by voltage divider from 5V) for startup
digitalWrite(oePin, HIGH); // active low
pinMode(rclkPin, OUTPUT); // rising edge latches
digitalWrite(rclkPin, LOW); // rising edge latches

// for sense read
pinMode(sensePin, INPUT_PULLUP);

// for inhibit write
pinMode(inhibitPin0, OUTPUT); // active high
pinMode(inhibitPin1, OUTPUT); // active high
digitalWrite(inhibitPin0, LOW); // active high
digitalWrite(inhibitPin1, LOW); // active high

// start i2c
//i2c1.begin(D3,D2,100000); // SDA pin D3, SCL pin D2, 400k baud  **** for two wire i2c
// default std frequency on eps32 s3 is acutally 100k
Wire.begin(CUSTOM_SDA, CUSTOM_SCL, 100000); // Initializes I2C with custom pins and rate
// Wire.begin(); default pins are SDA A4, SCL A5
delay(50);

// set up oled
// by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
   Serial.println(F("SSD1306 allocation failed"));
  }
//else Serial.println(F("SSD1306 allocation completed"));
display.clearDisplay(); // using this before display.display gets rid of the oled Adafruit logo
display.display(); 

// for motion
pinMode(motionPin, INPUT_PULLDOWN);  // remains low or off when not connected
motionState = digitalRead(motionPin);
prior_motionState = motionState;

// for thermistor and voltage
analogReadResolution(12); // set analog read resolution to 12 bits 0-4096

//for LEDS
// Configure PWM channels
ledcSetup(CH_R, 5000, 8); // channel, freq, resolution
ledcSetup(CH_G, 5000, 8);
ledcSetup(CH_B, 5000, 8);
// Attach pins to channels
ledcAttachPin(LED_R, CH_R);
ledcAttachPin(LED_G, CH_G);
ledcAttachPin(LED_B, CH_B);
pinMode(LED_BUILTIN, OUTPUT);
setColor(0, 127, 0);   // Green
pinMode(blueLedPin, OUTPUT);
pinMode(redLedPin, OUTPUT);

// SPI uses D13 CLK, D12 POCI, D11 COPI - only CLK and COPI used here 
SPI.begin();
delay(50);
// testing 5 instead of 20
SPI.beginTransaction(SPISettings(25000000, MSBFIRST, SPI_MODE0)); //// 20MHz appears max for ESP32  25MHz is max for 74LS595
SPIWriteClear();  // clear
delay(1);
digitalWrite(oePin, LOW);  // enable outputs on 595s

// check input voltage
inputVoltageRead = 0;
int rCnt;
for (rCnt = 0; rCnt < 16; rCnt++){
  inputVoltageRead = inputVoltageRead + analogRead(inputVoltagePin); 
  delay(1);
  } 
inputVoltageRead = inputVoltageRead / rCnt; 
inputVoltageValue = (((float)inputVoltageRead * 3.3 * 11) / 4096) + 0.6;  // input was 1 / 11 and 4096 is full 
// scale with 3.3 volts - + 0.6 is correction factor from volt meter measurement
if (inputVoltageValue > 9){   // normal after diode voltage is is 8.0 volts
  Serial.print("inputVoltageValue "); 
  Serial.print("  ");
  Serial.print(inputVoltageValue);
  Serial.println("  INPUT OVER VOLTAGE - HALTING");
  strcpy(oled_line1_str, "                   ");
  strcpy(oled_line2_str, " ** INPUT OVER     ");
  strcpy(oled_line3_str, "        VOLTAGE ** ");
  strcpy(oled_line4_str, "      HALTING      ");
  strcpy(oled_line5_str, "                   ");
  strcpy(oled_line6_str, "        ");
  sprintf(oled_text_str, "%4.2f", inputVoltageValue);
  strcat(oled_line6_str, oled_text_str);
  strcat(oled_line6_str, "v");
  oledText(oled_line1_str, 8, 3, 1, false);
  oledText(oled_line2_str, 8, 13, 1, false);
  oledText(oled_line3_str, 8, 23, 1, false);
  oledText(oled_line4_str, 8, 33, 1, false);
  oledText(oled_line5_str, 8, 43, 1, false);
  oledText(oled_line6_str, 8, 53, 1, false);
  oledRect(); // also displays display
  while(1){
    setColor(16, 0, 0);   // Red - not too bright
    digitalWrite(redLedPin, LOW); // active low
    delay(250); 
    setColor(0, 0, 0); // Off
    digitalWrite(redLedPin, HIGH); // active low
    delay(250);
    }
  }
if (inputVoltageValue < 7){
  Serial.print("inputVoltageValue "); // for testing
  Serial.print("  ");
  Serial.print(inputVoltageValue);
  Serial.println("  INPUT UNDER VOLTAGE - HALTING");
  strcpy(oled_line1_str, "                   ");
  strcpy(oled_line2_str, " ** INPUT UNDER    ");
  strcpy(oled_line3_str, "        VOLTAGE ** ");
  strcpy(oled_line4_str, "      HALTING      ");
  strcpy(oled_line5_str, "                   ");
  strcpy(oled_line6_str, "        ");
  sprintf(oled_text_str, "%4.2f", inputVoltageValue);
  strcat(oled_line6_str, oled_text_str);
  strcat(oled_line6_str, "v");
  oledText(oled_line1_str, 8, 3, 1, false);
  oledText(oled_line2_str, 8, 13, 1, false);
  oledText(oled_line3_str, 8, 23, 1, false);
  oledText(oled_line4_str, 8, 33, 1, false);
  oledText(oled_line5_str, 8, 43, 1, false);
  oledText(oled_line6_str, 8, 53, 1, false);
  oledRect(); // also displays display
  while(1){
    setColor(16, 0, 0);   // Red - not too bright
    digitalWrite(redLedPin, LOW); // active low
    delay(250); 
    setColor(0, 0, 0); // Off
    digitalWrite(redLedPin, HIGH); // active low
    delay(250);
    }
  }

// for some movement before the splash screen  
// write random numbers 6400
char strMaxCnt[8] = "6400";
char oled_title_str[32] = "    write/read    ";
// function in menu.ino
randomWriteInhRead(0, 6400, 0, strMaxCnt, oled_title_str);  // no inhibit
delay(2000);

// for spalsh screen
menuState = 0;  
updateMenu();

// end setup
}

void loop()
{
if (getKey()){ 
  //Serial.println(key);  
  lmCommand = millis();  // record last command time for display active
  menu(); // execute key
  }

cm = millis(); // store current millis
// Check for motion every 0.5 seconds
if (cm - pmMotion >= 500){ 
  pmMotion = cm;  // Remember the time
  motionState = digitalRead(motionPin);  // active high
  if (motionState == HIGH){
    blueLedState = 1; // led on
    lmMotion = cm;  // Remember the time
    }
  else{
    blueLedState = 0; // led off
    }
  // for testing
  if (motionState != prior_motionState){
    prior_motionState = motionState; 
    if (motionReportFlag == 1){
      if (motionState == HIGH){
        Serial.println("motion detected  ");
        }
      else{
        Serial.println("no motion  ");
        }
      }
    }
  }  

// check if display active every 0.5 second 
// motion turns on display
// key turns on display  
// key turns on relay too if the key press runs executeAction() 
// display and relay are active for 1 minute
if (cm - pmDisplayActive >= 500) { 
  pmDisplayActive = cm;  // Remember the time
  if ( ( ((cm - lmMotion) <= (1 * 60000))  ) //  motion turns on for 1 minute
    || ((cm - lmCommand) <= (1 * 60000)) ){ // key press turns on for 1 minutes
      if (displayActiveFlag == 0){
      displayActiveFlag = 1;
      display.ssd1306_command(SSD1306_DISPLAYON); // To switch display back on
      }
    }  
  else { // relay and display off if last command > 1 minute
    relayOff(); 
    displayActiveFlag = 0;
    display.ssd1306_command(SSD1306_DISPLAYOFF); // To switch display off
    } 
  }

// LEDs updated every 0.25 second
if (cm - pmLeds >= 250) { 
  pmLeds = cm;  // Remember the time
  updateLeds();
  }

// For testing - check thermistor every 2 seconds - thermistor available if core temperature is a factor
if ((cm - pmTherm >= 2000) && (thermistorReportFlag == 1)){ 
  pmTherm = cm;  // Remember the time
  // analog read resolution set earlier to 12 bits 0-4096
  thermistorRead = analogRead(thermistorPin); 
  // old 1600 = 110deg, 1700 = 100deg , 1800 = 90deg, 1900 = 80deg  approx
  // 2200 is 65deg
  delay(1); // better read after delay
  thermistorRead = 0;
  int rCnt;
  for (rCnt = 0; rCnt < 16; rCnt++){
    thermistorRead = thermistorRead + analogRead(thermistorPin); 
    delay(1);
    } 
  thermistorRead = thermistorRead / rCnt; 
  thermistorValue = 35 + ((2300 - thermistorRead) / 10);
    // 2100 is 65
  if (thermistorReportFlag == 1){
    Serial.print("thermistorValue "); 
    Serial.print("  ");
    Serial.print(thermistorValue); // int - no decimal places
    Serial.print(" deg  ");
    Serial.println(thermistorRead); 
    }
  }

// For testing - check voltages every 2 seconds - runs if a voltage flag is on - for testing
if ((cm - pmVoltage >= 2000) && ((inputVoltageReportFlag == 1) || (VsVoltageReportFlag == 1))) {
  pmVoltage = cm;  // Remember the time
  // analog read resolution set earlier to 12 bits 0-4096
  inputVoltageRead = analogRead(inputVoltagePin);
  VsVoltageRead = analogRead(VsVoltagePin);
  delay(1); // better read after delay
  inputVoltageRead = 0;
  VsVoltageRead = 0;
  int rCnt;
  for (rCnt = 0; rCnt < 16; rCnt++){
    inputVoltageRead = inputVoltageRead + analogRead(inputVoltagePin); 
    VsVoltageRead = VsVoltageRead + analogRead(VsVoltagePin); 
    delay(1);
    } 
  inputVoltageRead = inputVoltageRead / rCnt; 
  inputVoltageValue = (((float)inputVoltageRead * 3.3 * 11) / 4096) + 0.6;  // input was 1 / 11 and 4096 is full 
  // scale with 3.3 volts - + 0.6 is correction factor from volt meter measurement
  VsVoltageRead = VsVoltageRead / rCnt; 
  VsVoltageValue = (((float)VsVoltageRead * 3.3 * 3) / 4096) + 0.25;  // input was 1 / 3 and 4096 is full 
  // scale with 3.3 volts - + 0.25 is correction factor from volt meter measurement
  if (inputVoltageReportFlag == 1){
    Serial.print("inputVoltageValue "); // for testing
    Serial.print("  ");
    Serial.println(inputVoltageValue, 1); // one decinal place
    }
  if (VsVoltageReportFlag == 1){
    Serial.print("VsVoltageValue "); // for testing
    Serial.print("  ");
    Serial.println(VsVoltageValue, 2); // two decinal places
    }
  }







// end loop
}  

void relayOn()
{
relayState = 1; // relay on
redLedState = 1; // led on
if (relayState != prior_relayState){
  digitalWrite(relayPin, HIGH); // turn relay on - active high
  delay(500);  // add delay for relay time and remote startup time
  prior_relayState = relayState; 
  if (relayReportFlag == 1){
    Serial.println("relay on");
    }
  }
updateLeds();
}
  
void relayOff()
{  
relayState = 0; // relay off
redLedState = 0; // turn off red led
if (relayState != prior_relayState){
  digitalWrite(relayPin, LOW);  // active high
  prior_relayState = relayState;
  if (relayReportFlag == 1){
    Serial.println("relay off");
    }
  }
updateLeds();
}

void updateLeds()
{
/*
for built in rgb LED
setColor(255, 0, 0);   // Red
setColor(0, 255, 0);   // Green
setColor(0, 0, 255);   // Blue
setColor(255, 255, 0); // Yellow
setColor(0, 255, 255); // Cyan
setColor(255, 0, 255); // Magenta
setColor(255, 255, 255); // White  
setColor(0, 0, 0); // Off
*/
if (blueLedState == 0 && redLedState == 0){
  setColor(0, 0, 0); // Off
  digitalWrite(blueLedPin, HIGH); // active low
  digitalWrite(redLedPin, HIGH); // active low
  return;
  }
if (blueLedState == 1 && redLedState == 0){
  setColor(0, 0, 127); // blue
  digitalWrite(blueLedPin, LOW); // active low
  digitalWrite(redLedPin, HIGH); // active low
  return;
  }
if (blueLedState == 0 && redLedState == 1){
  setColor(15, 0, 0); // red
  digitalWrite(blueLedPin, HIGH); // active low
  digitalWrite(redLedPin, LOW); // active low
  return;
  }
if (blueLedState == 1 && redLedState == 1){
  setColor(15, 0, 63); // magenta
  digitalWrite(blueLedPin, LOW); // active low
  digitalWrite(redLedPin, LOW); // active low
  return;
  }
}

// Function to set RGB color
void setColor(uint8_t r, uint8_t g, uint8_t b) 
{
// If LED is common anode, invert values: 255 - value
ledcWrite(CH_R, 255-r);
ledcWrite(CH_G, 255-g);
ledcWrite(CH_B, 255-b);
}

void addSpacesL2() // Insert space after every 4 characters, except at the end
{  // oled_line2_str
  int j = 0; int i = 0; char astext_str[32];
  for (i = 0; i < 16; i++) {
    astext_str[j++] = oled_line2_str[i];
    if ((i + 1) % 4 == 0) {astext_str[j++] = ' ';} 
    }
  astext_str[j] = '\0';
  strcpy(oled_line2_str, astext_str);
}

void addSpacesL3() // Insert space after every 4 characters, except at the end
{  // oled_line3_str
  int j = 0;   int i = 0;   char astext_str[32];
  for (i = 0; i < 16; i++) {
    astext_str[j++] = oled_line3_str[i];
    if ((i + 1) % 4 == 0) {astext_str[j++] = ' ';} 
    }
  astext_str[j] = '\0';
  strcpy(oled_line3_str, astext_str);
}

// oledText(char *text, int x, int y,int size, boolean d)
//   text is the text string to be printed
//   x is the integer x position of text
//   y is the integer y position of text
//   z is the text size - supports sizes from 1 to 8
//   d is either "true" or "false". True draws the display - not used here -display.display used instead
void oledText(char *text, int x, int y, int size, boolean d)
{
display.setTextSize(size);
display.setTextColor(WHITE);
display.setCursor(x,y);
display.println(text);
// display.display();  // draws display by flushing all changes to the display hardware
// we only draw display after the rectangle is drawn below 
// oledDisplay_display();  // includes display.display - use for blanking display
}

// draws oled rectangle around three text lines
void oledRect() 
{
display.drawRect(0, 0, 128, 64, WHITE);  // 128 by 64 is maximum - one pixel line
// display.display(); // draws display by flushing all changes to the display hardware
oledDisplay_display();  // includes display.display - use for blanking display
//other OLED display commands
//display.setTextColor(BLACK, WHITE); //reverse
//display.drawPixel(pixelX, pixelY, color) //single pixel
//display.width() //returns value
//display.height()
//display.drawRect(upperLeftX, upperLeftY, width, height, color)
//display.drawLine(startX, startY, endX, endY, color)
//display.fillRect(upperLeftX, upperLeftY, width, height, WHITE)
//display.drawRoundRect(upperLeftX, upperLeftY, width, height, cornerRadius, color)
//display.drawFillRoundRect(upperLeftX, upperLeftY, width, height, cornerRadius, color)
//display.drawCircle(centerX, centerY, radius, color)
//display.fillCircle(centerX, centerY, radius, color)
//display.drawTriangle(poin1X, point1Y, point2X, point2Y, point3X, point3Y, color)
//display.fillTriangle(poin1X, point1Y, point2X, point2Y, point3X, point3Y, color)
//display.write() function to display a single character
//display.drawBitmap(topLeftX, topLeftY, imageData, width, height, color) // for custom graphic
//display.setRotation(2);  // rotate display 180 choices 0, 1 (90), 2 (180) and 3 (270)
// display.ssd1306_command(SSD1306_DISPLAYOFF); // To switch display off
// display.ssd1306_command(SSD1306_DISPLAYON); // To switch display back on
}

// draws oled display
// replacement for display.display to allow for turning display off to preserve oled life
void oledDisplay_display() {  
   if (displayActiveFlag != 1){
     display.clearDisplay(); // clear oled display 
     }
   display.display(); // draws display by flushing all changes to the display hardware
   }


/*
In the *** IDE Tools menu ***, enter Pin Numbering and choose By GPIO number (legacy);
Make sure the sketch always uses labels to refer to pins. If you used the number 2, 
replace this with the symbol D2 everywhere.  This will switch to a more library-compatible 
scheme and avoid the above confusion.  Do not include GPIO in the number. 
See pin table below.

Nano	ESP32
D0	GPIO 44
D1	GPIO 43
D2	GPIO 5  **** is this esp32 pin 5 ss that conflicts with use of eeprom???  however, this is used with i2c now so probapbly not
D3	GPIO 6
D4	GPIO 7
D5	GPIO 8
D6	GPIO 9  *** do not want to use gpi0 6-11 as also used for integrated flash?? eeprom??? **** 
D7	GPIO 10
D8	GPIO 17
D9	GPIO 18
D10	GPIO 21
D11	GPIO 38
D12	GPIO 47
D13	GPIO 48  also built in led and SPI clock and used with Blynk for link indication
A0	GPIO 1
A1	GPIO 2
A2	GPIO 3
A3	GPIO 4
A4	GPIO 11
A5	GPIO 12
A6	GPIO 13
A7	GPIO 14
BOOT0	GPIO 46  also Red on rgb led 
BOOT1	GPIO 0   also Green on rgb led

GPIO 45 is not on board pinout but is Blue on rgb 
Note - some early boards (not mine) have different rgb colors

*/

