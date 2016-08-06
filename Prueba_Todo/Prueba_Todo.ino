#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <QTRSensors.h>

#define NUM_SENSORS   1     // number of sensors used
#define TIMEOUT       2500  // waits for 2500 microseconds for sensor outputs to go low
#define EMITTER_PIN   2     // emitter is controlled by digital pin 2
#define SIERRA        4     //Pin digital encargado de prender y apagar la cierra
#define TACOMETRO     3     //Pin SIGNAL del tacometro
#define LEDVERDE      A0
#define LEDROJO       A1

//Pines en el PCF8574(dir,en,rw,rs,d4,d5,d6,d7,bl, blpol)
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

Servo myservo1;
Servo myservo0;
bool aux;
bool bqtr,bus,res;
float distancia;
long tiempo;

// Sensor QTR-1RC al pin 9
QTRSensorsRC qtrrc((unsigned char[]) {9}, NUM_SENSORS, TIMEOUT, EMITTER_PIN); 
unsigned int sensorValues[NUM_SENSORS];
float i=0,j;
float pi=3.141592;
volatile float dist=0;
volatile bool visto=false;
volatile bool hold;
volatile bool ais=false;

void taco(){
  res=true;
  if (digitalRead(TACOMETRO)==1 && !visto && !hold){
    visto=true;
    dist=dist+(pi/2.0)*1.34543017;//****** ;// *****1.35543017  1.30543017
  }else if(digitalRead(TACOMETRO)==0){
    visto = false;
  }
  if( ((52<=dist && dist<=63) || (209<=dist && dist<=220)) &&!ais){
    j=dist-2.5;
    if(dist>=200 && j<209){
      res=false;
    }
    if(res){
    dist+= -2.5;
    ais=true;
    }
  }else if((dist>63 && 209>dist) || dist>220){
    ais=false;
  }
  }
void setup(){
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(TACOMETRO),taco,CHANGE);
  pinMode(SIERRA,OUTPUT);
  pinMode(7, OUTPUT); /*Activación del pin 7 como salida: para el pulso ultrasónico (Trigger)*/
  pinMode(8, INPUT); /*Activación del pin 8 como entrada: tiempo del rebote del ultrasonido (Echo)*/
  pinMode(LEDROJO, OUTPUT); // Led Rojo
  pinMode(LEDVERDE, OUTPUT); // Led Verde  
  lcd.begin(16,2);
  myservo0.attach(10);
  myservo1.attach(6);
  bqtr = false;
  hold = false;
  aux = true;
  bus=false;
}

void loop(){
  if(dist>63){
  digitalWrite(SIERRA,HIGH);
  }
  //if(ais){
  //digitalWrite(SIERRA,LOW);
  //}
  lcd.setCursor(0,1);
  lcd.clear();
  //lcd.print(sensorValues[0]);
  
  //Serial.print(dist);
  myservo0.write(-180);
  myservo1.write(180);
  hold = true;
  //qtrrc.read(sensorValues);
  //lcd.setCursor(0,0);
  //lcd.print(sensorValues[0]); 
  /*if(bqtr == false && sensorValues[0]<=300){
    digitalWrite(LEDROJO, HIGH);   // set the LED Red on
    bqtr = true;
    myservo0.detach();
    myservo1.detach();
    lcd.clear();
    lcd.print("Pto C:");
    lcd.print(dist);
    lcd.print(" cm");
    lcd.setCursor(0,1);
    lcd.print(sensorValues[0]);
    delay(10000);
    lcd.clear();
    digitalWrite(LEDROJO, LOW);    // set the LED off
    myservo0.attach(10);
    myservo1.attach(6);
    myservo0.write(-180);
    myservo1.write(180);
    delay(1000);
  }
  else if(bqtr == true && sensorValues[0]>300){
    bqtr = false;
  }*/
  hold = false;
  digitalWrite(7,LOW); /*Por cuestión de estabilización del sensor*/
  delayMicroseconds(5);
  digitalWrite(7, HIGH); /*Envío del pulso ultrasónico*/
  delayMicroseconds(10);
  Serial.println("Lanza ondas");
  tiempo = pulseIn(8, HIGH); /*Función para medir la longitud del pulso entrante. 
                               Mide el tiempo que transcurrido entre el envío
                               del pulso ultrasónico y cuando el sensor recibe el rebote, 
                               es decir: desde que el pin 12 empieza a
                               recibir el rebote, HIGH, hasta que deja de hacerlo, LOW, 
                               la longitud del pulso entrante*/
  distancia= tiempo/58.0; /*fórmula para calcular la distancia obteniendo un valor entero*/

  // Vegetacion en el piso
  if(20<=distancia && distancia<=40){
    digitalWrite(LEDVERDE, HIGH);  // set the LED Green on
    bus=true;
    myservo0.detach();
    myservo1.detach();
    lcd.clear();
    lcd.print("V:");
    i=dist+3.0;
    lcd.print(i);
    lcd.print("cm");
    delay(10000);
    lcd.clear();
    digitalWrite(LEDVERDE, LOW);  // set the LED off
    myservo0.attach(10);
    myservo1.attach(6);
    myservo0.write(-180);
    myservo1.write(180);
    delay(1500);
}
}
