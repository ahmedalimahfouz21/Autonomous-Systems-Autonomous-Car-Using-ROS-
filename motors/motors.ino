const int rev_left = 2;
const int fwd_left = 3;
const int rev_right =  4;
const int fwd_right = 5;
const int en_left = 10;      //pwm
const int en_right = 11;     // pwm
// closed loop

int PiPin = A1;    // select the input pin from the Raspberry Pi
int P = 6 ;  //Gain


int sensorValue = 0;  
int PiValue = 0;      
int outputValue = 0;
int Error = 0;
int ControlInput=0;



int pwm_r = 0;
int pwm_l = 0;
int pot_l =A0;
int pot_r=A1;
int is_Streight=1;
int is_Circle=1;
 #define outputA 12
 #define outputB 13
  int counter = 0; 
 int aState;
 int aLastState; 
 // motors update
 int motors_state_old[7]={0,0,0,0,0,0,0};
int motors_state_current[7]={0,0,0,0,0,0,0};
int flag;
void setup() {
  // put your setup code here, to run once:
pinMode (outputA,INPUT);
pinMode (outputB,INPUT);
pinMode(rev_left, OUTPUT);
pinMode(fwd_left, OUTPUT);
pinMode(en_left, OUTPUT);
pinMode(rev_right, OUTPUT);
pinMode(fwd_right, OUTPUT);
pinMode(en_right, OUTPUT);
   Serial.begin (9600);
   // Reads the initial state of the outputA
   aLastState = digitalRead(outputA); 
}

void loop() {
//straight_line();
//Circle();
sensorValue=encoders();
closed_loop_streight_line(sensorValue);
}
void straight_line(){
  if(is_Streight==1){
    // put your main code here, to run repeatedly:
      forward(255);
      Serial.println("Forward");
delay(3400);
analogWrite(en_right,0);
analogWrite(en_left,0);
    
   is_Streight=0;
}
else {
hold();
}
}

void Circle(){
      rotate_right(100,250);
      Serial.println("Rotate Right");   
}

int encoders(){
  
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
    return counter;
}
 void closed_loop_streight_line(int current_pos){
    // read the value from the sensor:
  sensorValue = current_pos;
    // read the value from the Pi:
  //PiValue = analogRead(PiPin);
  PiValue = 10;
  Error = current_pos - PiValue;
  Serial.print("Error = ");
  Serial.print(Error);

  ControlInput = P * Error;

if (Error>0){
      outputValue=constrain(ControlInput, 0, 255);
      forward(outputValue);
}
else if(Error<0){
      outputValue=constrain(ControlInput, 0, 255);
      backward(outputValue);
}
else{
  hold();
}
  delay(2);
}
 
void forward(int pwm){
  flag= chech_state_change();
  if (flag==1){
   hold();
  }
      analogWrite(en_left,pwm);
      digitalWrite(fwd_left,HIGH);
      digitalWrite(rev_left,LOW);
      analogWrite(en_right,pwm);
      digitalWrite(fwd_right,HIGH);
      digitalWrite(rev_right,LOW);
      Serial.println("Forward");
}

void backward(int pwm){
    flag= chech_state_change();
  if (flag==1){
   hold();
  }
      analogWrite(en_left,pwm);
      digitalWrite(fwd_left,LOW);
      digitalWrite(rev_left,HIGH);
      analogWrite(en_right,pwm);
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

void rotate_left_CD(int pwm){
     flag= chech_state_change();
  if (flag==1){
   hold();
  }
      analogWrite(en_left,pwm);
      digitalWrite(fwd_left,LOW);
      digitalWrite(rev_left,HIGH);
      analogWrite(en_right,pwm);
      digitalWrite(fwd_right,HIGH);
      digitalWrite(rev_right,LOW);
      Serial.println("rotate CCW");
}

void rotate_right_CD(int pwm){
     flag= chech_state_change();
  if (flag==1){
   hold();
  }
      analogWrite(en_left,pwm);
      digitalWrite(fwd_left,HIGH);
      digitalWrite(rev_left,LOW);
      analogWrite(en_right,pwm);
      digitalWrite(fwd_right,LOW);
      digitalWrite(rev_right,HIGH); 
      Serial.println("rotate CW");
}
void rotate_left(int pwm_l, int pwm_r){
       flag= chech_state_change();
  if (flag==1){
   hold();
  }
   analogWrite(en_right,pwm_l);
      digitalWrite(fwd_right,HIGH);
      digitalWrite(rev_right,LOW);
      analogWrite(en_left,pwm_r);
      digitalWrite(fwd_left,LOW);
      digitalWrite(rev_left,HIGH);
      Serial.println("rotate left");
}
void rotate_right(int pwm_l, int pwm_r){
       flag= chech_state_change();
  if (flag==1){
   hold();
  }
      analogWrite(en_left,pwm_l);
      digitalWrite(fwd_left,HIGH);
      digitalWrite(rev_left,LOW);
      analogWrite(en_right,pwm_r);
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
