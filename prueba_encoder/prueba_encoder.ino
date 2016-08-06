#include <Servo.h>

/*Encoder Omron 1230:

1) Cómo conectarlo:

	Viendo el aparato desde arriba: 
  el pin de la derecha es el "E" (emisor), es decir 5V; 
  el pin  del medio es Ground; 
  el pin de la izquierda es el "S" (receptor), 
  el cual es el pin que va dirigido a los pines analógicos del Arduino.

	El pin "Emisor" requiere una resistencia de 470 ohms
	El pin "S", recepto, requiere una resistencia de 10k ohms

2) Cómo funciona:

	El encoder va conectado al pin analógica A0 del Arduino UNO

	El código printea "0" (cero) si el IR LED del encoder no percibe "algo"... 
  En caso contrario, significa que el IR LED percibió "algo". 
  De esta forma, mostrará en el terminal un número que representa lo que vió.

	Por último, se tiene que esperar un lapso muy pequeño 
  (delay de 10 a 20 millis) para que vuelva a entrar al loop y leer.
*/

/*
  Notar que para reducir el error haya que quitar los booleanos o 
  ajustar el delay (por encima de 20 apesta y 10 es muy corto)
*/

Servo myservo;
int val;
float pi=3.1415,dist=0;
int sensorPin = 0; //analog pin 0
bool visto=false;
void setup(){
  Serial.begin(9600);
  myservo.attach(8);
}
void loop(){
  myservo.write(180);
  val = analogRead(sensorPin);
  if (val>13 && !visto){
    visto=true;
    dist = dist + (pi/2)*2.2;
    Serial.print(dist);
    Serial.println("cm");
  }else if (val<=13){
    visto = false;
  }
  delay(20);
}
