#include <Nose.h>
#define MQ2_A A0
#define MQ2_B A1
#define MQ3_A A2
#define MQ3_B A3
#define MQ4_A A4
#define MQ4_B A5
#define MQ6_A A6
#define MQ6_B A7
#define MQ7_A A8
#define MQ7_B A9
#define MQ8_A A10
#define MQ8_B A11
#define MQ9_A A12
#define MQ9_B A13
#define MG811_A A14
#define MG811_B A15
int M2_H2 = 0;
int M4_H2 = 1;
int M6_H2 = 2;
int M7_H2 = 3;
int M8_H2 = 4;
int M2_CO = 5;
int M7_CO = 6;
int M2_CH4 = 7;
int M4_CH4 = 8;
int M9_CH4 = 9;
int M3_C6H6 = 10;
int M2_C3H8 = 11;
int M3_OH = 12;
int M2_LPG = 13;

const int mq2[2] = {MQ2_A, MQ2_B};
const int mq3[2] = {MQ3_A, MQ3_B};
const int mq4[2] = {MQ4_A, MQ4_B};
const int mq6[2] = {MQ6_A, MQ6_B};
const int mq7[2] = {MQ7_A, MQ7_B};
const int mq8[2] = {MQ8_A, MQ8_B};
const int mq9[2] = {MQ9_A, MQ9_B};
const int mg811[2] = {MG811_A, MG811_B};

float rl2a[2] = {62.59, 62.59};
float rl7a[2] = {19.75, 13.38};
float rl4a[2] = {5.34, 20.12};
float rl3a[2] = {200, 200};
float rl6a[2] = {21.54, 16.54};
float rl8a[2] = {121.39, 133.66};
float rl9a[2] = {72.98, 63.07};
float mg811_v[2] = {0.66, 2.40};

Nose MQ2_H2(mq2, rl2a, M2_H2, true);
Nose MQ4_H2(mq4, rl4a, M4_H2);
Nose MQ6_H2(mq6, rl6a, M6_H2);
Nose MQ7_H2(mq7, rl7a, M7_H2);
Nose MQ8_H2(mq8, rl8a, M8_H2);
Nose MQ2_CO(mq2, rl2a, M2_CO);
Nose MQ7_CO(mq7, rl7a, M7_CO);
Nose MQ2_CH4(mq2, rl2a, M2_CH4);
Nose MQ4_CH4(mq4, rl4a, M4_CH4);
Nose MQ9_CH4(mq9, rl9a, M9_CH4);
Nose MQ3_C6H6(mq3, rl3a, M3_C6H6);
Nose MQ2_C3H8(mq2, rl2a, M2_C3H8);
Nose MQ3_OH(mq3, rl3a, M3_OH);
Nose MQ2_LPG(mq2, rl2a, M2_LPG);
Nose MG811_CO2(mg811, mg811_v);

void setup(){
    MQ2_H2.setStart(300, true);
    MQ8_H2.setStart(300, true);
}

void loop(){
    
}