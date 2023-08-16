#ifndef Nose_H
#define Nose_H

#include <Arduino.h>

class Nose
{
    public:
        Nose(int pin1, int pin2, bool isPPB, float b, float m, float ratioInCleanAir, bool isMG811, String gasType, float rl, bool comm);

        //general functions
        void printOutput();
        void setRatioInCleanAir(float x);
        void setPin1(int x);
        void setPin2(int x);
        void setRL(float x);
        void setB(float x);
        void setM(float x);

        //get main data
        float getOutput();

        //other data
        float getRL();
        float getB();
        float getM();
        float getVoltage();

        //tools
        float calculateRLoffset(float targetRL, float targetPPM);
        float calculateGradient(float b, float targetPPM);
        float calculateIntersect(float m, float targetPPM);
        float calculateCurrentRL(float targetPPM);
        float gradientDescent(float o, float h, float lr, float c);
        float calibrate();

    private:
        int _pin1;
        int _pin2;
        bool _isPPB;
        float _b;
        float _m;
        float _R0;
        bool _isMG811;
        String _gasType;
        float _readout;
        float _buffer;
        float _buffer_final;
        float _volt;
        float _RS_gas;
        float _ratio;
        float _ppm_log;
        float _ppm;
        float _ppb;
        float _RL;
        bool _com;
        float f(float x, float o);
        float d(float x, float h, float o);
};

class Thermocouple
{
    public:
        Thermocouple(int8_t SCLK, int8_t CS, int8_t MISO);
        float readTemps(void);

    private:
        int8_t _sclk, _cs, _miso;
        uint8_t spiRead(void);
};

#endif