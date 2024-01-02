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

  if(!accel.setFilter(
    static_cast<adxl357_filter_t>(
      static_cast<uint8_t>(adxl357_filter_t::HPF_CORNER_NO_HPF)
     | static_cast<uint8_t>(adxl357_filter_t::ODR_LPF_250))
     )){
    Serial.println("ADXL 357 Set filter Failed");
    while(1);
  }

  accel.setMode(true);

  Serial.println("");

  double HPF_freq = accel.getHPF();
  double ODR = accel.getODR();
  Serial.print("Current HPF at:\t ");Serial.print(HPF_freq);Serial.print("[Hz], ODR:\t ");Serial.print(ODR);Serial.println("[Hz]");
}

void loop(void)
{
}
