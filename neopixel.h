//#include <NeoPixelBus.h>
#include <NeoPixelBrightnessBus.h> // instead of NeoPixelBus.h

const uint16_t PixelCount = 24; // this example assumes 4 pixels, making it smaller will cause a failure
const uint8_t PixelPin = 2;  // make sure to set this to the correct pin, ignored for Esp8266

#define colorSaturation 255 // saturation of color constants
RgbColor red(colorSaturation, 0, 0);
RgbColor green(0, colorSaturation, 0);
RgbColor blue(0, 0, colorSaturation);

// Make sure to provide the correct color order feature
// for your NeoPixels
NeoPixelBrightnessBus<NeoRgbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);

/*
#define colorSaturation 128

// three element pixels, in different order and speeds
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);
//NeoPixelBus<NeoRgbFeature, Neo400KbpsMethod> strip(PixelCount, PixelPin);

// For Esp8266, the Pin is omitted and it uses GPIO3 due to DMA hardware use.  
// There are other Esp8266 alternative methods that provide more pin options, but also have
// other side effects.
// for details see wiki linked here https://github.com/Makuna/NeoPixelBus/wiki/ESP8266-NeoMethods 

// You can also use one of these for Esp8266, 
// each having their own restrictions
//
// These two are the same as above as the DMA method is the default
// NOTE: These will ignore the PIN and use GPI03 pin
//NeoPixelBus<NeoGrbFeature, NeoEsp8266Dma800KbpsMethod> strip(PixelCount, PixelPin);
//NeoPixelBus<NeoRgbFeature, NeoEsp8266Dma400KbpsMethod> strip(PixelCount, PixelPin);

// Uart method is good for the Esp-01 or other pin restricted modules
// for details see wiki linked here https://github.com/Makuna/NeoPixelBus/wiki/ESP8266-NeoMethods 
// NOTE: These will ignore the PIN and use GPI02 pin
//NeoPixelBus<NeoGrbFeature, NeoEsp8266Uart1800KbpsMethod> strip(PixelCount, PixelPin);
//NeoPixelBus<NeoRgbFeature, NeoEsp8266Uart1400KbpsMethod> strip(PixelCount, PixelPin);

// The bitbang method is really only good if you are not using WiFi features of the ESP
// It works with all but pin 16
//NeoPixelBus<NeoGrbFeature, NeoEsp8266BitBang800KbpsMethod> strip(PixelCount, PixelPin);
//NeoPixelBus<NeoRgbFeature, NeoEsp8266BitBang400KbpsMethod> strip(PixelCount, PixelPin);

// four element pixels, RGBW
//NeoPixelBus<NeoRgbwFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);

RgbColor red(colorSaturation, 0, 0);
RgbColor green(0, colorSaturation, 0);
RgbColor blue(0, 0, colorSaturation);
*/
RgbColor white(colorSaturation);
RgbColor black(0);

HslColor hslRed(red);
HslColor hslGreen(green);
HslColor hslBlue(blue);
HslColor hslWhite(white);
HslColor hslBlack(black);

// you loose the original color the lower the dim value used
// here due to quantization
const uint8_t c_MinBrightness = 8; 
const uint8_t c_MaxBrightness = 255;
uint8_t maxBrightness = 255;

void setNeoLedsByStr(const String& s) {
  int colonIndex = s.indexOf(':');
  String key = s.substring(0, colonIndex);
  String val = s.substring(colonIndex + 1);

  if (key == "all") {
    int color = (int) strtol(val.c_str() + 1, 0, 16);
    for (int i = 0; i < 24; i++) {
      strip.SetPixelColor(i, RgbColor(color >> 16, (color >> 8) & 0xFF, color & 0xFF));
    }
    Serial.print("Set all LEDs to ");
    Serial.print(String(color >> 16, HEX));
    Serial.print(String((color >> 8) & 0xFF, HEX));
    Serial.print(String(color & 0xFF, HEX));
  } else {
    int dashIndex = key.indexOf('-');
    if (dashIndex != -1) {
      int leftIndex = key.substring(0, dashIndex).toInt();
      int rightIndex = key.substring(dashIndex + 1).toInt();
      int color = (int) strtol(val.c_str() + 1, 0, 16);
      for (int i = leftIndex; i <= min(rightIndex, 23); i++) {
        strip.SetPixelColor(i, RgbColor(color >> 16, (color >> 8) & 0xFF, color & 0xFF));
      }
      Serial.print("Set LEDs #" + String(leftIndex) + "-" + String(rightIndex) + " to ");
      Serial.print(String(color >> 16, HEX));
      Serial.print(String((color >> 8) & 0xFF, HEX));
      Serial.print(String(color & 0xFF, HEX));
    } else {
      int ledIndex = key.toInt();
      int color = (int) strtol(val.c_str() + 1, 0, 16);
      strip.SetPixelColor(ledIndex, RgbColor(color >> 16, (color >> 8) & 0xFF, color & 0xFF));
      Serial.print("Set LED " + String(ledIndex) + " to ");
      Serial.print(String(color >> 16, HEX));
      Serial.print(String((color >> 8) & 0xFF, HEX));
      Serial.print(String(color & 0xFF, HEX));
    }
  }
}

int _p = 100;

void setBrightnessPercent(int p) {
  _p=p;
  Serial.println("Setting brightness to " + String(p) + "%, which is " + String(c_MinBrightness + int(p * 0.01 * (maxBrightness - c_MinBrightness))));
  strip.SetBrightness(c_MinBrightness + int(p * 0.01 * (maxBrightness - c_MinBrightness)));
  strip.Show();
}
void setMaxBrightness(uint8_t v) {
  Serial.println("Setting max brightness to " + String(v));
  maxBrightness = v;
//  if (strip.GetBrightness() > maxBrightness)
//    strip.SetBrightness(maxBrightness);
setBrightnessPercent(_p);
}


/*
int i =0;
void setup()
{
  
    Serial.begin(115200);
 //   while (!Serial); // wait for serial attach

 //   Serial.println();
 //   Serial.println("Initializing...");
 //   Serial.flush();

    // this resets all the neopixels to an off state
    strip.Begin();
//    strip.Show();


 //   Serial.println();
 //   Serial.println("Running...");
 
     for(i=0;i<24;i++){
      if((i!=0) || (i!=8) || (i!=16))
       strip.SetPixelColor(i, black);
    }
    strip.SetPixelColor(0, red);
    strip.SetPixelColor(8, green);
    strip.SetPixelColor(16, blue);
 
}


void loop()
{
    

 //   Serial.println("Colors R, G, B, W...");

    // set the colors, 
    // if they don't match in order, you need to use NeoGrbFeature feature

    strip.Show();

    strip.RotateRight(1);

    delay(62);

    }
    */
