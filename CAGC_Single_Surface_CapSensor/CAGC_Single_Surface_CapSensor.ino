/*SIMPLIFIED ONE SURFACE CAP SENSOR EXAMPLE
based on this example....


Example code for a Flora game controller with capacitive touch sensing! Full tutorial and video:
 
 http://learn.adafruit.com/plush-game-controller/
 
 Uses Modern Device's Capacitive Sensing library: https://github.com/moderndevice/CapSense
 
 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!
 
 Written by Limor Fried & Becky Stern for Adafruit Industries.
 BSD license, all text above must be included in any redistribution
 */
#include <CapPin.h>

CapPin cPin_10 = CapPin(10); // read pin 10 (D10 on Flora) - connect to NES B
CapPin pins = cPin_10;

char Keys = 'a';

boolean currentPressed = false;
// Capactive touch threashhold, you might want to mess with this if you find its too
// sensitive or not sensitive enough
#define THRESH 500

float smoothed = 0;

void setup()
{
  //while (!Serial)
  Serial.begin(115200);
  Serial.println("start");
  Keyboard.begin();
}


void loop()
{
  for (int i=0;i<8;i++) 
    delay(1);
    long total1 = 0;
    long start = millis();
    long total = pins.readPin(2000);
    //Serial.println(total); //UNCOMMENT IF YOU WANT TO SEE VALUES

    // check if we are sensing that a finger is touching the pad
    // and that it wasnt already pressed
    if ((total > THRESH) && (! currentPressed)) {
      Serial.print("Key pressed #"); 
      Serial.print(" ("); 
      Serial.print(Keys); 
      Serial.println(")");
      currentPressed = true;

      Keyboard.press(Keys);
    }
    else if ((total <= THRESH) && (currentPressed)) {
      // key was released (no touch, and it was pressed before)
      Serial.print("Key released #"); 
      Serial.print(" ("); 
      Serial.print(Keys); 
      Serial.println(")");
      currentPressed = false;
      Keyboard.release(Keys);
    }
    /*
    // simple lowpass filter to take out some of the jitter
     // change parameter (0 is min, .99 is max) or eliminate to suit
     smoothed[i] = smooth(total, .8, smoothed[i]);
     
     Serial.print(i); Serial.print(": ");
     Serial.print( millis() - start); // time to execute in mS
     Serial.print("ms \t");
     Serial.print(total); // raw total
     Serial.print("\t->\t");
     Serial.println((int) smoothed[i]); // smoothed
     */
    delay(5);
  
}

// simple lowpass filter
// requires recycling the output in the "smoothedVal" param
int smooth(int data, float filterVal, float smoothedVal){

  if (filterVal > 1){ // check to make sure param's are within range
    filterVal = .999999;
  }
  else if (filterVal <= 0){
    filterVal = 0;
  }

  smoothedVal = (data * (1 - filterVal)) + (smoothedVal * filterVal);

  return (int)smoothedVal;
}

