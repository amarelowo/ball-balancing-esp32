#include <Arduino.h>
#include <Servo.h>
#include <PID.h>


#define pinServoY 26
#define pinServoX 14
#define pinPotA 35
#define pinPotB 34

const int period = 1;
unsigned long time;

float kp = 1.0;
float ki = 0.0;
float kd = 0.0;
int setPoint = 250;

float rangeMap = kp*setPoint;

Servo sX;
Servo sY;

PID pidX(kp,ki,kd,setPoint, period);
PID pidY(kp,ki,kd,setPoint, period);

//String data = String(5);
String data;


void setup(){
  Serial.begin(115200);
  Serial.setTimeout(10);

  sX.attach(pinServoX);
  sX.write(141);
  sY.attach(pinServoY);
  sY.write(148);

  // pinMode(pinPotA, INPUT);
  // pinMode(pinPotB, INPUT);


  time = millis();
}


bool verificaDado(String data){
  bool verf;
  String verificador = data.substring(0,1);
  if (verificador == "c"){
    verf = true;
  }
  else if(verificador == "f"){
    verf = false;}
  return verf;
}

void loop(){
  
  while(Serial.available()){
    
    data = Serial.readString();
    data.trim();
    
    if (verificaDado(data) == false){
      int dataX = data.substring(2,7).toInt();
      int dataY = data.substring(8,13).toInt();

      // sX.write(dataX);
      // sY.write(dataY);
      
      // Serial.print("data: ");
      // Serial.print(data);
      // Serial.print(" | posX: ");
      // Serial.print(dataX);
      // Serial.print(" | posY: ");
      // Serial.println(dataY);



      // s.write(dataForm);
      if (millis() > time+period){
      
        

        double posServoX = pidX.process(dataX);
        double posServoY = pidY.process(dataY);

        
        int maxX = 161;  //15 graus 161
        int minX = 121;  //-15 graus 121
        int maxY = 168;  //15 graus 168
        int minY = 128;  // -15 graus 128

        posServoX = map(posServoX, -setPoint,setPoint,minX, maxX);
        posServoY = map(posServoY, -setPoint,setPoint,minY, maxY);

        if(posServoX > maxX){posServoX = maxX;}
        if(posServoX < minX){posServoX = minX;}
        if(posServoY > maxY){posServoY = maxY;}
        if(posServoY < minY){posServoY = minY;}

        // Serial.println(pidX.pidString() +" | pos: " + (String)posServo);
        Serial.print("posX: ");
        Serial.print(posServoX);
        Serial.print(" | posY: ");
        Serial.println(posServoY);

        sY.write(posServoY);
        sX.write(posServoX);
        time = millis();
      } 
    }
    else{
      kp = data.substring(2,7).toFloat();
      ki = data.substring(8,13).toFloat();
      kd = data.substring(14,18).toFloat();
      pidX.atualizarPID(kp,ki,kd);
      Serial.println("kp: "+(String)kp+" | ki: "+(String)ki+" | kd: "+(String)kd);
    }
  }
  // int leituraPotA = analogRead(pinPotA);
  // int leituraPotB = analogRead(pinPotB);
  // float posServoA = map(leituraPotA, 0,4096,0,180);
  // float posServoB = map(leituraPotB,0,4096,0,180);
  // if(posServoA < 78){
  //   posServoA = 78;
  // }
  // if(posServoB < 98){
  //   posServoB = 98;
  // }
  
}