#include <Arduino.h>
#include <Servo.h>
#include <PID.h>


#define pinServo 33

const int period = 50;
unsigned long time;

float kp = 1;
float ki = 0.5;
float kd = 800;
int setPoint = 500;

float rangeMap = kp*setPoint;

Servo s;
PID pid(kp,ki,kd,setPoint, period);

//String data = String(5);
String data;


void setup(){
  Serial.begin(115200);
  Serial.setTimeout(10);

  s.attach(pinServo);
  s.write(120);

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
      int dataForm = data.substring(2,7).toInt();

      // s.write(dataForm);
      if (millis() > time+period){
      
        time = millis();

        double posServo = pid.process(dataForm);
      

        posServo = map(posServo, setPoint,-setPoint,0, 150);


        if(posServo < 40){posServo = 40;}
        if(posServo > 155){posServo = 155;}

        Serial.println(pid.pidString() +" | pos: " + (String)posServo);

        s.write(posServo+30);
      } 
    }
    else{
      kp = data.substring(2,7).toFloat();
      ki = data.substring(8,13).toFloat();
      kd = data.substring(14,18).toFloat();
      pid.atualizarPID(kp,ki,kd);
      Serial.println("kp: "+(String)kp+" | ki: "+(String)ki+" | kd: "+(String)kd);
    }
  }

}