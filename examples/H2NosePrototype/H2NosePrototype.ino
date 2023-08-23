#define MQ2_A A0
#define MQ2_B A1
#define MQ3_A A2
#define MQ3_B A3
#define MQ4_A A4
#define MQ4_B A5
#define MQ7_A A8
#define MQ7_B A9
#define MQ8_A A10
#define MQ8_B A11
#define MQ9_A A12
#define MQ9_B A13
#define MG811_A A14
#define MG811_B A15
#include <Nose.h>

const float rsr02 = 9.83;
const float rsr03 = 60.0;
const float rsr04 = 4.4;
const float rsr07 = 27.5;

const double b2_h2 = 3.99626591;
const double m2_h2 = -1.1690194965;

const double b7_co = 1.4441096915;
const double m7_co = -0.6705969873;

const double b4_ch4 = 1.1001300621;
const double m4_ch4 = -0.3613714269;

const double b3_benzene = 4.8387;
const double m3_benzene = -2.68;

const double b3_alcohol = 0.3934;
const double m3_alcohol = -1.504;

const double m2_lpg = -0.4900080111;
const double b2_lpg = 1.3688274357;

const double m2_propane = -0.4756496415;
const double b2_propane = 1.3504783634;

const double MG811_V400 = 0.480;
const double MG811_V4000 = 1.995;

const float rl2 = 70;
const float rl7 = 10;
const float rl4 = 20;
const float rl3 = 200;

int DO = 4;
int CS = 5;
int CLK = 6;

Nose MQ2_H2(MQ2_A, MQ2_B, false, b2_h2, m2_h2, rsr02, false, "H2", rl2, false);
Nose MQ7_CO(MQ7_A, MQ7_B, false, b7_co, m7_co, rsr07, false, "CO", rl7, true);
Nose MG811_CO2(MG811_A, MG811_B, false, MG811_V400, MG811_V4000, rsr02, true, "CO2", 0, true);
Nose MQ4_CH4(MQ4_A, MQ4_B, false, b4_ch4, b4_ch4, rsr04, false, "CH4", rl4, true);
Nose MQ3_C6H6(MQ3_A, MQ3_B, false, b3_benzene, m3_benzene, rsr03, false, "C6H6", rl3, true);
Nose MQ2_C3H8(MQ2_A, MQ2_B, false, b2_propane, m2_propane, rsr02, false, "C3H8", rl2, true);
Nose MQ3_OH(MQ3_A, MQ3_B, false, b3_alcohol, m3_alcohol, rsr03, false, "OH", rl3, true);
Nose MQ2_LPG(MQ2_A, MQ2_B, false, b2_propane, m2_propane, rsr02, false, "LPG", rl2, true);
Thermocouple fff(CLK, CS, DO);

float sensorOuts[8];

void setup() {
  Serial.begin(9600);
  delay(500);
}

void loop() {
  float h2 = MQ2_H2.getOutput();
  float co = MQ7_CO.getOutput();
  float co2 = MG811_CO2.getOutput();
  float ch4 = MQ4_CH4.getOutput();
  float c6h6 = MQ3_C6H6.getOutput();
  float c3h8 = MQ2_C3H8.getOutput();
  float oh = MQ3_OH.getOutput();
  float lpg = MQ2_LPG.getOutput();
  fff.readTemps();

  avgOut(h2, co, co2, ch4, c6h6, c3h8, oh, lpg, 1800, 1);
  MQ2_H2.setPPM(sensorOuts[0]);
  MQ7_CO.setPPM(sensorOuts[1]);
  MG811_CO2.setPPM(sensorOuts[2]);
  MQ4_CH4.setPPM(sensorOuts[3]);
  MQ3_C6H6.setPPM(sensorOuts[4]);
  MQ2_C3H8.setPPM(sensorOuts[5]);
  MQ3_OH.setPPM(sensorOuts[6]);
  MQ2_LPG.setPPM(sensorOuts[7]);
  
  Serial.print("{");
  MQ2_H2.printOutput();
  MQ7_CO.printOutput();
  MG811_CO2.printOutput();
  MQ4_CH4.printOutput();
  MQ3_C6H6.printOutput();
  MQ2_C3H8.printOutput();
  MQ3_OH.printOutput();
  MQ2_LPG.printOutput();
  Serial.println("}");
}

void avgOut(float a, float b, float c, float d, float e, float f, float g, float h, int tries, int interval)
{
  float avga = 0.0;
  float avgb = 0.0;
  float avgc = 0.0;
  float avgd = 0.0;
  float avge = 0.0;
  float avgf = 0.0;
  float avgg = 0.0;
  float avgh = 0.0;

  for(int i = 0; i < tries; i++)
  {
    avga += a;
    avgb += b;
    avgc += c;
    avgd += d;
    avge += e;
    avgf += f;
    avgg += g;
    avgh += h;
    delay(interval);
  }
  sensorOuts[0] = (avga/tries);
  sensorOuts[1] = (avgb/tries);
  sensorOuts[2] = (avgc/tries);
  sensorOuts[3] = (avgd/tries);
  sensorOuts[4] = (avge/tries);
  sensorOuts[5] = (avgf/tries);
  sensorOuts[6] = (avgg/tries);
  sensorOuts[7] = (avgh/tries);
}