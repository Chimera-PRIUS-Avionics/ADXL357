#include <Wire.h>
#include <ADXL357.h>

ADXL357 accel = ADXL357(false);

int32_t xs[32];
int32_t ys[32];
int32_t zs[32];

void setup(void)
{
  Wire.begin();
  Wire.setClock(3400000);

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

  if(!accel.setFilter(
    static_cast<adxl357_filter_t>(
      static_cast<uint8_t>(adxl357_filter_t::HPF_CORNER_NO_HPF)
     | static_cast<uint8_t>(adxl357_filter_t::ODR_LPF_250)) // Change the frequency
     )){
    Serial.println("ADXL 357 Set filter Failed");
    while(1);
  }

  accel.setMode(true);

  Serial.println("");
}

void loop(void)
{
  int n = accel.getAllFIFOData(xs, ys, zs);
  
  for(int i = 0; i < n/3; ++i){
    Serial.print("n:"); Serial.print(n);Serial.print("\t");
    Serial.print("x:"); Serial.print(xs[i]);Serial.print("\t");
    Serial.print("y:"); Serial.print(ys[i]);Serial.print("\t");
    Serial.print("z:"); Serial.print(zs[i]);Serial.print("\t");

    Serial.println("");
  }

}
