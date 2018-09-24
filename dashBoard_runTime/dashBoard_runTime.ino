int led = 12;
int button = 2;
volatile byte state = LOW ;
int count;

void setup() {
  count = 0;
  attachInterrupt(digitalPinToInterrupt(button), counT, RISING);
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(button, INPUT);

}

void counT()
{
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 200)
  {
    //Do ya Thing
    count++;
    if (count > 5) {
      count = 0;
    }
  }
  last_interrupt_time = interrupt_time;
}






void ledBlink( int speed) {
  digitalWrite(led, HIGH);
  delay(speed / 2);
  digitalWrite(led, LOW);
  delay(speed / 2);
  digitalWrite(led, HIGH);
  delay(speed / 2);
  digitalWrite(led, LOW);
  delay(speed * 2);
}

void getClock(){
  Serial.println("UHRZEIT");
  ledBlink(1000);
}

  void getRPM(){
  Serial.println("RPM");
  ledBlink(800);
}

void getOilTemp(){
  Serial.println("ÖLTEMP");
  ledBlink(600);
}
void getSpeed(){
  Serial.println("Speed");
  ledBlink(400);
}
void getDirection(){
  Serial.println("Richtung");
  ledBlink(200);
}
void getGPS(){
  Serial.println("GPS - Location");
  ledBlink(100);
}


  void showDisp(){
  if(count == 0){
  getClock();
}
  else if(count == 1){
  getRPM();
}
  else if(count == 2){
  getOilTemp();
}
  else if(count == 3){
  getSpeed();
}
  else if(count == 4){
  getDirection();
}
  else if(count == 5){
  getGPS();
}
}

  void loop() {
  showDisp();

}
