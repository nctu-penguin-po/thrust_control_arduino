void setup() {
  // put your setup code here, to run once:
Serial2.begin(115200);
Serial.begin(115200);
Serial.println("haha");
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial2.available()){
    Serial.print((char)Serial2.read());
  }
}
