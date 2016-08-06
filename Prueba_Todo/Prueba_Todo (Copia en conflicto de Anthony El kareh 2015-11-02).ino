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
bool bqtr;
bool boolus; 
long distancia;
long tiempo;

// Sensor QTR-1RC al pin 9
QTRSensorsRC qtrrc((unsigned char[]) {9}, NUM_SENSORS, TIMEOUT, EMITTER_PIN); 
unsigned int sensorValues[NUM_SENSORS];
static int i=0;
float pi=3.1415;
volatile float dist=0;
volatile bool visto=false;
volatile bool hold=false;

void taco(){
  if (digitalRead(TACOMETRO)==1 && !visto && !hold){
    visto=true;
    dist = dist + (pi/2)*1.3;
  }else if(digitalRead(TACOMETRO)==0){
    visto = false;
  }
  }
  
void setup(){
  //Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(TACOMETRO),taco,CHANGE);
  pinMode(SIERRA,OUTPUT);
  pinMode(7, OUTPUT); /*Activación del pin 7 como salida: para el pulso ultrasónico (Trigger)*/
  pinMode(8, INPUT); /*Activación del pin 8 como entrada: tiempo del rebote del ultrasonido (Echo)*/
  pinMode(LEDROJO, OUTPUT); // Led Rojo
  pinMode(LEDVERDE, OUTPUT); // Led Verde  
  lcd.begin(16,2);
  myservo0.attach(10);
  myservo1.attach(6);
  bqtr=false;
  boolus =false;
  }

void loop(){

  digitalWrite(SIERRA,HIGH);
  
  lcd.setCursor(0,1);
  lcd.print(distancia);
  
  myservo0.write(-180);
  myservo1.write(180);
  hold = true;
  qtrrc.read(sensorValues);
  //Serial.println(sensorValues[0]); 
  if(bqtr == false && sensorValues[0]<=300){
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
    myservo0.attach(10);
    myservo1.attach(6);
    myservo0.write(-180);
    myservo1.write(180);
    digitalWrite(LEDROJO, LOW);    // set the LED off
  }
  else if(bqtr == true && sensorValues[0]>300){
    bqtr = false;
  }
  hold = false;
  digitalWrite(7,LOW); /*Por cuestión de estabilización del sensor*/
  delayMicroseconds(5);                                                                             //Acabo de comentar esto
  digitalWrite(7, HIGH); /*Envío del pulso ultrasónico*/
  delayMicroseconds(10);                                                                            //Acabo de comentar esto
  //  Serial.println("Lanza ondas");
  tiempo = pulseIn(8, HIGH); /*Función para medir la longitud del pulso entrante. 
                               Mide el tiempo que transcurrido entre el envío
                               del pulso ultrasónico y cuando el sensor recibe el rebote, 
                               es decir: desde que el pin 12 empieza a
                               recibir el rebote, HIGH, hasta que deja de hacerlo, LOW, 
                               la longitud del pulso entrante*/
  //Serial.println("Fuck u");
  distancia= int(tiempo/58); /*fórmula para calcular la distancia obteniendo un valor entero*/

  // Vegetacion en el piso
  if(20<=distancia && distancia<=40 && !boolus){
    boolus = true;
    digitalWrite(LEDVERDE, HIGH);   // set the LED Green on
    myservo0.detach();
    myservo1.detach();
    lcd.clear();
    lcd.print("V:");
    lcd.print(distancia);
    lcd.print("cm");
    delay(10000);
    lcd.clear();
    myservo0.attach(10);
    myservo1.attach(6);
    myservo0.write(-180);
    myservo1.write(180);
    digitalWrite(LEDVERDE, LOW);    // set the LED off
  }
  else if(distancia > 40 && boolus){
    boolus = false;
  }
    
}
