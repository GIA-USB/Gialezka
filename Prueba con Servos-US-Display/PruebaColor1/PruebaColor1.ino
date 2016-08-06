#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <QTRSensors.h>
 


#define NUM_SENSORS   1     // number of sensors used
#define TIMEOUT       2500  // waits for 2500 microseconds for sensor outputs to go low
#define EMITTER_PIN   2     // emitter is controlled by digital pin 2
 
//Pines en el PCF8574(dir, en,rw,rs,d4,d5,d6,d7,bl, blpol)

/*
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

Servo myservo1;
Servo myservo0;

long distancia;
long tiempo;
*/
// sensors 0 through 7 are connected to digital pins 3 through 10, respectively
QTRSensorsRC qtrrc((unsigned char[]) {9},
  NUM_SENSORS, TIMEOUT, EMITTER_PIN); 
unsigned int sensorValues[NUM_SENSORS];

void setup(){

  Serial.begin(9600);
  
  pinMode(7, OUTPUT); /*Activación del pin 7 como salida: para el pulso ultrasónico*/
  pinMode(8, INPUT); /*Activación del pin 8 como entrada: tiempo del rebote del ultrasonido*/
  //lcd.begin(16,1);
  //myservo0.attach(10);
  //myservo1.attach(6);
  
}

void loop(){
  
  
  
  
   qtrrc.read(sensorValues);

  // print the sensor values as numbers from 0 to 2500, where 0 means maximum reflectance and
  // 1023 means minimum reflectance
    Serial.print("Color");
    Serial.print(sensorValues[0]);
    Serial.print('\t'); // tab to format the raw data into columns in the Serial monitor
 
  Serial.println();
  
  delay(250);
  
  
  /*
  myservo0.write(-180);
  myservo1.write(180);
  

  
  digitalWrite(7,LOW); /*Por cuestión de estabilización del sensor*/
 // delayMicroseconds(5);
  //digitalWrite(7, HIGH); /*Envío del pulso ultrasónico*/
  //delayMicroseconds(10);
  
  
     
 // tiempo = pulseIn(8, HIGH); 
  
  //distancia= int(tiempo/58); /*fórmula para calcular la distancia obteniendo un valor entero*/
  
  

  /*Monitorización en centímetros por el monitor serial
  Serial.print("Distancia ");
  Serial.print(distancia);
  Serial.print(" cm\n");
  */


 /*
  // Imprimir en LCD la distancia medida 
  lcd.setCursor(10,8);
  lcd.print(distancia);
  lcd.print(" cm");
 
  
  // Imprimir Color
  lcd.setCursor(0,0);
  lcd.print("C: ");
  lcd.print(sensorValues[0]);
  
  
  
  if(sensorValues[0]<=1000){
    
    myservo0.detach();
    myservo1.detach();
    delay(5000);
    myservo0.attach(10);
    myservo1.attach(6);
    myservo0.write(-180);
    myservo1.write(180);
    delay(100);
  }
  else{
    delay(100);
  }
  
  if(20<=distancia && distancia<=40){
    myservo0.detach();
    myservo1.detach();
    delay(10000);
    myservo0.attach(10);
    myservo1.attach(6);
    myservo0.write(-180);
    myservo1.write(180);
    delay(100);
  }
  else{
    delay(100);
  }
  
  
  lcd.clear();
  
   */
}
