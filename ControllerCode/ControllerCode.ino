//Libraries used
#include <Wire.h>
#include <MPU6050.h>

//Variables needed
MPU6050 mpu;
int incomingByte = 0;
int btn = 5;
int irs = 3;

void setup() {
  Serial.begin(9600);
  pinMode(irs, INPUT);
  pinMode(btn, INPUT);
}

void loop() {
  if(Serial.available() > 0)
  {
    incomingByte = Serial.read();
    
    if(incomingByte == 'P'){ //ValueForPlayer'sMovement
      Vector normAccel = mpu.readNormalizeAccel();
      int Xaxis = normAccel.ZAxis * 10;
      int Yaxis = normAccel.XAxis * 10 - 98;
      
      Serial.print(getPadded(Xaxis));
      Serial.print("-");
      Serial.println(getPadded(Yaxis));
    }
    
    if(incomingByte == 'B'){ //ValueForTrigger
      if(digitalRead(btn) == true){
        Serial.print(getPadded(1));
      }
      else{
        Serial.print(getPadded(0));
      }
    }
    if(incomingByte == 'R'){ //ValueForReloading
      if(digitalRead(irs) == true){
        Serial.print(getPadded(1));
      }
      else{
        Serial.print(getPadded(0));
      }
    }
  }
}

String getPadded(int num)
{
  char buff[5];
  char padded[6];
  sprintf(buff, "%.4u", num); // convert to 4 character string and save it to buff
  for(int i = 0; i < 5; i++) padded[i] = buff[i];
  padded[5] = '\0';
  return String(padded);
}
