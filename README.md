# H2NoseLib

A simple library made to help the development of the electronic nose project using MQ type sensors

Quick example on how to use

# Nose class
<h3>Nose nose(int pin1, int pin2, bool isPPB, float b, float m, float rsr0, bool isMG811, String gastype, float rl, bool comm);</h3>


"nose" will be the name of the sensor (or in the case of the H2Nose project, the device name and gas type(ex:MQ2_H2))

pin1 and pin2 will take 2 analog inputs and averages them out

setting isPPB will multiply the output by 1000 to turn it from PPM(parts per million) to PPB(parts per billion)

float b and m is used for the values to count the exponential regression and determine the concentration of any gasses

rsr0 is the ratio between the resistance in clean air and the R0 resistance of the sensor

the MG811 is a different type of sensor and uses a different algorithm which is why it should be set to true if you are using the MG811

gastype will determine the name of the variable for the dictionary in the jetson side of the code

rl is the load resistence of the sensor

comm is just used to add a comma in front of the values so the python code in the jetson can separate it

<h3>nose.getOutput()</h3>

outputs the PPM value

<h3>nose.printOutput()</h3>

self explanatory


# Thermocouple class
<h3>Thermocouple thermocouple(SCLK, CS, DO)</h3>


"thermocouple" name it as you wish then input the SCLK, CS, and DO pins
