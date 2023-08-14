//define SCLK, CS, and DO pins
int DO = 4;
int CS = 5;
int SCLK = 6;

//initialize class
Thermocouple thermocouple(SCLK, CS, DO);

//setup
void setup() {
    Serial.begin(9600);
    delay(500); //wait for thermocouple to stabilize
}

void loop() {
    Serial.print("Temperature (Celsius) : ");
    Serial.print(thermocouple.readTemps());
    delay(1000); //at least 250ms delay between reads
}