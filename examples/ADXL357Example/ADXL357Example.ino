#include <Wire.h>
#include <ADXL357.h>

ADXL357 accel = ADXL357(false);

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

  Serial.println("");
}

void loop(void)
{
  double x, y, z;

  double g = 9.81;

  x = accel.getX() * accel.getScale() * g;
  y = accel.getY() * accel.getScale() * g;
  z = accel.getZ() * accel.getScale() * g;

  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("Variable_1:"); Serial.print(x, 12); Serial.print("\t");
  Serial.print("Variable_2:"); Serial.print(y, 12); Serial.print("\t");
  Serial.print("Variable_3:"); Serial.print(z, 12); 
  Serial.print("\r\n");
  delay(1);
}
