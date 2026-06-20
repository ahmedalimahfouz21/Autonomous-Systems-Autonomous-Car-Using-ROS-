#include <Wire.h>
//#include <TimedAction.h>

// motor vars

const int rev_left = 16;
const int fwd_left = 3;
const int rev_right =  4;
const int fwd_right = 5;
const int en_left = 10;      //pwm
const int en_right = 11;     // pwm

  int robot_vel_r = 0; //actuating signal
  int robot_vel_l = 0; //actuating signal
  int pwm_r = 0;
  int pwm_l = 0;

// imu vars

const int MPU = 0x68; // MPU6050 I2C address
float AccX, AccY, AccZ;
float GyroX, GyroY, GyroZ;
float accAngleX, accAngleY, gyroAngleX, gyroAngleY, gyroAngleZ;
float roll, pitch, yaw;
float AccErrorX, AccErrorY, GyroErrorX, GyroErrorY, GyroErrorZ;
float elapsedTime, currentTime, previousTime;
int c = 0;

// ultrasonic vars

const int trigPin_f = 6;
const int echoPin_f = 7;

const int trigPin_l = 8;
const int echoPin_l = 9;

const int trigPin_r = 12;
const int echoPin_r = 13;

//const int trigPin_l = 14;
//const int echoPin_l = 15;

// defines variables
long duration_f;
int distance_f;

long duration_b;
int distance_b;

long duration_r;
int distance_r;

long duration_l;
int distance_l;

//encoder vars

/*int sensor_r = 18;
unsigned long start_time_r = 0;
unsigned long end_time_r = 0;
int steps_r=0;
float steps_old_r=0;
float temp_r=0;
float rps_r=0;
float vel_r;

int sensor_l = 19;
unsigned long start_time_l = 0;
unsigned long end_time_l = 0;
int steps_l=0;
float steps_old_l=0;
float temp_l=0;
float rps_l=0;
float vel_l;
*/
 #define outputA 14
 #define outputB 15
 int counter = 0; 
 int aState;
 int aLastState;  
// Speed control
int pot_l =A0;
int pot_r=A1;
int motors_state_old[7]={0,0,0,0,0,0,0};
int motors_state_current[7]={0,0,0,0,0,0,0};
int flag;

void calculate_IMU_error(){
  // We can call this funtion in the setup section to calculate the accelerometer and gyro data error. From here we will get the error values used in the above equations printed on the Serial Monitor.
  // Note that we should place the IMU flat in order to get the proper values, so that we then can the correct values
  // Read accelerometer values 200 times
  while (c < 200) {
    Wire.beginTransmission(MPU);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 6, true);
    AccX = (Wire.read() << 8 | Wire.read()) / 16384.0 ;
    AccY = (Wire.read() << 8 | Wire.read()) / 16384.0 ;
    AccZ = (Wire.read() << 8 | Wire.read()) / 16384.0 ;
    // Sum all readings
    AccErrorX = AccErrorX + ((atan((AccY) / sqrt(pow((AccX), 2) + pow((AccZ), 2))) * 180 / PI));
    AccErrorY = AccErrorY + ((atan(-1 * (AccX) / sqrt(pow((AccY), 2) + pow((AccZ), 2))) * 180 / PI));
    c++;
  }
  //Divide the sum by 200 to get the error value
  AccErrorX = AccErrorX / 200;
  AccErrorY = AccErrorY / 200;
  c = 0;
  // Read gyro values 200 times
  while (c < 200) {
    Wire.beginTransmission(MPU);
    Wire.write(0x43);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 6, true);
    GyroX = Wire.read() << 8 | Wire.read();
    GyroY = Wire.read() << 8 | Wire.read();
    GyroZ = Wire.read() << 8 | Wire.read();
    // Sum all readings
    GyroErrorX = GyroErrorX + (GyroX / 131.0);
    GyroErrorY = GyroErrorY + (GyroY / 131.0);
    GyroErrorZ = GyroErrorZ + (GyroZ / 131.0);
    c++;
  }
  //Divide the sum by 200 to get the error value
  GyroErrorX = GyroErrorX / 200;
  GyroErrorY = GyroErrorY / 200;
  GyroErrorZ = GyroErrorZ / 200;

}
void setup() {

//sets pins of the 4 motors


pinMode(rev_left, OUTPUT);
pinMode(fwd_left, OUTPUT);
pinMode(en_left, OUTPUT);
pinMode(rev_right, OUTPUT);
pinMode(fwd_right, OUTPUT);
pinMode(en_right, OUTPUT);
//speed Control
pinMode(pot_l,INPUT);
pinMode(pot_r,INPUT);

////////////// imu config

Wire.begin();                      // Initialize comunication
Wire.beginTransmission(MPU);       // Start communication with MPU6050 // MPU=0x68
Wire.write(0x6B);                  // Talk to the register 6B
Wire.write(0x00);                  // Make reset - place a 0 into the 6B register
Wire.endTransmission(true);        //end the transmission
  
  // Configure Accelerometer Sensitivity - Full Scale Range (default +/- 2g)
Wire.beginTransmission(MPU);
Wire.write(0x1C);                  //Talk to the ACCEL_CONFIG register (1C hex)
Wire.write(0x10);                  //Set the register bits as 00010000 (+/- 8g full scale range)
Wire.endTransmission(true);
  // Configure Gyro Sensitivity - Full Scale Range (default +/- 250deg/s)
Wire.beginTransmission(MPU);
Wire.write(0x1B);                   // Talk to the GYRO_CONFIG register (1B hex)
Wire.write(0x10);                   // Set the register bits as 00010000 (1000deg/s full scale)
Wire.endTransmission(true);
delay(20);
calculate_IMU_error();
delay(20);

////////////// Ultrasonic sensors config

pinMode(trigPin_f, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin_f, INPUT); // Sets the echoPin as an Input
//pinMode(trigPin_b, OUTPUT); // Sets the trigPin as an Output
//pinMode(echoPin_b, INPUT); // Sets the echoPin as an Input
pinMode(trigPin_r, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin_r, INPUT); // Sets the echoPin as an Input
pinMode(trigPin_l, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin_l, INPUT); // Sets the echoPin as an Input

////////////// encoders config

 // pinMode (outputA,INPUT);
 // pinMode (outputB,INPUT);
   
   Serial.begin (9600);
   // Reads the initial state of the outputA
   aLastState = digitalRead(outputA);   
 } 
 


void loop() {
  sensors() ;
  motors();
}


void motors(){
   robot_vel_r = analogRead(pot_r); //actuating signal
   robot_vel_l = analogRead(pot_l); //actuating signal
   pwm_r = map(robot_vel_r,0,1023,0,255);
   pwm_l = map(robot_vel_l,0,1023,0,255);
//if( distance_f >9 and distance_b >9 and  distance_l >9 and  distance_r >9){
if( distance_f >9 and  distance_l >9 and  distance_r >9){
if(620<robot_vel_r && robot_vel_r<1023 && 620<robot_vel_l && robot_vel_l<1023){
    /// forward
     reset_states();
     motors_state_current[0]=1;
    forward();
  }
  else if(400<robot_vel_r && robot_vel_r<620 && 400<robot_vel_l && robot_vel_l<620 ){
    /// hold
         reset_states();
     motors_state_current[1]=1;
   hold();
  }
  else if(0<robot_vel_r && robot_vel_r<400 && 0<robot_vel_l && robot_vel_l<400 ){  
    /// backward 
    reset_states();
     motors_state_current[2]=1;
    backward();
  }
  else if(620<robot_vel_r && robot_vel_r<1023  &&  0<robot_vel_l && robot_vel_l<400 ){
    /// rotate around CD left (CCW)
    reset_states();
     motors_state_current[3]=1;
    rotate_left_CD();
  }
  else if (620<robot_vel_l && robot_vel_l<1023  &&  0<robot_vel_r && robot_vel_r<400 ){
    /// rotate around CD right (CW)
   reset_states();
     motors_state_current[4]=1;
   rotate_right_CD();
  }
  else if (620<robot_vel_r && robot_vel_r<1023  &&  400<robot_vel_l && robot_vel_l<620){
    /// rotate left 
    reset_states();
     motors_state_current[5]=1;
   rotate_left();
  }
  else if(620<robot_vel_l && robot_vel_l<1023  &&  400<robot_vel_r && robot_vel_r<620){
    /// rotate right
   reset_states();
     motors_state_current[6]=1;
rotate_right();  
      
  }
}
else if(distance_f <9){
    /// backward 
    reset_states();
     motors_state_current[2]=1;
    backward();
}
//else if(distance_b <9){
    /* forward
     reset_states();
     motors_state_current[0]=1;
forward();
}*/
else if(distance_l <9){
    /// rotate right
    reset_states();
     motors_state_current[6]=1;
    rotate_right(); 
}
else if(distance_r <9){
    /// rotate left
    reset_states();
     motors_state_current[5]=1;
   rotate_left();
}
for (int i=0;i<7;i++)
motors_state_old[i]= motors_state_current[i];
}


void sensors(){
  imu();
 // ultrasonic();
//  encoders();
}

void imu(){
  //Serial.print("here");
  // === Read acceleromter data === //
  Wire.beginTransmission(MPU);
  Wire.write(0x3B); // Start with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
  //For a range of +-2g, we need to divide the raw values by 16384, according to the datasheet
  AccX = (Wire.read() << 8 | Wire.read()) / 16384.0; // X-axis value
  AccY = (Wire.read() << 8 | Wire.read()) / 16384.0; // Y-axis value
  AccZ = (Wire.read() << 8 | Wire.read()) / 16384.0; // Z-axis value
  Serial.print(AccX*10);
  Serial.print("m/s^2");
  Serial.println("/");
  Serial.print(AccY*10);
  Serial.print("m/s^2");
  Serial.println("/");
  Serial.print(AccZ*10);
  Serial.println("m/s^2");
  // Calculating Roll and Pitch from the accelerometer data
  accAngleX = (atan(AccY / sqrt(pow(AccX, 2) + pow(AccZ, 2))) * 180 / PI) - AccErrorX ; 
  accAngleY = (atan(-1 * AccX / sqrt(pow(AccY, 2) + pow(AccZ, 2))) * 180 / PI) - AccErrorY ;

  // === Read gyroscope data === //
  previousTime = currentTime;        // Previous time is stored before the actual time read
  currentTime = millis();            // Current time actual time read
  elapsedTime = (currentTime - previousTime) / 1000; // Divide by 1000 to get seconds
  Wire.beginTransmission(MPU);
  Wire.write(0x43); // Gyro data first register address 0x43
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); // Read 4 registers total, each axis value is stored in 2 registers
  GyroX = (Wire.read() << 8 | Wire.read()) / 131.0; // For a 250deg/s range we have to divide first the raw value by 131.0, according to the datasheet
  GyroY = (Wire.read() << 8 | Wire.read()) / 131.0;
  GyroZ = (Wire.read() << 8 | Wire.read()) / 131.0;
  // Correct the outputs with the calculated error values
  GyroX = GyroX - GyroErrorX ;
  GyroY = GyroY - GyroErrorY ;
  GyroZ = GyroZ - GyroErrorZ ;

  // Currently the raw values are in degrees per seconds, deg/s, so we need to multiply by sendonds (s) to get the angle in degrees
  gyroAngleX = gyroAngleX + GyroX * elapsedTime; // deg/s * s = deg
  gyroAngleY = gyroAngleY + GyroY * elapsedTime;
  yaw =  yaw + GyroZ * elapsedTime;

  // Complementary filter - combine acceleromter and gyro angle values
  roll = 0.96 * gyroAngleX + 0.04 * accAngleX;
  pitch = 0.96 * gyroAngleY + 0.04 * accAngleY;
  
  // Print the values on the serial monitor
  Serial.print(roll);
  Serial.print("/");
  Serial.print(pitch);
  Serial.print("/");
  Serial.println(yaw);
  
 // delay(1000);
}
void ultrasonic(){
  // Clears the trigPin
digitalWrite(trigPin_f, LOW);
delayMicroseconds(2);
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin_f, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin_f, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
duration_f = pulseIn(echoPin_f, HIGH);
// Calculating the distance
distance_f= duration_f*0.034/2;
// Prints the distance on the Serial Monitor
Serial.print("Distance_f: ");
Serial.println(distance_f);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//digitalWrite(trigPin_b, LOW);
//delayMicroseconds(2);
// Sets the trigPin on HIGH state for 10 micro seconds
//digitalWrite(trigPin_b, HIGH);
//delayMicroseconds(10);
//digitalWrite(trigPin_b, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
//duration_b = pulseIn(echoPin_b, HIGH);
// Calculating the distance
//distance_b= duration_b*0.034/2;
//Serial.print("Distance_b: ");
//Serial.println(distance_b);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
digitalWrite(trigPin_r, LOW);
delayMicroseconds(2);
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin_r, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin_r, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
duration_r = pulseIn(echoPin_r, HIGH);
// Calculating the distance
distance_r= duration_r*0.034/2;
Serial.print("Distance_r: ");
Serial.println(distance_r);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
digitalWrite(trigPin_l, LOW);
delayMicroseconds(2);
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin_l, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin_l, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
duration_l = pulseIn(echoPin_l, HIGH);
// Calculating the distance
distance_l= duration_l*0.034/2;
Serial.print("Distance_l: ");
Serial.println(distance_l);

}
void encoders(){
  
 aState = digitalRead(outputA); // Reads the "current" state of the outputA
   // If the previous and the current state of the outputA are different, that means a Pulse has occured
   if (aState != aLastState){     
     // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
     if (digitalRead(outputB) != aState) { 
       counter ++;
     } else {
       counter --;
     }
     Serial.print("Position: ");
     Serial.println(counter);
   } 
   aLastState = aState; // Updates the previous state of the outputA with the current state
    

/////////////////////////////////////////////////////////////////////////////////////////

// start_time_l=millis();
// end_time_l=start_time_l+1000;
// while(millis()<end_time_l)
// {
//   if(digitalRead(sensor_l))
//   {
//    steps_l=steps_l+1; 
//    while(digitalRead(sensor_l));
//   }
// }
//    temp_l=steps_l-steps_old_l;
//    steps_old_l=steps_l;
//    rps_l=(temp_l/20);
//    vel_l = rps_l*0.065*22/7;
//    Serial.print("vel_left: ");
//    Serial.print(vel_l);
//    Serial.println(" m/s");
}



/*void calculate_IMU_error(){
  // We can call this funtion in the setup section to calculate the accelerometer and gyro data error. From here we will get the error values used in the above equations printed on the Serial Monitor.
  // Note that we should place the IMU flat in order to get the proper values, so that we then can the correct values
  // Read accelerometer values 200 times
  while (c < 200) {
    Wire.beginTransmission(MPU);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 6, true);
    AccX = (Wire.read() << 8 | Wire.read()) / 16384.0 ;
    AccY = (Wire.read() << 8 | Wire.read()) / 16384.0 ;
    AccZ = (Wire.read() << 8 | Wire.read()) / 16384.0 ;
    // Sum all readings
    AccErrorX = AccErrorX + ((atan((AccY) / sqrt(pow((AccX), 2) + pow((AccZ), 2))) * 180 / PI));
    AccErrorY = AccErrorY + ((atan(-1 * (AccX) / sqrt(pow((AccY), 2) + pow((AccZ), 2))) * 180 / PI));
    c++;
  }
  //Divide the sum by 200 to get the error value
  AccErrorX = AccErrorX / 200;
  AccErrorY = AccErrorY / 200;
  c = 0;
  // Read gyro values 200 times
  while (c < 200) {
    Wire.beginTransmission(MPU);
    Wire.write(0x43);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 6, true);
    GyroX = Wire.read() << 8 | Wire.read();
    GyroY = Wire.read() << 8 | Wire.read();
    GyroZ = Wire.read() << 8 | Wire.read();
    // Sum all readings
    GyroErrorX = GyroErrorX + (GyroX / 131.0);
    GyroErrorY = GyroErrorY + (GyroY / 131.0);
    GyroErrorZ = GyroErrorZ + (GyroZ / 131.0);
    c++;
  }
  //Divide the sum by 200 to get the error value
  GyroErrorX = GyroErrorX / 200;
  GyroErrorY = GyroErrorY / 200;
  GyroErrorZ = GyroErrorZ / 200;

}*/
void forward(){
  flag= chech_state_change();
  if (flag==1){
   hold();
  }
      analogWrite(en_left,pwm_l);
      digitalWrite(fwd_left,HIGH);
      digitalWrite(rev_left,LOW);
      analogWrite(en_right,pwm_r);
      digitalWrite(fwd_right,HIGH);
      digitalWrite(rev_right,LOW);
      Serial.println("Forward");
}

void backward(){
    flag= chech_state_change();
  if (flag==1){
   hold();
  }
      analogWrite(en_left,400-pwm_l);
      digitalWrite(fwd_left,LOW);
      digitalWrite(rev_left,HIGH);
      analogWrite(en_right,400-pwm_r);
      digitalWrite(fwd_right,LOW);
      digitalWrite(rev_right,HIGH);
      Serial.println("BACKWARD");
  }

void hold(){
      analogWrite(en_left,0);
      digitalWrite(fwd_left,LOW);
      digitalWrite(rev_left,HIGH);
      analogWrite(en_right,0);
      digitalWrite(fwd_right,LOW);
      digitalWrite(rev_right,HIGH);
      Serial.println("HOLD");
      delay(50);
}

void rotate_left_CD(){
     flag= chech_state_change();
  if (flag==1){
   hold();
  }
      analogWrite(en_left,pwm_r);
      digitalWrite(fwd_left,LOW);
      digitalWrite(rev_left,HIGH);
      analogWrite(en_right,pwm_r);
      digitalWrite(fwd_right,HIGH);
      digitalWrite(rev_right,LOW);
      Serial.println("rotate CCW");
}

void rotate_right_CD(){
     flag= chech_state_change();
  if (flag==1){
   hold();
  }
      analogWrite(en_left,pwm_l);
      digitalWrite(fwd_left,HIGH);
      digitalWrite(rev_left,LOW);
      analogWrite(en_right,pwm_l);
      digitalWrite(fwd_right,LOW);
      digitalWrite(rev_right,HIGH); 
      Serial.println("rotate CW");
}
void rotate_left(){
       flag= chech_state_change();
  if (flag==1){
   hold();
  }
   analogWrite(en_right,pwm_r);
      digitalWrite(fwd_right,HIGH);
      digitalWrite(rev_right,LOW);
      analogWrite(en_left,0);
      digitalWrite(fwd_left,LOW);
      digitalWrite(rev_left,HIGH);
      Serial.println("rotate left");
}
void rotate_right(){
       flag= chech_state_change();
  if (flag==1){
   hold();
  }
      analogWrite(en_left,pwm_l);
      digitalWrite(fwd_left,HIGH);
      digitalWrite(rev_left,LOW);
      analogWrite(en_right,0);
      digitalWrite(fwd_right,LOW);
      digitalWrite(rev_right,HIGH);
      Serial.println("rotate right");
}
int chech_state_change(){
  int Flag=0;
   for (int i=0; i<7; i++){
   if (motors_state_current[i]!=motors_state_old[i])
   Flag=1;
   return Flag;
  }
  return Flag;
}
int reset_states(){
  for (int i=0; i<7; i++){
   motors_state_current[i]=0;
  } 
  }
