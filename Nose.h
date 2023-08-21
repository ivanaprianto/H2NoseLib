#ifndef Nose_H
#define Nose_H

#define MAXLENGTH 9

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
        void setPPM(float x);

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

class ZE07H2
{
    public:
        ZE07H2(HardwareSerial *Serial);	//read the uart signal by hardware uart,such as D0
        //   ZE07H2(SoftwareSerial *Serial);	//read the uart signal by software uart,such as D10
        ZE07H2(int pin,float ref);			//read the analog signal by analog input pin ,such as A2; ref:voltage on AREF pin
        boolean available(uint16_t timeout);		//new data was recevied
        float uartReadPPM();		//get the concentration(ppm) by uart signal
        float dacReadPPM();		//get the concentration(ppm) by analog signal

    private:
        Stream *mySerial;
        byte receivedCommandStack[MAXLENGTH];
        byte checkSum(byte array[],byte length);
        void boucle();
        boolean receivedFlag;
        byte _sensorPin;
        enum STAT_ZE07 { STATUS_WAITING, STATUS_OK };	
        STAT_ZE07 _status;
        uint8_t _index = 0;
        float _ref;
};

#endif