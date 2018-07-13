#include <MqttConnector.h>

extern Adafruit_ADS1115 ads1115;
extern LiquidCrystal_I2C lcd;
extern int relayPinState;
extern MqttConnector* mqtt;
extern int relayPin;
extern int val;
extern int x;
extern char myName[];

static void readSensor();

extern String DEVICE_NAME;
extern int PUBLISH_EVERY;

int multiMap(int val, int* _in, int* _out, uint8_t size)
{
  // take care the value is within range
  // val = constrain(val, _in[0], _in[size-1]);
  if (val <= _in[0]) return _out[0];
  if (val >= _in[size - 1]) return _out[size - 1];

  // search right interval
  uint8_t pos = 1;  // _in[0] allready tested
  while (val > _in[pos]) pos++;

  // this will handle all exact "points" in the _in array
  if (val == _in[pos]) return _out[pos];

  // interpolate in the right segment for the rest
  return (val - _in[pos - 1]) * (_out[pos] - _out[pos - 1]) / (_in[pos] - _in[pos - 1]) + _out[pos - 1];
}


void register_publish_hooks() {
  strcpy(myName, DEVICE_NAME.c_str());
  mqtt->on_prepare_data_once([&](void) {
    Serial.println("initializing sensor...");
  });

  mqtt->on_before_prepare_data([&](void) {
    readSensor();
  });

  mqtt->on_prepare_data([&](JsonObject * root) {
    JsonObject& data = (*root)["d"];
    JsonObject& info = (*root)["info"];
    data["myName"] = myName;
    data["millis"] = millis();
    data["Index"] = x;
    data["relayState"] = relayPinState;
    data["updateInterval"] = PUBLISH_EVERY;
  }, PUBLISH_EVERY);
  mqtt->on_after_prepare_data([&](JsonObject * root) {
    /**************
      JsonObject& data = (*root)["d"];
      data.remove("version");
      data.remove("subscription");
    **************/
  });
}

static void readSensor() {
  // perform reading sensor
  Serial.println("Perform reading sensor...");
  int16_t adc0;
  adc0 = ads1115.readADC_SingleEnded(0)/8;
  float sensorVoltage;
  float sensorValue;
  Serial.print("AIN0: "); Serial.println(adc0);
  Serial.println(" ");
  sensorValue = adc0;
  sensorVoltage = sensorValue / 1024 * 5;
  int out[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
  int in[]  = {10, 46, 65, 83, 103, 124, 142, 162, 180, 200, 221, 240};
  
  x = multiMap(sensorValue, in, out, 12);
  
  
  Serial.print("reading = ");
  Serial.print(sensorValue);
  Serial.print("\t");
  lcd.setCursor(10, 0);
  lcd.print(sensorValue);
  lcd.print("  ");
  Serial.print("voltage = ");
  Serial.print(sensorVoltage);
  Serial.println(" V");
  lcd.setCursor(10, 2);
  lcd.print(sensorVoltage);
  lcd.print("  ");
  lcd.setCursor(17, 0);
  lcd.print(x);
  lcd.print(" ");
  Serial.print(x);
  Serial.print("\t");
  delay(1000);






  
}
