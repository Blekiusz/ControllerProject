//Libraries used
#include <Wire.h>
#include <MPU6050.h>

//Variables needed
MPU6050 mpu;
int incomingByte = 0;
int btn = 5;
int irs = 3;
float roll = 0;
float yaw = 0;

void setup() {
  Serial.begin(9600);
  pinMode(irs, INPUT);
  pinMode(btn, INPUT);
  
  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }
}

void loop() {
  if(Serial.available() > 0)
  {
    incomingByte = Serial.read();
    
    if(incomingByte == 'X')
    {
      Vector norm = mpu.readNormalizeGyro();
      roll = roll + norm.XAxis * 0.01;
      Serial.println(roll); // X axis
    }
    
    if(incomingByte == 'Y')
    {
      Vector norm = mpu.readNormalizeGyro();
      yaw = yaw + norm.ZAxis * 0.01;
      Serial.println(yaw); // Y axis
    }
    
    if(incomingByte == 'B')
    {
      if(digitalRead(btn) == true)
      {
        Serial.println(1);
      }
      else
      {
        Serial.println(0);
      }
    }
    if(incomingByte == 'R')
    {
      if(digitalRead(irs) == true)
      {
        Serial.println(1);
      }
      else
      {
        Serial.println(0);
      }
    }
  }
}
