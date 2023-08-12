#ifndef Nose_H
#define Nose_H

#include <Arduino.h>

class Nose
{
    public:
        Nose(int pin1, int pin2, bool isPPB, float b, float m, float rsr0, bool isMG811, String gastype, float rl, bool comm);
        float getOutput();
        void printOutput();
        void setRSR0(float x);
        void setPin1(int x);
        void setPin2(int x);
        void setRL(float x);
    private:
        int _pin1;
        int _pin2;
        bool _isPPB;
        float _b;
        float _m;
        float _rsr0;
        bool _isMG811;
        String _gastype;
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
};

#endif