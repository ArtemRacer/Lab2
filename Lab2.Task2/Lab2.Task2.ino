#include <SPI.h>

enum { REG = 8 };


/* Тепер надсилаємо по 16 біт. Важливий момент: так як за замовчуванням
 * дані передаються, починаючи зі старшого біта, спочатку потрібно
 * відправити старший байт, потім - молодший - тоді всі 16 біт
 * передадуться в правильному порядку.
 */
void writeShiftRegister16(int ss_pin, uint16_t value)
{
  digitalWrite(ss_pin, LOW);
  /* Фокус ось у чому: спочатку надсилаємо старший байт */
  SPI.transfer(highByte(value));
  /* А потім молодший */
  SPI.transfer(lowByte(value));
  digitalWrite(ss_pin, HIGH);
}


void setup()
{
  SPI.begin();

  pinMode(REG, OUTPUT);
  writeShiftRegister16(REG, 0);
}


/* Злегка змінимо функцію для роботи з 16-бітними значеннями */
void rotateLeft(uint16_t &bits)
{
  uint16_t high_bit = bits & (1 << 15) ? 1 : 0;
  bits = (bits << 1) | high_bit;
}


void loop()
{
  static uint16_t nomad = 1;

  writeShiftRegister16(REG, nomad);
  rotateLeft(nomad);

  delay(1000 / 8);
}
