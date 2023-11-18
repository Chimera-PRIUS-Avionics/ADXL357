#include <Wire.h>
#include <ADXL357.h>

ADXL357 accel = ADXL357(false);

void setup(void)
{
  Serial.begin(9600);
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

  Serial.println("");
}

void loop(void)
{
  double x, y, z;

  x = accel.getX() * ADXL357_SCL_40G;
  y = accel.getY() * ADXL357_SCL_40G;
  z = accel.getZ() * ADXL357_SCL_40G;

  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("X: "); Serial.print(x); Serial.print("g  ");
  Serial.print("Y: "); Serial.print(y); Serial.print("g  ");
  Serial.print("Z: "); Serial.print(z); Serial.print("g  ");
  delay(500);
}
