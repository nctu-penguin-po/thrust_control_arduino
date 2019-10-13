#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#define NUM_MOTOR 1
// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
 
// our servo # counter
//uint8_t servonum = 0;

String data_from_computer;
void setup() {
  Serial.begin(9600);
  Serial.println("16 channel Servo test!");
 
  pwm.begin();
  
  pwm.setPWMFreq(52);  // Analog servos run at ~50 Hz updates模擬伺服執行在50赫茲更新

   delay(10);
}
double Motor_pwm=1500;
char data;
void loop() {
  
  if(Serial.available()){
    data=Serial.read();
    if((int)data==100){
      Motor_pwm = 0;
      for(int i=0;i<2;i++){
        Motor_pwm=Motor_pwm*100+(int)Serial.read();
      }
    }
    else{
      Motor_pwm = 1500;
      for(int i=0;i<2;i++){
        Serial.read();
      }
    }
    Serial.println(Motor_pwm);
  }
  if(Motor_pwm>1000){
    pwm.setPWM(0,0,Motor_pwm/20000.0*4096);
    delay(10);
  }
}
