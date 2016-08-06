//Librerias
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <QTRSensors.h>

//Inicializaciones
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
bool aislantes;
bool salida;
long distancia;
long tiempo;

// Sensor QTR-1RC al pin 9
QTRSensorsRC qtrrc((unsigned char[]) {9}, NUM_SENSORS, TIMEOUT, EMITTER_PIN); 
unsigned int sensorValues[NUM_SENSORS];
float pi=3.141592;
volatile float dist=0;
volatile bool visto=false;
volatile bool hold;

//Funcion de la interrupcion
void taco(){
  if (digitalRead(TACOMETRO)==1 && !visto && !hold){
    visto=true;
    dist=dist+(pi/2.0)*1.30543017;
  }
  else if(digitalRead(TACOMETRO)==0){
    visto = false;
  }
}
void setup(){
  //Serial.begin(9600);
  //Habilitamos la interrupcion
  attachInterrupt(digitalPinToInterrupt(TACOMETRO),taco,CHANGE);
  //Seteamos los pines
  pinMode(SIERRA,OUTPUT);
  pinMode(7, OUTPUT); /*Activación del pin 7 como salida: para el pulso ultrasónico (Trigger)*/
  pinMode(8, INPUT); /*Activación del pin 8 como entrada: tiempo del rebote del ultrasonido (Echo)*/
  pinMode(LEDROJO, OUTPUT); // Led Rojo
  pinMode(LEDVERDE, OUTPUT); // Led Verde  
  //Habilitamos la pantalla
  lcd.begin(16,2);
  //Seteamos los servos
  myservo0.attach(10);
  myservo1.attach(6);
  //Inicializamos los booleanos
  bqtr = false;
  aislantes = true;
  hold = false;
  salida = true;
}

void loop(){
  //Definimos si estamos sobre los aislantes
  //Se enciende la sierra en caso de no estar sobre los aislantes
  if ((dist >= 44 && dist <= 57) || (dist >= 195 && dist <= 207) || (dist >= 346 && dist <= 358)){
    digitalWrite(SIERRA,LOW);
    aislantes = true;
  }
  else if(dist > 57 || (dist < 195 && dist > 207) || (dist < 346 && dist > 358)){
    digitalWrite(SIERRA,HIGH);
    aislantes = false;
  }
  //Nos ubicamos en la fila 1 columna 0 del display
  lcd.setCursor(0,1);
  //Imprimimos la distancia RECORDAR ES TEMPORAL!!!! 
  lcd.print(dist);
  //ES TEMPORAL!!!
  //Prendemos los motores
  myservo0.write(-180);
  myservo1.write(180);
  //Inhabilitamos las interrupciones 
  hold = true;
  //Realizamos la lectura de puntos calientes
  qtrrc.read(sensorValues);
  //Realizamos la inspeccion de puntos calientes
  if(bqtr == false && sensorValues[0]<=300 && !aislantes && !salida){
    //Si estamos en el ultimo tramo apagamos el robot
    if (dist > 420){
      exit(0);
    }
    //Se 
    digitalWrite(LEDROJO, HIGH);   // set the LED Red on
    bqtr = true;
    myservo0.detach();
    myservo1.detach();
    lcd.clear();
    lcd.print("Pto C:");
    lcd.print(dist);
    lcd.print(" cm");
    lcd.setCursor(0,1);
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
  salida = false;
  hold = false;
  digitalWrite(7,LOW); /*Por cuestión de estabilización del sensor*/
  delayMicroseconds(5);
  digitalWrite(7, HIGH); /*Envío del pulso ultrasónico*/
  delayMicroseconds(10);
  tiempo = pulseIn(8, HIGH); /*Función para medir la longitud del pulso entrante. 
                               Mide el tiempo que transcurrido entre el envío
                               del pulso ultrasónico y cuando el sensor recibe el rebote, 
                               es decir: desde que el pin 12 empieza a
                               recibir el rebote, HIGH, hasta que deja de hacerlo, LOW, 
                               la longitud del pulso entrante*/
  distancia= int(tiempo/58); /*fórmula para calcular la distancia obteniendo un valor entero*/

  // Vegetacion en el piso
  if(20<=distancia && distancia<=40){
    digitalWrite(LEDVERDE, HIGH);   // set the LED Green on
    myservo0.detach();
    myservo1.detach();
    lcd.clear();
    lcd.print("V:");
    lcd.print(dist);
    lcd.print("cm");
    delay(10000);
    lcd.clear();
    myservo0.attach(10);
    myservo1.attach(6);
    myservo0.write(-180);
    myservo1.write(180);
    delay(1000);
    digitalWrite(LEDVERDE, LOW);    // set the LED off
  }
  
}
