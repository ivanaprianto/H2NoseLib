/*
    library for the h2nose project
*/
#include "Arduino.h"
#include "Nose.h"

Nose::Nose(int pins[], bool isPPB, float b, float m, float ratioInCleanAir, bool isMG811, String gasType, float rl, bool comm)
{
    _pins[] = pins[]; //Sensor pins
    _isPPB = isPPB; //for MQ3
    _b = b; //V400 in MG811
    _m = m; //V4000 in MG811
    _R0 = ratioInCleanAir; //Clean air to R0 ratio
    _isMG811 = isMG811; //for MG811
    _gasType = gasType; //H2, CH4, C3H8, etc.
    _RL = rl; //load resistance
    _com = comm; //separate print with comma
    _sizearr = sizeof(_pins) / sizeof(int); 
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
    _readout = (analogRead(_pin1) + analogRead(_pin2)) / 2;
    _volt = (_readout * (5.0/1023.0));
    return _volt;
}

float Nose::getOutput()
{
    _volt = getVoltage();
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

float Nose::getOutput()
{
    _volts = getVoltage();
    for(int i = 0; i<_sizearr; i++)
    {
        if(_isMG811)
        {
            _buffer = 0;
            _buffer = (_b - _m)/(log10(400) - log10(40000)); // Delta V
            _buffer = (_volt - _b)/_buffer; 
            _buffer += log10(400);
            _buffer_final = pow(10, _buffer);
            return _buffer_final; //return in ppm
        } else {
            _RS_gas = ((5.0*_RL)/_volts[i])-_RL; //Get value of RS in a gas
            _ratio = _RS_gas/_R0;  // Get ratio RS_gas/RS_air
            _ppm_log = (log10(_ratio)-_b)/_m; //Get ppm value in linear scale according to the the ratio value  
            _ppm[i] = pow(10, _ppm_log); //Convert ppm value to log scale 
            if (_isPPB){
                _ppb = _ppm[i]*1000;
            }
            float ppmavg = 0.0;
            for(int i = 0; i < _sizearr; i++)
            {
                ppmavg += _ppm[i];
            }
            return ppmavg;
        }
    }
}

void Nose::setPPM(float x, int i)
{
    _ppm[i] = x;
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

void Nose::printOutputInd()
{
    for()
}

float Nose::calculateRLoffset(float targetRL, float targetPPM, int i) //does nothing, will make it useful later
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

float Nose::calculateCurrentRL(float targetPPM, int i){
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

float Nose::calculateIntersect(float m, float targetPPM, int i){
    _readout = (analogRead(_pin1) + analogRead(_pin2)) / 2; //Read analog values of sensors
    _volt = _readout*(5.0/1023); //Convert to voltage
    _RS_gas = ((5.0*_RL)/_volt)-_RL; //Get value of RS in a gas
    _ratio = _RS_gas/_R0;  // Get ratio RS_gas/RS_air

    float targetPPM_log = log10(targetPPM); 
    float b = log10(_ratio) - (m*targetPPM_log);
    return b;
}

float Nose::calculateGradient(float b, float targetPPM, int i){
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
        c -= (d(c, h, o)*lr);
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

ZE07H2::ZE07H2(HardwareSerial *Serial)	//read the uart signal by hardware uart,such as D0
{
    mySerial = Serial;
    receivedFlag = 0;
}

// ZE07H2::ZE07H2(SoftwareSerial *Serial)	//read the uart signal by software uart,such as D10
// {
//     mySerial = Serial;
//     receivedFlag = 0;
// }

ZE07H2:: ZE07H2(int pin,float ref)//read the analog signal by analog input pin ,such as A2; ref:voltage on AREF pin
{
    _sensorPin = pin;
    _ref = ref;											//for arduino uno ,the ref should be 5.0V(Typical)
}

byte ZE07H2::checkSum(byte array[],byte length)	
{
    byte sum = 0;
    for(int i = 1; i < length-1; i ++)
    {
		sum += array[i];
    }
    sum = (~sum) + 1;
    return sum;
}

void ZE07H2::boucle()
{
  _status = STATUS_WAITING;
  if (mySerial->available()){
    uint8_t ch = mySerial->read();
    switch (_index){
    case 0:
      if (ch != 0xFF){
        return;
      }
      receivedCommandStack[_index]=ch;
      break;
    case 1:
      if (ch != 0x04){
        _index = 0;
        return;
      }
      receivedCommandStack[_index]=ch;
      break;
    case 2:
     if (ch != 0x03){
        _index = 0;
        return;
      }
      receivedCommandStack[_index]=ch;
      break;
    default:
      if (_index==8){
        _status = STATUS_OK;
      
      }
      receivedCommandStack[_index]=ch;
      break;
    }
    _index++;
  }
  
}

boolean ZE07H2::available(uint16_t timeout)		//new data was recevied
{
    if (timeout > 0){
		uint32_t start = millis();
		do{
			boucle();
			if (_status == STATUS_OK) break;
		} while (millis() - start < timeout);
	} else {
    boucle();
	}
	if ((receivedCommandStack[MAXLENGTH-1]==checkSum(receivedCommandStack,MAXLENGTH)) && (_status = STATUS_OK)){
		receivedFlag = 1;			//new data received
		return receivedFlag;
	} else {
		receivedFlag = 0;			//data loss or error
		return receivedFlag;
	}
}

float ZE07H2::uartReadPPM()
{
    receivedFlag = 0;
    float ppm = (unsigned int)receivedCommandStack[4]<<8 | receivedCommandStack[5];		// bit 4: ppm high 8-bit; bit 5: ppm low 8-bit
    ppm = ppm / 10.0;
    return ppm;
}
 
float ZE07H2::dacReadPPM()
{
   float analogVoltage = analogRead(_sensorPin) / 1024.0 * _ref;
   float ppm = (3.125 * analogVoltage - 1.25) * 100;	//linear relationship(0.4V for 0 ppm and 2V for 500ppm)
   if(ppm<0)
	   ppm=0;
   else if(ppm>500)
	   ppm = 500;
   return ppm;
}