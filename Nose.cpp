/*
    library for the h2nose project
*/
#include "Arduino.h"
#include "Nose.h"

Nose::Nose(int pin1, int pin2, bool isPPB, float b, float m, float rsr0, bool isMG811, String gastype, float rl, bool comm)
{
    _pin1 = pin1; //1st sensor
    _pin2 = pin2; //2nd sensor
    _isPPB = isPPB; //for MQ3
    _b = b; //V400 in MG811
    _m = m; //V4000 in MG811
    _rsr0 = rsr0; //RS/R0 ratio
    _isMG811 = isMG811; //for MG811
    _gastype = gastype; //H2, CH4, C3H8, etc.
    _RL = rl; //load resistance
    _com = comm; //separate print with comma
}

void Nose::setRSR0(float x)
{
    _rsr0 = x;
}

void Nose::setPin1(int x)
{
    _pin1 = x;
}

void Nose::setPin2(int x)
{
    _pin2 = x;
}

void Nose::setRL(float x)
{
    _RL = x;
}

void Nose::setB(float x)
{
    _b = x;
}

void Nose::setM(float x)
{
    _m = x;
}

void Nose::getRL()
{
    return _RL;
}

void Nose::getB()
{
    return _b;
}

void Nose::getm()
{
    return _m;
}

float Nose::getOutput()
{
    _readout = (analogRead(_pin1) + analogRead(_pin2)) / 2; //Read analog values of sensors
    _volt = _readout*(5.0/1023); //Convert to voltage
    if(_isMG811){
        _buffer = 0;
        _buffer = (_b - _m)/(log10(400) - log10(40000)); // Delta V
        _buffer = (_volt - _b)/_buffer; 
        _buffer += log10(400);
        _buffer_final = pow(10, _buffer);
        return _buffer_final; //return in ppm
    } else {
        _RS_gas = ((5.0*_RL)/_volt)-_RL; //Get value of RS in a gas
        _ratio = _RS_gas/_rsr0;  // Get ratio RS_gas/RS_air
        _ppm_log = (log10(_ratio)-_b)/_m; //Get ppm value in linear scale according to the the ratio value  
        _ppm = pow(10, _ppm_log); //Convert ppm value to log scale 
        if (_isPPB){
            _ppb = _ppm*1000;
            return _ppb; //return conversion value (parts per billion)
        } else {
            return _ppm; //return conversion value (in parts per million)
        }
    }
}

void Nose::printOutput()
{
    if(_isMG811){
        Serial.print(",\""+_gastype+"\""+":"+_buffer_final);
    } else {
        if (_isPPB){
            Serial.print("\""+_gastype+"\""+":"+_ppb);
        } else {
            if (_com){
                Serial.print(",\""+_gastype+"\""+":"+_ppm); 
            } else {
                Serial.print("\""+_gastype+"\""+":"+_ppm);   
            }
        }
    }
}