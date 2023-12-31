#include <Wire.h>
#include <ADXL357.h>

ADXL357 accel = ADXL357(false);

double scale = 0;
double g;

void setup(void)
{
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Accelerometer Test"); Serial.println("");

  if(!accel.begin())
  {
    Serial.println("ADXL 357 Not Found!");
    while(1);
  }

  accel.setMode(false);

  if(!accel.setRange(adxl357_range_t::Range_40_G)){
    Serial.println("ADXL 357 Set Range Failed");
    while(1);
  }

  accel.setMode(true);

  scale = accel.getScale();
  g = 9.81;

  Serial.println("");
}

void loop(void)
{
  int32_t x, y, z;

  if(accel.getXYZ(x, y, z)){
    /* Display the results (acceleration is measured in m/s^2) */
    Serial.print("Variable_1:"); Serial.print(x * scale * g, 12); Serial.print("\t");
    Serial.print("Variable_2:"); Serial.print(y * scale * g, 12); Serial.print("\t");
    Serial.print("Variable_3:"); Serial.print(z * scale * g, 12); 
    Serial.print("\r\n");
  }
  delay(1);
}
