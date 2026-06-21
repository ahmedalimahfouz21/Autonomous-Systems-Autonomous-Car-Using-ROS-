
int sensorPin = A0;    // select the input pin for the Encoder

int PiPin = A1;    // select the input pin from the Raspberry Pi

int PWM = A2;            // PWM 

int P = 6   //Gain


int sensorValue = 0;  
int PiValue = 0;      
int outputValue = 0;
int Error = 0;
int ControlInput=0;

void setup() {

  Serial.begin(9600);

}

void loop() {
    // read the value from the sensor:
  sensorValue = analogRead(sensorPin);
    // read the value from the Pi:
  PiValue = analogRead(PiPin);

  Error = sensorValue - PiValue;
  Serial.print("Error = ");
  Serial.print(Error);

  ControlInput = P * Error;

  outputValue = map(ControlInput, 0, 1023, 0, 255);

  analogWrite(PWM, outputValue);

  delay(2);



}
