//Ich habe etwas geändert..

#include <DallasTemperature.h>
#include <OneWire.h>
#include <SPI.h>                      //Serial Peripheral Interface (SPI) library for synchronous serial data protocol
#include <Wire.h>                     //Wire library used for I2C communication: Arduino Pro Mini pins used = A4 (SDA) and A5 (SCL)

#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"
#include <SoftwareSerial.h>           //SoftwareSerial library used to allow serial communication on other digital pins (i.e. Pins 3 & 4 for the this GPS project)
#include <TinyGPS++.h>

#define SerialMonitor Serial
#define ONE_WIRE_BUS 5
int button = 2;
volatile byte state = LOW ;
int count;
int progs;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature temp(&oneWire);
SoftwareSerial ss(4, 3);
TinyGPSPlus gps;
SSD1306AsciiAvrI2c display;

void setup() {
  progs = 4;

  ss.begin(9600);
  Serial.begin(9600);
  temp.begin();

  attachInterrupt(digitalPinToInterrupt(button), counT, RISING);
  display.begin(&Adafruit128x64, 0x3C);
  display.setFont(fixed_bold10x15);

  display.clear();
}


void counT() {
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 200) {
    //Do ya Thing
    count++;
    display.clear();
    if (count > progs - 1) {
      count = 0;
    }
  }
  last_interrupt_time = interrupt_time;
}


static void smartDelay(unsigned long ms) {
  unsigned long start = millis();
  do
  {

    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}
void printRPM() {
  display.set1X();
  display.setCursor(40, 0);
  display.println("RPM");
  display.set2X();
  display.setCursor(15, 4);
  display.print("3500");
}

void printTime() {

  display.setCursor(10, 3);
  display.set2X();
  if (!gps.time.isValid()) {
    display.print("No GPS");
    display.clear();
  }
  else {
    if (gps.time.hour() < 10) display.print('0');
    display.print(gps.time.hour() + 1);
    display.print(":");
    if (gps.time.minute() < 10) display.print('0');
    display.print(gps.time.minute());
  }

}

void printSpeed() {
  int speed;


  display.set2X();
  if (!gps.speed.isValid()) {
    display.print("No GPS");
    display.clear();
  }
  speed = gps.speed.kmph();

  display.set1X();
  display.setCursor(35, 0);
  display.println("Speed");
  display.set2X();
  display.setCursor(15, 4);

  if (speed < 10)
    display.print(" ");
  if (speed < 100)
    display.print(" ");
  display.print(speed);
  display.set1X();
  display.print("Km/h");

}


void printOilTemp() {

  temp.requestTemperatures();
  int tempC = (int) temp.getTempCByIndex(0);
  display.set1X();
  display.setCursor(40, 0);
  display.println("Oil");
  display.set2X();
  display.setCursor(15, 4);
  display.print(tempC);
  display.set1X();5
  display.print("*");
  display.println("C");

}



void loop() {
  /*display.set1X();
    display.setCursor(118,0);
    display.print(gps.satellites.value());
  */
  if (count == 0)
    printTime();

  if (count == 1)
    printSpeed();


  if (count == 2)
    printRPM();

  if (count == 3)
    printOilTemp();

  smartDelay(1000);

}
