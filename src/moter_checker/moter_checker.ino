#include <Servo.h>
#define START_POSITION 700
#define RESOLUTION 5

Servo servo;

void setup() {
  Serial.begin(250000);
  Serial.setTimeout(10);
  servo.attach(3);
}

int set_position(int default_position){
  while(Serial.available()==0);
  int target_position = Serial.parseInt();
  while(Serial.read()!=-1);
  if(target_position==0){
    target_position=default_position;
  }
  Serial.print(target_position);
  Serial.println(" [ms]");
  return target_position;
}

void loop() {
  // 実験1
  for(int target_position=START_POSITION; target_position<20000; target_position+=RESOLUTION){
    target_position = set_position(target_position);
    servo.writeMicroseconds(target_position);
  }

  // 実験2
  for(int target_position=START_POSITION; target_position<20000; target_position+=RESOLUTION){
    target_position = set_position(target_position);
    servo.writeMicroseconds(START_POSITION);
    delay(1000);
    servo.writeMicroseconds(target_position);
  }
}
