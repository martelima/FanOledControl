/*
Project made By Mateus From Materpillar
*/

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 _display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int ANALOG_INPUT_PIN = A0;
const int MIN_ANALOG_INPUT = 0;
const int MAX_ANALOG_INPUT = 1023;
const int DELAY_LOOP_MS = 5; // change to slow down how often to read and graph value
//const int MOTOR_OUTPUT 3;
#define PinOutMotor 3
int _circularBuffer[SCREEN_WIDTH]; //fast way to store values 
int _curWriteIndex = 0; // tracks where we are in the circular buffer

// for tracking fps
float _fps = 0;
unsigned long _frameCount = 0;
unsigned long _fpsStartTimeStamp = 0;

// status bar
boolean _drawStatusBar = true; // change to show/hide status bar
int _graphHeight = SCREEN_HEIGHT;

void setup() {
  pinMode(PinOutMotor,OUTPUT);
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!_display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  // Clear the buffer
  _display.clearDisplay();

  _display.setTextSize(1);
  _display.setTextColor(WHITE, BLACK);
  _display.setCursor(0, 0);
  _display.println("Iniciando sistema...");
  _display.setTextSize(3);
  _display.setCursor(0, 16);
  _display.println(" MOURA");
  _display.setCursor(0, 45);
  _display.setTextSize(1);
  _display.println("     MATERPILLAR");

_display.setTextSize(1);
  _display.display();
  
  delay(5000);
  _display.clearDisplay();

  if(_drawStatusBar){
    _graphHeight = SCREEN_HEIGHT - 10;
  }

  _fpsStartTimeStamp = millis();
}

void loop() {
  // Clear the display on each frame. We draw from the _circularBuffer
  _display.clearDisplay();

  // Read and store the analog data into a circular buffer
  int analogVal = analogRead(ANALOG_INPUT_PIN);
  Serial.println(analogVal);
  _circularBuffer[_curWriteIndex++] = analogVal;

  // Set the circular buffer index back to zero when it reaches the 
  // right of the screen
  if(_curWriteIndex >= _display.width()){
    _curWriteIndex = 0;
  }
  
  if(_drawStatusBar){
    drawStatusBar(analogVal);
  }
  
  // Draw the line graph based on data in _circularBuffer
  int xPos = 0; 
  for (int i = _curWriteIndex; i < _display.width(); i++){
    int analogVal = _circularBuffer[i];
    drawLine(xPos, analogVal);
    xPos++;
  }
  
  for(int i = 0; i < _curWriteIndex; i++){
    int analogVal = _circularBuffer[i];
    drawLine(xPos, analogVal);
    xPos++;;
  }
  
  _display.display();
  
  calcFrameRate();
  
  delay(DELAY_LOOP_MS);
}

/**
 * Draw the line at the given x position and analog value
 */
void drawLine(int xPos, int analogVal){
  int lineHeight = map(analogVal, MIN_ANALOG_INPUT, MAX_ANALOG_INPUT, 0, _graphHeight);
  int yPos = _display.height() - lineHeight + 6;
  _display.drawFastVLine(xPos, yPos, lineHeight, SSD1306_WHITE);
}

/**
 * Call this every frame to calculate frame rate
 */
void calcFrameRate() {
    
  unsigned long elapsedTime = millis() - _fpsStartTimeStamp;
  _frameCount++;
  if (elapsedTime > 1000) {
    _fps = _frameCount / (elapsedTime / 1000.0);
    _fpsStartTimeStamp = millis();
    _frameCount = 0;
  }
}

/**
 * Draws the status bar at top of screen with fps and analog value
 */
void drawStatusBar(int analogVal) {

   // erase status bar by drawing all black
  _display.fillRect(0, 0, _display.width(), 8, SSD1306_BLACK); 

  int SensorVelM=analogRead(A1);
  // Draw current val
  _display.setCursor(0, 0);
  _display.print("I:");
  _display.setCursor(10, 0);
  _display.print(analogVal);
  //_display.fillCircle( 27, 1, 1,SSD1306_WHITE);
 // _display.setCursor(30, 0);
 //_display.print("C");

 //temp
  _display.setCursor(47, 0);
  _display.print("T:");
  int valTemp=map(analogVal,0,1023,0,100);
  _display.setCursor(58, 0);
  _display.print(valTemp);
  _display.drawCircle( 77, 1, 1,SSD1306_WHITE);
  _display.setCursor(80, 0);
  _display.print("C");
  

//RPM
  int SensorTemp=analogRead(A0);
  _display.setCursor(99, 0);
  _display.print("R:");
  int valRPM=map(SensorTemp,0,1023,0,210);
  int valRPMPWM=map(SensorTemp,0,1023,0,255);
  _display.setCursor(110, 0);
  _display.print(valRPM);
  analogWrite(PinOutMotor,valRPMPWM);
  

  // Draw frame count
  int16_t x1, y1;
  uint16_t w, h;
  _display.getTextBounds("XX.XX fps", 0, 0, &x1, &y1, &w, &h);
  _display.setCursor(_display.width() - w, 0);
  //_display.print(_fps);
  //_display.print(" fps");
}
