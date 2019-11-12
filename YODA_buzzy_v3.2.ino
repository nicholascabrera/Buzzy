
/*
 * -----------------------------------------------------------------------
 * YODA "Buzzy" Program
 * version: 3.2
 * author: C P Althoff
 * date: 010-OCT-2019
 * 
 * board: Adafruit Feather M0 Express
 * 
 * ----------------------------------------------------------------------
 */

#include <Adafruit_NeoPixel.h>
#include <EasyNeoPixels.h>


// Which pin on the Arduino is connected to the NeoPixels?
#define LED_PIN    8
 
// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 1
 
// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

//-----------------------------------------------------------------------

class Buzzy                         //allows motor to vibrate on/off while the loop runs w/o delays
                                    //this class keeps the NeoPixel lit "solid" for the full duration
{
  // class member variables
  // these are initialized at startup
  int buzzyPin;                     // the number of the buzzy motor pin
  long OnTime;                      // milliseconds of on-time
  long OffTime;                     // milliseconds of off-time

  //these mantain the current state
  int buzzyState;                   //buzzyState used to set the motor
  unsigned long previousMillis;     //will store the last time Buzzy was updated

  // constructor - creates a Buzzy
  // and initializes the member variables and state
  public:
  Buzzy(int pin, long on, long off)
  {
    buzzyPin = pin;
    pinMode(buzzyPin, OUTPUT);

    OnTime = on;
    OffTime = off;

    buzzyState = LOW;
    previousMillis = 0;
  }

  void Update()
  {
    // check to see if it is time to change the state of the Buzzy
    unsigned long currentMillis = millis();

    if ((buzzyState == HIGH) && (currentMillis - previousMillis >= OnTime))
    {
      buzzyState = LOW;                     // turn off the buzzy
      previousMillis = currentMillis;       // remember the time
      digitalWrite(buzzyPin, buzzyState);   // update the buzzy
    }

    else if ((buzzyState == LOW) && (currentMillis - previousMillis >= OffTime))
    {
      buzzyState = HIGH;                    // turn on the buzzy
      previousMillis = currentMillis;       // remember the time
      digitalWrite(buzzyPin, buzzyState);   // update the buzzy
    }

    strip.setPixelColor(0,0,150,0);       // makes the NeoPixel "green"
    strip.show();                         // turns on the NeoPixel
  }

};  // end class definition

//------------------------------------------------------------------------------------------

class Buzzy2                         //allows motor to vibrate on/off while the loop runs w/o delays
                                     //this class version only blinks the NeoPixel when the motor is running
{
  // class member variables
  // these are initialized at startup
  int buzzyPin;                     // the number of the buzzy motor pin
  long OnTime;                      // milliseconds of on-time
  long OffTime;                     // milliseconds of off-time

  //these mantain the current state
  int buzzyState;                   //buzzyState used to set the motor
  unsigned long previousMillis;     //will store the last time Buzzy was updated

  // constructor - creates a Buzzy
  // and initializes the member variables and state
  public:
  Buzzy2(int pin, long on, long off)
  {
    buzzyPin = pin;
    pinMode(buzzyPin, OUTPUT);

    OnTime = on;
    OffTime = off;

    buzzyState = LOW;
    previousMillis = 0;
  }

  void Update()
  {
    // check to see if it is time to change the state of the Buzzy
    unsigned long currentMillis = millis();

    if ((buzzyState == HIGH) && (currentMillis - previousMillis >= OnTime))
    {
      buzzyState = LOW;                     // turn off the buzzy
      previousMillis = currentMillis;       // remember the time
      writeEasyNeoPixel(0, LOW);             // turn off the neopixel
      digitalWrite(buzzyPin, buzzyState);   // update the buzzy
    }

    else if ((buzzyState == LOW) && (currentMillis - previousMillis >= OffTime))
    {
      buzzyState = HIGH;                    // turn on the buzzy
      previousMillis = currentMillis;       // remember the time
      strip.setPixelColor(0,0,150,0);       // makes the NeoPixel "green"
      strip.show();                         // turns on the NeoPixel
      digitalWrite(buzzyPin, buzzyState);   // update the buzzy
    }
  }

};  // end class definition

//------------------------------------------------------------------------------------------

// constants won't change
// use to set pin numbers
const int ledPin = 13;                    // sets LED pin to "13"
const int buttonPin = 10;                 // sets button pin to "10"

// variables will change
int systemState = LOW;                    // the current state of of the system
int buttonState;                          // the current reading from the input pin (after debounce check)
int lastButtonState = HIGH;               // the previous reading from the input pin

// the following variables are long's because the time, measured in milliseconds,
// will quickly become a bigger number than can be stored in an int
long lastDebounceTime = 0;                // the last time the output pin was toggled
long debounceDelay = 100;                 // the debounce time; increase if the output flickers

unsigned long timeout1 = 5000;            // timeout after (milliseconds)
unsigned long timeout2 = 30000;
unsigned long start;                      // used as the "start" for the timer


//------------------------------------------------------------------------------------------

Buzzy motor1(5,200,100);                   //sets motor pin to "5" with "on" and "off" times (milliseconds)
Buzzy2 motor2(5,100,1900);

//------------------------------------------------------------------------------------------

void setup() 
{
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

  strip.begin();
  strip.setBrightness(255);
  strip.show();

  setupEasyNeoPixels(8, 1);

} // close setup

//-------------------------------------------------------------------------------------------

void loop() 
{
  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);

  // check to see if you just pressed the button, and you've waited 
  // long enough since the last press to ignore any noise:  

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) 
  {
    // reset the debouncing timer
    lastDebounceTime = millis();
    
      // toggles the systemState variable each time the button is pressed 
      if (buttonState == LOW) 
      {
      systemState = !systemState;       // toggle system state
      start = millis();                 // sets the "start" for the timer
      } 
  }

  if ((millis() - lastDebounceTime) > debounceDelay)
  {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:
    buttonState = reading;
  }
 
  unsigned long now = millis();           // checks the time "now" in the loop
  unsigned long elapsed1 = now - start;    // updates the elapsed time for the timer
  unsigned long elapsed2 = now - start;
  
  if (systemState == HIGH)
  {
    // runs the buzzy

    if(elapsed1 < timeout1)
    {
      motor1.Update();
    }
    else if(elapsed2 < timeout2)
    {
      motor2.Update();
    }

    else
    {
      //toggle systemState (timeout)
      systemState = LOW;                          
    }
  }

  else if (systemState == LOW)
  {
    // turns off the buzzy
    digitalWrite(5, LOW);                  // turn off the motor
    writeEasyNeoPixel(0, LOW);             // turn off the neopixel
  }
  
    
  //digitalWrite(ledPin, systemState);    //set the LED using the systemState


  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonState = reading;

} // end loop

//--------------------------------------------------------------------------------

// 0 to 255
void brighten() 
{
  uint16_t i, j;

  for (j = 0; j < 255; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, 0, j, 0);
    }
    strip.show();
    delay(10);
  }
}

//--------------------------------------------------------------------------------

// 255 to 0
void darken() 
{
  uint16_t i, j;

  for (j = 255; j > 0; j--) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, 0, j, 0);
    }
    strip.show();
    delay(10);
  }
}

//end program