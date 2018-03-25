// This #include statement was automatically added by the Particle IDE.
#include <neopixel.h>
#include <DS18B20.h>
#include <math.h>

// All temperature code was taken frm the DS18B20 sample code for one sensor

// temperature variables
#define TEMP_PIN D0
const int      MAXRETRY          = 4;
const uint32_t msSAMPLE_INTERVAL = 2500;
const uint32_t msMETRIC_PUBLISH  = 30000;

DS18B20  ds18b20(TEMP_PIN, true); //Sets Pin D2 for Water Temp Sensor and
                            // this is the only sensor on bus
char     szInfo[64];
double   celsius;
double   fahrenheit;
uint32_t msLastMetric;
uint32_t msLastSample;

// LED variables
#define LED_COUNT 5
#define LED_PIN D3
#define LED_TYPE WS2812 // If you are seeing all the lights turn on try setting the LED_TYPE above to WS2811, WS2812 or WS2812B
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN, LED_TYPE);

void setup() {
  Serial.begin(115200);
  strip.begin();
  Particle.variable("temp", fahrenheit);
  Particle.function("setLED", setLED);
}

void loop() {
  if (millis() - msLastSample >= msSAMPLE_INTERVAL){
    getTemp();
  }

  if (millis() - msLastMetric >= msMETRIC_PUBLISH){
    Serial.println("Publishing now.");
    publishData();
  }
}

void publishData(){
  sprintf(szInfo, "%2.2f", fahrenheit);
  Particle.publish("dsTmp", szInfo, PRIVATE);
  msLastMetric = millis();
}

void getTemp(){
  float _temp;
  int   i = 0;

  do {
    _temp = ds18b20.getTemperature();
  } while (!ds18b20.crcCheck() && MAXRETRY > i++);

  if (i < MAXRETRY) {
    celsius = _temp;
    fahrenheit = ds18b20.convertToFahrenheit(_temp);
    Serial.println(fahrenheit);
  }
  else {
    celsius = fahrenheit = NAN;
    Serial.println("Invalid reading");
  }
  msLastSample = millis();
}

// If you are seeing all the lights turn on try setting the LED_TYPE above to WS2812 or WS2812B
int setLED(String command){
    // Get the index of the ending of these values by starting the index look up after preceeding comma
    int idxEnd = command.indexOf(',');
    int redEnd = command.indexOf(',', idxEnd+1);
    int grnEnd = command.indexOf(',', redEnd+1);
    int bluEnd = command.indexOf(',', grnEnd+1);

    // Use the above end positions to get substrings of the values out of the entire string
    int idx = command.substring(0, idxEnd).toInt();
    int red = command.substring(idxEnd+1, redEnd).toInt();
    int grn = command.substring(redEnd+1, grnEnd).toInt();
    int blu = command.substring(grnEnd+1, bluEnd).toInt();
    Particle.publish("ledCmd", String(idx) + " " + String(red) + " " + String(grn) + " " + String(blu), PRIVATE);

    Serial.println(command + " " + String(idx) + " " + String(red) + " " + String(grn) + " " + String(blu));

    //Some LEDs are RGB, some are GRB. The arduino library lets you define this, but the particle one doesnt, so we swap the R and G inputs
    strip.setPixelColor(idx, strip.Color(grn, red, blu));
    strip.show();
    return 1;
}
