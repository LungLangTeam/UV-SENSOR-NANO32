#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
int val,x;
#include <Adafruit_ADS1015.h>
Adafruit_ADS1115 ads1115;



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


void setup()
{
  Serial.begin(9600);
  lcd.begin();
  lcd.setCursor(0, 0);
  lcd.print("reading = ");
  lcd.setCursor(0, 2);
  lcd.print("voltage = ");
  ads1115.setGain(GAIN_ONE);
  ads1115.begin();
}

void loop()
{ int16_t adc0;
  adc0 = ads1115.readADC_SingleEnded(0)/8;
  float sensorVoltage;
  float sensorValue;
  Serial.print("AIN0: "); Serial.println(adc0);
  Serial.println(" ");
  sensorValue = analogRead(adc0);
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
