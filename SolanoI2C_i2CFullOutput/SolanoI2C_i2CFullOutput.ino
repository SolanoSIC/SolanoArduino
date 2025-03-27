#include <Arduino.h>
#include <Wire.h>
#include <math.h>

/** I2C stuff **/
#define SDAM_Pin 5   // select ESP32  I2C pins
#define SCLM_Pin 6
// Define the address of the probe. Here 0x7A is the address of regular probes. 
// If you're using multi-probes, the ones with a "2" on the back have address 0x7B
#define PROBE_ADDRESS 0x7A 
#define I2C_FREQ 400000

/** Data variables**/
uint8_t rawBytesData[18]={};
int16_t data_airspeed = 0;
int16_t data_airspeed_filtered =0;
int16_t data_NS = 0;
int16_t data_NS_filtered = 0;
int16_t data_EW = 0;
int16_t data_EW_filtered = 0;
int16_t data_pdiff_mid = 0;
int16_t data_pdiff_EW = 0;
int16_t data_pdiff_NS = 0;

double angle_NS = 0;
double angle_NS_filtered = 0;
double angle_EW = 0;
double angle_EW_filtered = 0;
double airspeed = 0;
double airspeed_filtered = 0;
double pdiff_mid = 0;
double pdiff_EW = 0;
double pdiff_NS = 0;

// Counter for serial plotting
int counter = 0;
int counterReset = 100000;

void setup() {

  Serial.begin(230400);
  delay(100);  //delay for serial

  //begin standard wire for the probe
  Wire.begin(SDAM_Pin,SCLM_Pin,I2C_FREQ);
  Serial.println("setting up Wire master");

}

void loop() {

  Wire.requestFrom(PROBE_ADDRESS, 12); // request 12 bytes from the probe

  // Grab the bytes data from the probe
  for(int i=0;i<12;i++){
    int c = Wire.read(); // receive a byte as character
    rawBytesData[i]=c;
  }

  // translate the bytes into relevant data
  data_airspeed = (((int16_t)rawBytesData[0]) << 8) | rawBytesData[1];
  data_airspeed_filtered = (((int16_t)rawBytesData[2]) << 8) | rawBytesData[3];
  data_NS = (((int16_t)rawBytesData[4]) << 8) | rawBytesData[5];
  data_NS_filtered = (((int16_t)rawBytesData[6]) << 8) | rawBytesData[7];
  data_EW = (((int16_t)rawBytesData[8]) << 8) | rawBytesData[9];
  data_EW_filtered = (((int16_t)rawBytesData[10]) << 8) | rawBytesData[11];
  data_pdiff_mid = (((int16_t)rawBytesData[12]) << 8) | rawBytesData[13];
  data_pdiff_EW = (((int16_t)rawBytesData[14]) << 8) | rawBytesData[15];
  data_pdiff_NS = (((int16_t)rawBytesData[16]) << 8) | rawBytesData[17];

  airspeed = double(data_airspeed) / 100.0f;
  airspeed_filtered = double(data_airspeed_filtered) / 100.0f;
  angle_NS = double(data_NS) / 100.0f;
  angle_NS_filtered = double(data_NS_filtered) / 100.0f;
  angle_EW = double(data_EW) / 100.0f;
  angle_EW_filtered = double(data_EW_filtered) / 100.0f;
  pdiff_mid = double(data_pdiff_mid) / 100.0f;
  pdiff_EW = double(data_pdiff_EW) / 100.0f;
  pdiff_NS = double(data_pdiff_NS) / 100.0f;
  
  // Number after counter defines the frequency of the data you want printed on the serial port
  counter++;
  if (counter % 10 == 0) {
    print_data(); //this is serial print
  }
  if (counter == counterReset){
    counter = 0; 
  } 
}

void print_data() {
  char buffer[40];

  int i = 0;

  dtostrf(airspeed, 10, 3, buffer);  // 1
  i = i + Serial.print(buffer);
  i = i + Serial.print(";");

  dtostrf(airspeed_filtered, 10, 3, buffer);  // 2
  i = i + Serial.print(buffer);
  i = i + Serial.print(";");

  dtostrf(angle_NS, 10, 3, buffer);  // 3
  i = i + Serial.print(buffer);
  i = i + Serial.print(";");

  dtostrf(angle_NS_filtered, 10, 3, buffer);  // 4
  i = i + Serial.print(buffer);
  i = i + Serial.print(";");

  dtostrf(angle_EW, 10, 3, buffer);  // 5
  i = i + Serial.print(buffer);
  i = i + Serial.print(";");

  dtostrf(angle_EW_filtered, 10, 3, buffer);  // 6
  i = i + Serial.print(buffer);
  i = i + Serial.print("\n");
}

