
#include <Adafruit_SSD1306.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#define OLED_ADDR 0x3c


int led = 12;
int button = 2;
volatile byte state = LOW ;
int count;

static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;

SoftwareSerial ss(RXPin, TXPin);
TinyGPSPlus gps;
Adafruit_SSD1306 display(4);

void setup() {
  count = 0;
  ss.begin(GPSBaud);
    Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(button), counT, RISING);

  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.setTextColor(WHITE);
 
}


// counT zählt die knopfdrücke und schaltet die entsprechenden Displays durch
void counT() {
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 200) {
    //Do ya Thing
    count++;
    if (count > 4) {
      count = 0;
    }
  }
  last_interrupt_time = interrupt_time;
}









//Getter und Setter, benutzen teilweise weitere Funktionen
void getClock() {
  display.setCursor(7,30);
  display.setTextSize(4);
  
  if (gps.time.hour() < 10) display.print(F("0"));
  display.print(gps.time.hour() + 2);
  display.print(F(":"));
  if (gps.time.minute() < 10) display.print(F("0"));
  display.println(gps.time.minute());
  display.display();
  display.clearDisplay();
      if(ss.available()){
    Serial.write(ss.read());
  }
  
}

void getRPM() {
  display.setTextSize(2);
  display.setCursor(45, 0);
  display.println("RPM");
  display.setTextSize(4);
  display.setCursor(20, 30);
  display.println("3200");
  display.display();
  display.clearDisplay();

}

void getOilTemp() {

  display.setTextSize(2);
  display.setCursor(50, 0);
  display.println("Oil");
  display.setTextSize(4);
  display.setCursor(20, 30);
  display.print("82");
  display.setCursor(65, 25);
  display.setTextSize(3);
  display.print(char(9));
  display.setTextSize(4);
  display.setCursor(84, 30);
  display.println("C");
  display.display();
  display.clearDisplay();
}
void getSpeed() {
  display.setTextSize(2);
  display.setCursor(42, 0);
  display.println("Speed");
  display.setTextSize(4);
  display.setCursor(10, 30);
  display.print("312");
  display.setTextSize(2);
  display.setCursor(80, 40);
  display.println( "Km/h");
  display.display();
  display.clearDisplay();
}
void getDirection() {
  display.setTextSize(2);
  display.setCursor(45, 0);
  display.println("Richtung");
  display.setTextSize(4);
  display.setCursor(20, 30);
  display.println("Pfeil");

}
void getGPS() {
  display.setTextSize(2);
  display.setCursor(45, 0);
  display.println("COORD");
  display.setTextSize(3);
  display.setCursor(20, 20);
  display.println("Hier sind");
  display.println("Koordinaten");
  display.display();
  display.clearDisplay();
}
void getAirTemp() {
    
  display.setTextSize(2);
  display.setCursor(25, 0);
    display.print("Num Sats");
    display.setCursor(40,30);
    display.setTextSize(4);
    display.println(gps.satellites.value());
    display.display();
    display.clearDisplay();
  
}

void printBar(int state) {
  int posX = 3;
  int posY = 60;
  int rectWidth = 24;
  int rectHeight = 4;
  int space = 25;
  if (state == 0 ) {
    display.fillRect(posX, posY, rectWidth, rectHeight, WHITE);
    display.drawRect(posX + space * 1, posY, rectWidth, rectHeight, WHITE);
    display.drawRect(posX + space * 2, posY, rectWidth, rectHeight, WHITE);
    display.drawRect(posX + space * 3, posY, rectWidth, rectHeight, WHITE);
    display.drawRect(posX + space * 4, posY, rectWidth, rectHeight, WHITE);
  }
  if (state == 1 ) {
    display.drawRect(posX, posY, rectWidth, rectHeight, WHITE);
    display.fillRect(posX + space * 1, posY, rectWidth, rectHeight, WHITE);
    display.drawRect(posX + space * 2, posY, rectWidth, rectHeight, WHITE);
    display.drawRect(posX + space * 3, posY, rectWidth, rectHeight, WHITE);
    display.drawRect(posX + space * 4, posY, rectWidth, rectHeight, WHITE);
  }
  if (state == 2 ) {
    display.drawRect(posX, posY, rectWidth, rectHeight, WHITE);
    display.drawRect(posX + space * 1, posY, rectWidth, rectHeight, WHITE);
    display.fillRect(posX + space * 2, posY, rectWidth, rectHeight, WHITE);
    display.drawRect(posX + space * 3, posY, rectWidth, rectHeight, WHITE);
    display.drawRect(posX + space * 4, posY, rectWidth, rectHeight, WHITE);
  }
  if (state == 3 ) {
    display.drawRect(posX, posY, rectWidth, rectHeight, WHITE);
    display.drawRect(posX + space * 1, posY, rectWidth, rectHeight, WHITE);
    display.drawRect(posX + space * 2, posY, rectWidth, rectHeight, WHITE);
    display.fillRect(posX + space * 3, posY, rectWidth, rectHeight, WHITE);
    display.drawRect(posX + space * 4, posY, rectWidth, rectHeight, WHITE);
  }
  if (state == 4 ) {
    display.drawRect(posX, posY, rectWidth, rectHeight, WHITE);
    display.drawRect(posX + space * 1, posY, rectWidth, rectHeight, WHITE);
    display.drawRect(posX + space * 2, posY, rectWidth, rectHeight, WHITE);
    display.drawRect(posX + space * 3, posY, rectWidth, rectHeight, WHITE);
    display.fillRect(posX + space * 4, posY, rectWidth, rectHeight, WHITE);
  }
}




void loop() {

  
  printBar(count);
  if (count == 0) {
    getClock();
  }
  else if (count == 1) {
    getAirTemp();
  }
  else if (count == 2) {
    getRPM();
  }
  else if (count == 3) {
    getOilTemp();
  }
  else if (count == 4) {
    getSpeed();
  }
  //delay(1000);
  
  
 
}
