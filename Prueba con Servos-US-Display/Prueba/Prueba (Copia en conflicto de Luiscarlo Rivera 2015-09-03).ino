#include <LiquidCrystal.h>
#include <Servo.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

Servo myservo1;
Servo myservo0;

long distancia;
long tiempo;

void setup(){

  Serial.begin(9600);
  pinMode(7, OUTPUT); /*Activación del pin 7 como salida: para el pulso ultrasónico*/
  //pinMode(9,OUTPUT); /*Activación del pint 9 como contraste del display*/
  pinMode(8, INPUT); /*Activación del pin 8 como entrada: tiempo del rebote del ultrasonido*/
  //analogWrite(9,50);
  lcd.begin(16,1);
  myservo0.attach(10);
  myservo1.attach(6);
  
}

void loop(){

  myservo0.write(-180);
  myservo1.write(180);
  
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
  
  distancia= int(0.017*tiempo); /*fórmula para calcular la distancia obteniendo un valor entero*/
  
  /*Monitorización en centímetros por el monitor serial*/
  Serial.print("Distancia ");
  Serial.print(distancia);
  Serial.print(" cm");

  /* Imprimir en LCD la distancia medida */
  lcd.setCursor(0,0);
  lcd.print(distancia);
  lcd.print(" cm");
  delay(1000);
  lcd.clear();
}
