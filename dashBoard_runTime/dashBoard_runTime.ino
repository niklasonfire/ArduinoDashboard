

#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 display(4);

#include "Wire.h"

#define DS3231_ADRESSE 0x68
#define OLED_ADDR 0x3c


int led = 12;
int button = 2;
volatile byte state = LOW ;
int count;
byte time[2];

void setup() {
  count = 0;
  Wire.begin();
  attachInterrupt(digitalPinToInterrupt(button), counT, RISING);
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.setTextColor(WHITE);
}
// counT zählt die knopfdrücke und schaltet die entsprechenden Displays durch
void counT()
{
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

void leseDS3231zeit(byte *sekunde, byte *minute, byte *stunde) {
  Wire.beginTransmission(DS3231_ADRESSE);
  Wire.write(0); // DS3231 Register zu 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_ADRESSE, 7); // 7 Byte Daten vom DS3231 holen
  *sekunde = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *stunde = bcdToDec(Wire.read() & 0x3f);
}
void getZeit() {
  byte sekunde, minute, stunde;
  leseDS3231zeit(&sekunde, &minute, &stunde);   // Daten vom DS3231 holen
  time[0] = minute;
  time[1] = stunde;
}


byte decToBcd(byte val) {
  // Dezimal Zahl zu binary coded decimal (BCD) umwandeln
  return ((val / 10 * 16) + (val % 10));
}
byte bcdToDec(byte val) {
  // BCD (binary coded decimal) in Dezimal Zahl umwandeln
  return ((val / 16 * 10) + (val % 16));
}

int tempDS3231() {
  // Die Temperatur des DS3231 auslesen
  int temp;
  int msb, lsb;
  Wire.beginTransmission(DS3231_ADRESSE);
  Wire.write(0x11); // DS3231 Register zu 11h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_ADRESSE, 2); // 2 Byte Daten vom DS3231 holen
  msb = Wire.read();
  lsb = Wire.read();
  temp = ((msb << 2) + (lsb >> 6) ) / 4.0;
  return temp;
}




//Getter und Setter, benutzen teilweise weitere Funktionen
void getClock() {
  getZeit();
  String zeit;
  byte stunde, minute;
  display.setTextSize(4);
  display.setCursor(7, 30);
  if (minute != time[0]) {
    
    minute = time[0];
    stunde = time[1];
    if (stunde < 10) {
      zeit = "0";
    }
    zeit.concat(stunde);
    zeit.concat(":");
    if (minute < 10) {
      zeit.concat("0");
    }

    zeit.concat(minute);
    display.print(zeit);

    display.display();

    display.clearDisplay();
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
  int temp;
  if(temp != tempDS3231()){
    temp = tempDS3231();
  display.setTextSize(2);
  display.setCursor(45, 0);
  display.println("Air");
  display.setTextSize(4);
  display.setCursor(15, 30);
  display.print(temp);
  display.setCursor(65, 25);
  display.setTextSize(3);
  display.print(char(9));
  display.setTextSize(4);
  display.setCursor(84, 30);
  display.println("C");
  display.display();
  display.clearDisplay();
  }
}

void printBar(int state){
  int posX = 3;
  int posY = 60;
  int rectWidth = 24;
  int rectHeight = 4;
  int space = 25;
  if(state == 0 ){
  display.fillRect(posX,posY,rectWidth,rectHeight,WHITE);
  display.drawRect(posX+space*1,posY,rectWidth,rectHeight,WHITE);
  display.drawRect(posX+space*2,posY,rectWidth,rectHeight,WHITE);
  display.drawRect(posX+space*3,posY,rectWidth,rectHeight,WHITE);
  display.drawRect(posX+space*4,posY,rectWidth,rectHeight,WHITE);
  }
  if(state == 1 ){
  display.drawRect(posX,posY,rectWidth,rectHeight,WHITE);
  display.fillRect(posX+space*1,posY,rectWidth,rectHeight,WHITE);
  display.drawRect(posX+space*2,posY,rectWidth,rectHeight,WHITE);
  display.drawRect(posX+space*3,posY,rectWidth,rectHeight,WHITE);
  display.drawRect(posX+space*4,posY,rectWidth,rectHeight,WHITE);
  }
  if(state == 2 ){
  display.drawRect(posX,posY,rectWidth,rectHeight,WHITE);
  display.drawRect(posX+space*1,posY,rectWidth,rectHeight,WHITE);
  display.fillRect(posX+space*2,posY,rectWidth,rectHeight,WHITE);
  display.drawRect(posX+space*3,posY,rectWidth,rectHeight,WHITE);
  display.drawRect(posX+space*4,posY,rectWidth,rectHeight,WHITE);
  }
  if(state == 3 ){
  display.drawRect(posX,posY,rectWidth,rectHeight,WHITE);
  display.drawRect(posX+space*1,posY,rectWidth,rectHeight,WHITE);
  display.drawRect(posX+space*2,posY,rectWidth,rectHeight,WHITE);
  display.fillRect(posX+space*3,posY,rectWidth,rectHeight,WHITE);
  display.drawRect(posX+space*4,posY,rectWidth,rectHeight,WHITE);
  }
  if(state == 4 ){
  display.drawRect(posX,posY,rectWidth,rectHeight,WHITE);
  display.drawRect(posX+space*1,posY,rectWidth,rectHeight,WHITE);
  display.drawRect(posX+space*2,posY,rectWidth,rectHeight,WHITE);
  display.drawRect(posX+space*3,posY,rectWidth,rectHeight,WHITE);
  display.fillRect(posX+space*4,posY,rectWidth,rectHeight,WHITE);
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
