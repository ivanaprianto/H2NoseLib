#ifndef Nose_H
#define Nose_H

#define MAXLENGTH 9

#define DHTLIB_OK                        0
#define DHTLIB_ERROR_CHECKSUM           -1
#define DHTLIB_ERROR_TIMEOUT            -2
#define DHTLIB_ERROR_CONNECT            -3
#define DHTLIB_ERROR_ACK_L              -4
#define DHTLIB_ERROR_ACK_H              -5

#include <Arduino.h>

class Nose
{
    public:
        Nose(int pin1, int pin2, bool isPPB, float b, float m, float ratioInCleanAir, bool isMG811, String gasType, float rl, bool comm = false);
        Nose(int pin[2], float v[2]);
        Nose(int pin[2], float rl[2], int gastype, bool comm = false);

        //general functions
        void printOutput();
        void printOutputBoth(bool inject = false);
        void setRatioInCleanAir(float x);
        void setPin1(int x);
        void setPin2(int x);
        void setRL(float x);
        void setRL(float x[2]);
        void setB(float x);
        void setM(float x);
        void setPPM(float x, float y);
        void setPPM(float x);
        void addUpAll();
        void averageOut(int dataCount);
        void setStart(float ppm, bool air = false, bool prt = false);
        void returnToArray(float outs[2]);

        //get main data
        float getOutput(bool inject = false, float volt = 0.0, float rl = 0.0);

        //other data
        float getRL();
        float getRL(float outs[2]);
        float getB();
        float getM();
        float getVoltage();

        //tools
        float calculateRLoffset(float targetRL, float targetPPM);
        float calculateGradient(float b, float targetPPM);
        float calculateIntersect(float m, float targetPPM);
        float calculateCurrentRL(float targetPPM);
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
        float _readout1;
        float _readout2;
        float _buffer;
        float _buffer_final;
        float _volt;
        float _volt1;
        float _volt2;
        float _RS_gas;
        float _ratio;
        float _ppm_log;
        float _ppm;
        float _ppm1;
        float _ppm2;
        float _ppb;
        float _RL;
        float _RL1;
        float _RL2;
        bool _com;
        float _total;
        float _total1;
        float _total2;
        // MQ2-H2
        const double m2_h2 = -1.1690194965;
        const double b2_h2 = 3.99626591;
        // MQ4-H2 
        const double m4_h2 = -0.1528128737;
        const double b4_h2 = 0.88540934406;
        // MQ6-H2 
        const double m6_h2 = -0.2596918708;
        const double b6_h2 = 1.10129490713;
        // MQ7-H2 
        const double m7_h2 = -0.7106886988;
        const double b7_h2 = 1.3473178615;
        // MQ8-H2 
        const double m8_h2 = -1.4418066958;
        const double b8_h2 = 4.23409440355;
        // MQ2-CO
        const double m2_co = -0.3128241909;
        const double b2_co = 1.42738802266;
        // MQ7-CO
        const double m7_co = -0.6705969873;
        const double b7_co = 1.4441096915;
        // MQ2-CH4
        const double m2_ch4 = -0.3720037523;
        const double b2_ch4 = 1.33311304481;
        // MQ4-CH4
        const double m4_ch4 = -0.3613714269;
        const double b4_ch4 = 1.1001300621;
        // MQ6-CH4
        const double m6_ch4 = -0.4017608236;
        const double b6_ch4 = 1.33943705422;
        // MQ9-CH4
        const double m9_ch4 = -0.3803855584;
        const double b9_ch4 = 1.36664027361;
        // MQ3-C6H6
        const double m3_benzene = -2.68;
        const double b3_benzene = 4.8387;
        // MQ2-C3H8
        const double m2_propane = -0.4756496415;
        const double b2_propane = 1.3504783634;
        // MQ2-OH
        const double m2_alcohol = -0.370747619784;
        const double b2_alcohol = 1.31549939184;
        // MQ3-OH
        const double m3_alcohol = -1.504;
        const double b3_alcohol = 0.3934;
        // MQ8-OH
        const double m8_alcohol = -0.599026918876;
        const double b8_alcohol = 2.77631891722;
        // MQ2-LPG
        const double m2_lpg = -0.4900080111;
        const double b2_lpg = 1.3688274357;
        // MQ4-LPG 
        const double m4_lpg = -0.274092305273;
        const double b4_lpg = 0.971430484482;
        // MQ6-LPG
        const double m6_lpg = -0.356813804571;
        const double b6_lpg = 1.14325856192;
        // MQ9-LPG 
        const double m9_lpg = -0.473054469946;
        const double b9_lpg = 1.41073181966;
        const float rsr02 = 9.83;
        const float rsr03 = 60.0;
        const float rsr04 = 4.4;
        const float rsr06 = 10.0;
        const float rsr07 = 27.5;
        const float rsr08 = 70.0;
        const float rsr09 = 9.6;
};

class Thermocouple
{
    public:
        Thermocouple(int8_t SCLK, int8_t CS, int8_t MISO, String identifier);
        void printTemps();
        float readTemps(void);

    private:
        int8_t _sclk, _cs, _miso;
        String _identifier;
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

class DHT22
{
    public:
        DHT22(uint8_t pin, bool disableIRQ = false);
        void printOutput();
        int8_t read();
        float humidity;
        float temperature;

    private:
        uint8_t bits[5];
        int8_t _readSensor(uint8_t pin, uint8_t wakeupDelay, uint8_t leadingZeroBits);
        bool   _disableIRQ;
        int    _pin;
};

#endif
