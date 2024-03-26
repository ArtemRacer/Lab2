#include <SPI.h>

// Визначення пінів для регістрів зсуву
const int latchPin = 8; // ST_CP 74HC595
const int clockPin = 13; // SH_CP 74HC595
const int dataPin = 11; // DS 74HC595

const int loadPin = 10; // PL 74HC165
const int enablePin = 9; // CE 74HC165

void setup() {
  // Налаштування пінів
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(loadPin, OUTPUT);
  pinMode(enablePin, OUTPUT);

  // Початкове вимкнення регістру зсуву 74HC165
  digitalWrite(enablePin, HIGH);

  // Налаштування SPI
  SPI.begin();
}

void loop() {
  // Читання стану кнопок
  digitalWrite(loadPin, LOW);
  delayMicroseconds(5);
  digitalWrite(loadPin, HIGH);
  digitalWrite(enablePin, LOW);

  byte buttons = SPI.transfer(0);

  digitalWrite(enablePin, HIGH);

  // Засвічування світлодіодів
  digitalWrite(latchPin, LOW);
  SPI.transfer(buttons);
  digitalWrite(latchPin, HIGH);

  // Затримка перед наступним оновленням
  delay(100);
}
