#include <Arduino.h>
#include <HUSKYLENS.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Wire.h>
// #include <Adafruit_GFX.h>

HUSKYLENS huskylens;
//HUSKYLENS green line >> SDA; blue line >> SCL

HUSKYLENSResult result;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define LOGO_HEIGHT   25
#define LOGO_WIDTH    48
static const unsigned char PROGMEM bmp_left[] =
{ 0b00000000,0b00000000,0b00000000, 0b11000000, 0b00000000, 0b00000000, 
  0b00000000,0b00000000,0b00000000, 0b11110000, 0b00000000, 0b00000000, 
  0b00000000,0b00000000,0b00000000, 0b11111100, 0b00000000, 0b00000000, 
  0b00000000,0b00000000,0b00000000, 0b11111111, 0b00000000, 0b00000000,
  0b00000000,0b00000000,0b00000000, 0b11111111, 0b11000000, 0b00000000,
  0b00000000,0b00000000,0b00000000, 0b11111111, 0b11110000, 0b00000000,
  0b00000000,0b00000000,0b00000000, 0b11111111, 0b11111100, 0b00000000,
  0b00000000,0b00000000,0b00000000, 0b11111111, 0b11111111, 0b00000000,
  0b00000000,0b00000000,0b00000000, 0b11111111, 0b11111111, 0b11000000,
  0b11111111,0b11111111,0b11111111, 0b11111111, 0b11111111, 0b11110000,
  0b11111111,0b11111111,0b11111111, 0b11111111, 0b11111111, 0b11111100,
  0b11111111,0b11111111,0b11111111, 0b11111111, 0b11111111, 0b11111111,
  0b11111111,0b11111111,0b11111111, 0b11111111, 0b11111111, 0b11111111,
  0b11111111,0b11111111,0b11111111, 0b11111111, 0b11111111, 0b11111111,
  0b11111111,0b11111111,0b11111111, 0b11111111, 0b11111111, 0b11111100,
  0b11111111,0b11111111,0b11111111, 0b11111111, 0b11111111, 0b11110000,
  0b00000000,0b00000000,0b00000000, 0b11111111, 0b11111111, 0b11000000,
  0b00000000,0b00000000,0b00000000, 0b11111111, 0b11111111, 0b00000000,
  0b00000000,0b00000000,0b00000000, 0b11111111, 0b11111100, 0b00000000,
  0b00000000,0b00000000,0b00000000, 0b11111111, 0b11110000, 0b00000000,
  0b00000000,0b00000000,0b00000000, 0b11111111, 0b11000000, 0b00000000, 
  0b00000000,0b00000000,0b00000000, 0b11111111, 0b00000000, 0b00000000, 
  0b00000000,0b00000000,0b00000000, 0b11111100, 0b00000000, 0b00000000,  
  0b00000000,0b00000000,0b00000000, 0b11110000, 0b00000000, 0b00000000,
  0b00000000,0b00000000,0b00000000, 0b11000000, 0b00000000, 0b00000000 };
static const unsigned char PROGMEM bmp_right[] =
{ 0b00000000, 0b00000000, 0b00000011, 0b00000000, 0b00000000, 0b00000000, 
  0b00000000, 0b00000000, 0b00001111, 0b00000000, 0b00000000, 0b00000000, 
  0b00000000, 0b00000000, 0b00111111, 0b00000000, 0b00000000, 0b00000000, 
  0b00000000, 0b00000000, 0b11111111, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000011, 0b11111111, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00001111, 0b11111111, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00111111, 0b11111111, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b11111111, 0b11111111, 0b00000000, 0b00000000, 0b00000000,
  0b00000011, 0b11111111, 0b11111111, 0b00000000, 0b00000000, 0b00000000,
  0b00001111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
  0b00111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
  0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
  0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
  0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
  0b00111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
  0b00001111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
  0b00000011, 0b11111111, 0b11111111, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b11111111, 0b11111111, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00111111, 0b11111111, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00001111, 0b11111111, 0b00000000, 0b00000000, 0b00000000, 
  0b00000000, 0b00000011, 0b11111111, 0b00000000, 0b00000000, 0b00000000, 
  0b00000000, 0b00000000, 0b11111111, 0b00000000, 0b00000000, 0b00000000, 
  0b00000000, 0b00000000, 0b00111111, 0b00000000, 0b00000000, 0b00000000, 
  0b00000000, 0b00000000, 0b00001111, 0b00000000, 0b00000000, 0b00000000, 
  0b00000000, 0b00000000, 0b00000011, 0b00000000, 0b00000000, 0b00000000 };

void left();
void right();


void setup() {
  // put your setup code here, to run once:

 //Serial.begin(115200);
 Serial.begin(9600);
 Wire.begin();
 
  //-------------------------------------------------------------------------------------------------
  //DISPLAYS

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
  display.display();

  //-------------------------------------------------------------------------------------------------
  //HUSKYLENS

      while (!huskylens.begin(Wire))
      {
          Serial.println(F("Begin failed!"));
          Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>>I2C)"));
          Serial.println(F("2.Please recheck the connection."));
          delay(100);
      }

      huskylens.writeAlgorithm(ALGORITHM_OBJECT_TRACKING); //Switch the algorithm to object tracking.

}

void loop() {
  delay(50); //delay for stability
  
  int x;
  int w;
  int h;
  //check if Huskylens has seen something
  if (!huskylens.request()) Serial.println(F("Fail to request objects from HUSKYLENS!"));
  else if(!huskylens.isLearned()) Serial.println(F("Object not learned!"));
  else if(!huskylens.available()){
    //Serial.println(F("Object disappeared!"));
    display.clearDisplay();
    display.display();
  }
  else{  //if sees a car
      result = huskylens.read();  //read object data from Huskylens
      x = result.xCenter;
      w = result.width;
      h = result.height;
      delay(500);
      if(huskylens.count() > 0){ //if it sees an object
        if(w > 10 && h > 10){  //check object size to ensure no early indication
          if(x < 160){  //check if on right side
            right();  //signal right
          }
          else if(x > 160){  //check if on left side
            left();  //signal left
          }
        }
      }
  }

}

void right() {  //send right indicator to display
  display.clearDisplay();
  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT),
    bmp_right, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
  delay(1000);
}
void left() {  //send left indicator to display
  display.clearDisplay();
  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT),
    bmp_left, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
  delay(1000);
}