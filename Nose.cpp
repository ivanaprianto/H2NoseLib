/*
    library for the h2nose project
*/
#include "Arduino.h"
#include "Nose.h"

Nose::Nose(int pin1, int pin2, bool isPPB, float b, float m, float ratioInCleanAir, bool isMG811, String gasType, float rl, bool comm)
{
    _pin1 = pin1; //1st sensor
    _pin2 = pin2; //2nd sensor
    _isPPB = isPPB; //for MQ3
    _b = b; //V400 in MG811
    _m = m; //V4000 in MG811
    _R0 = ratioInCleanAir; //Clean air to R0 ratio
    _isMG811 = isMG811; //for MG811
    _gasType = gasType; //H2, CH4, C3H8, etc.
    _RL = rl; //load resistance
    _com = comm; //separate print with comma
}

Thermocouple::Thermocouple(int8_t SCLK, int8_t CS, int8_t MISO)
{
    _sclk = SCLK;
    _cs = CS;
    _miso = MISO;

    // define pin modes
    pinMode(_cs, OUTPUT);
    pinMode(_sclk, OUTPUT);
    pinMode(_miso, INPUT);

    digitalWrite(_cs, HIGH);
}

void Nose::setRatioInCleanAir(float x)
{
    _R0 = x;
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

float Nose::getRL()
{
    return _RL;
}

float Nose::getB()
{
    return _b;
}

float Nose::getM()
{
    return _m;
}

float Nose::getVoltage()
{
    _readout = (analogRead(_pin1) + analogRead(_pin2)) / 2; //Read analog values of sensors
    _volt = _readout*(5.0/1023); //Convert to voltage
    return _volt;
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
        _ratio = _RS_gas/_R0;  // Get ratio RS_gas/RS_air
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
        Serial.print(",\""+_gasType+"\""+":"+_buffer_final);
    } else {
        if (_isPPB){
            Serial.print("\""+_gasType+"\""+":"+_ppb);
        } else {
            if (_com){
                Serial.print(",\""+_gasType+"\""+":"+_ppm); 
            } else {
                Serial.print("\""+_gasType+"\""+":"+_ppm);   
            }
        }
    }
}

float Nose::calculateRLoffset(float targetRL, float targetPPM) //does nothing, will make it useful later
{
    _readout = (analogRead(_pin1) + analogRead(_pin2)) / 2; //Read analog values of sensors

    //simply reverses the getOutput() function and tries to find the 10bit data of a specific ppm in a specific load resistance
    float targetPPM_log = log10(targetPPM); 
    float c = (targetPPM_log*_m+_b);
    float targetRatio = pow(10, c);
    float targetRS = targetRatio*_R0;
    float targetVolt = (5.0 * targetRL)/(targetRS + targetRL); 
    float targetReadout = (targetVolt/5.0)*1023;

    return _readout - targetReadout;
}

float Nose::calculateCurrentRL(float targetPPM){
    _readout = (analogRead(_pin1) + analogRead(_pin2)) / 2; //Read analog values of sensors
    _volt = _readout*(5.0/1023); //Convert to voltage

    //instead of calculating for readout, it calculates the value of realRL to determine the actual load resistance of the sensor in a specific ppm
    float targetPPM_log = log10(targetPPM);
    float c = (targetPPM_log*_m+_b);
    float targetRatio = pow(10, c);
    float targetRS = targetRatio*_R0;
    float realRL = (_volt*targetRS) / (5.0-_volt);

    return realRL;
}

float Nose::calculateIntersect(float m, float targetPPM){
    _readout = (analogRead(_pin1) + analogRead(_pin2)) / 2; //Read analog values of sensors
    _volt = _readout*(5.0/1023); //Convert to voltage
    _RS_gas = ((5.0*_RL)/_volt)-_RL; //Get value of RS in a gas
    _ratio = _RS_gas/_R0;  // Get ratio RS_gas/RS_air

    float targetPPM_log = log10(targetPPM); 
    float b = log10(_ratio) - (m*targetPPM_log);
    return b;
}

float Nose::calculateGradient(float b, float targetPPM){
    _readout = (analogRead(_pin1) + analogRead(_pin2)) / 2; //Read analog values of sensors
    _volt = _readout*(5.0/1023); //Convert to voltage
    _RS_gas = ((5.0*_RL)/_volt)-_RL; //Get value of RS in a gas
    _ratio = _RS_gas/_R0;  // Get ratio RS_gas/RS_air

    float targetPPM_log = log10(targetPPM); 
    float m = (log10(_ratio) - b)/targetPPM_log;
    return m;
}

float Nose::gradientDescent(float o, float h, float lr, float c){
    if(c != o){
        c -= (d(c)*lr);
    } else {
        c = c;
    }
}

float Nose::f(float x, float o)
{
    return abs(pow(2, (o - x)));
}

float Nose::d(float x, float h, float o)
{
    return round((f(x + h, o)-f(x, o))/h);
}

float Nose::calibrate()
{
    _readout = (analogRead(_pin1) + analogRead(_pin2)) / 2; //Read analog values of sensors
    _volt = _readout*(5.0/1023); //Convert to voltage
    float RS_air = ((5.0*_RL)/_volt)-_RL; //Get value of RS in a gas
    float R0 = RS_air/_R0;
    return R0;
}

float Thermocouple::readTemps(void)
{
    uint16_t v;

    digitalWrite(_cs, LOW);
    delayMicroseconds(10);

    v = spiRead();
    v <<= 8;
    v |= spiRead();

    digitalWrite(_cs, HIGH);

    if (v & 0x4) {
        return NAN;
    }

    v >>= 3;
    return v * 0.25;
}

byte Thermocouple::spiRead(void)
{
    int i;
    byte d = 0;

    for (i =7; i >= 0; i--){
        digitalWrite(_sclk, LOW);
        delayMicroseconds(10);
        if (digitalRead(_miso)){
            d |= (1 << i);
        }
        digitalWrite(_sclk, HIGH);
        delayMicroseconds(10);
    }
    return d;
}

ZE07H2::ZE07H2()
{
    _s = NULL;
}

ZE07H2::begin(Stream *ser)
{
    _s = ser;
    _s->begin(9600);
}

void ZE07H2::setMode(int m)
{
    const byte Start      = 0xFF;
    const byte Sensor     = 0x01;
    const byte Command    = 0x78;
    byte Mode;
    if (m == 1) {
        Mode       = 0x41;
    } else {
        Mode       = 0x40;
    }
    const byte Zero       = 0x00;
    const byte Checksum   = 0x46;

    byte request[9] = 
    {
        Start,
        Sensor,
        Command,
        Mode,
        Zero,
        Zero,
        Zero,
        Zero,
        Checksum
    };

    _s->write(request, 9);

    _s->flush();
}

void ZE07H2::requestH2()
{
    const byte Start      = 0xFF;
    const byte Sensor     = 0x01;
    const byte Command    = 0x86;
    const byte Zero       = 0x00;
    const byte Checksum   = 0x79;

    byte request[9] = 
    {
        Start,
        Sensor,
        Command,
        Mode,
        Zero,
        Zero,
        Zero,
        Zero,
        Checksum
    };

    _s->write(request, 9);

    _s->flush();
}

float ZE07H2::readH2()
{
    while (_s->available() > 0 && _s->peek() != 0xFF) 
    {
        _s->read();
        delay(1);
    }

    _s->readBytes(response, 9);

    if (response[0] != 0xFF && (response[1] != 0x86 || response[1] != 0x04))
    {
        return -1;
    }

    byte checksum = 0;
    for (int i = 1; i < 8; i++) 
    {
        checksum += response[i];
    }
    checksum = 0xFF - checksum;
    checksum++;

    if (response[8] != checksum) 
    {
        return -2;
    }

    int low  = (int)response[4];
    int high = (int)response[5];
    float ppm = (high * 256 + low) * 0.1;
    return ppm;
}