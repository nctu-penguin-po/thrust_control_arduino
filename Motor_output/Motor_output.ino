#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#define NUM_MOTOR 1
// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
 
// our servo # counter
//uint8_t servonum = 0;

String data_from_computer;
void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  Serial.println("16 channel Servo test!");
  Serial2.println(2);
  pwm.begin();
  
  pwm.setPWMFreq(52);  // Analog servos run at ~50 Hz updates模擬伺服執行在50赫茲更新

   delay(10);
  for(int i=0;i<8;i++){
      pwm.setPWM(i,0,1500.0/20000.0*4096);
  }
}
double Motor_pwm[8]={1500};
char data;
int safe_counter=0;
void loop() {
  while(1){
    if(Serial.available()){
      data=Serial.read();
      if(data==100){                           //ascii碼100則歸零
        for(int i=0;i<8;i++)
          Motor_pwm[i]=0;
        safe_counter=0;
        //Serial2.println("restart");
      }
      else if(data==101){
      //Serial2.println("101!");
        if(safe_counter==16){      //ascii碼101則跳出 並確定8個馬達有輸出值
          //Serial2.println("out!");
          Serial.print(2);
          break;
        }
      }
      else{                                    //其他則改動馬達值
        Motor_pwm[safe_counter/2]=Motor_pwm[safe_counter/2]*100+(int)data;
        //Serial2.println(safe_counter);
        safe_counter++;
        
      }
      //delay(10);
    }
    
    //Serial2.println("hehe");
  }

  
  
  for(int i=0;i<8;i++){
    Serial2.print(i);
    Serial2.print(":");
    if(Motor_pwm[i]>1000){
      pwm.setPWM(i,0,(double)Motor_pwm[i]/20000.0*4096);
    }
    else
      pwm.setPWM(i,0,1500.0/20000.0*4096);
    Serial2.print(Motor_pwm[i]);
  }
  Serial2.print("\n");
}
