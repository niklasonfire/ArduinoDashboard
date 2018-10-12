#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 display(4);

#include "Wire.h"

#define DS3231_ADRESSE 0x68
#define OLED_ADDR 0x3c


int led = 12;
int button = 2;
volatile byte state = LOW ;
int count;
String zeit;
void setup() {
  count = 0;
  Wire.begin();
  attachInterrupt(digitalPinToInterrupt(button), counT, RISING);
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(4);



}
// counT zählt die knopfdrücke und schaltet die entsprechenden Displays durch
void counT()
{
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 200)
  {
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
void zeigeZeit() {
  byte sekunde, minute, stunde;
  leseDS3231zeit(&sekunde, &minute, &stunde);   // Daten vom DS3231 holen
  display.setTextSize(4);
  display.setCursor(7, 24);
  if (stunde < 10) {
    zeit = "0";
  }
  zeit.concat(stunde);
  zeit.concat(":");
  if (minute < 10) {
    zeit.concat("0");

    //u8g2.draw2x2String(0,0,"0");
  }


  zeit.concat(minute);
  display.println(zeit);
  display.display();
  display.clearDisplay();

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
  zeigeZeit();

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
  display.setCursor(45, 0);
  display.println("Air");
  display.setTextSize(4);
  display.setCursor(15, 30);
  display.print(tempDS3231());
  display.setCursor(65, 25);
  display.setTextSize(3);
  display.print(char(9));
  display.setTextSize(4);
  display.setCursor(84, 30);
  display.println("C");
  display.display();
  display.clearDisplay();
}







void showDisp() {
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

  //  else if (count == 5) {
  //  getDirection();
  //}
  //  else if (count == 6) {
  //    getGPS();
  //  }

}

void loop() {


  showDisp();
  delay(1000);
}
