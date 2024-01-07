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
  int n = accel.getFIFOEntries();

  Serial.print("Current Frame have FIFO: "); Serial.println(n);

  for(int i = 0; i < n; ++i){
    int32_t x, y, z;
    accel.getFIFOData(x, y, z);

    /* Display the results (acceleration is measured in gs */
    Serial.print("x:"); Serial.print(x * accel.getScale(), 12); Serial.print("\t");
    Serial.print("y:"); Serial.print(y * accel.getScale(), 12); Serial.print("\t");
    Serial.print("z:"); Serial.print(z * accel.getScale(), 12); 
    Serial.print("\r\n");
  }
}
