#define ECHOPIN 2                            // Pin to receive echo pulse
#define TRIGPIN 3                            // Pin to send trigger pulse

void setup(){
  Serial.begin(9600);
  pinMode(ECHOPIN, INPUT);
  pinMode(TRIGPIN, OUTPUT);
}

void loop(){
  digitalWrite(TRIGPIN, LOW);                   // Set the trigger pin to low for 2uS
  delay(2);
  digitalWrite(TRIGPIN, HIGH);                  // Send a 10uS high to trigger ranging
  delay(10);
  digitalWrite(TRIGPIN, LOW);                   // Send pin low again
  int distance = pulseIn(ECHOPIN, HIGH);        // Read in times pulse
  distance= distance/58;                        // Calculate distance from time of pulse
  Serial.println(distance);                     
  delay(50);                                    // Wait 50mS before next ranging
}
