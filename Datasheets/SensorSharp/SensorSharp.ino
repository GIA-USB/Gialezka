// Pines de lectura
int ir_sensor0 = A0;
 
void setup() {
    //initialize serial communications at 9600 bps
    Serial.begin(9600);
}
 
void loop() {
    int lectura, cm;
    lectura = analogRead(ir_sensor0); // lectura del sensor 0
    cm = pow(3027.4 / lectura, 1.2134); // conversión a centímetros
    Serial.print("Sensor 0: ");
    Serial.print(cm); // lectura del sensor 0
    Serial.print("\n");
    delay(500); // tiempo de espera
 
}
