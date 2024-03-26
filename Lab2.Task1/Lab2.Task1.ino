#include <SPI.h>

enum { REG_SELECT = 8 }; // пін, керуючий засувкою (SS в термінах SPI)

void setup()
{
  /* Ініціалізуємо шину SPI. Якщо використовується програмна реалізація,
   * то ви повинні самі налаштувати піни, по якими буде працювати SPI.
   */
  SPI.begin();

  pinMode(REG_SELECT, OUTPUT);
  digitalWrite(REG_SELECT, LOW); // вибір підлеглого - нашого регістра
  SPI.transfer(0); // очищаємо вміст регістра
  /* Завершуємо передачу даних. Після цього регістр встановить
   * на виводах QA-QH рівні, відповідні записаним бітам.
   */
  digitalWrite(REG_SELECT, HIGH);
}


/* Ця функція зсуває біти вліво на одну позицію, переміщаючи старший біт
 * на місце молодшого. Іншими словами, вона "обертає" біти по колу.
 * Наприклад, 11110000 перетворюється в 11100001.
 */
void rotateLeft(uint8_t &bits)
{
  uint8_t high_bit = bits & (1 << 7) ? 1 : 0;
  bits = (bits << 1) | high_bit;
}


void loop()
{
  static uint8_t nomad = 1; // это наш бегающий бит

  /* Записуємо значення у зсувний регістр */
  digitalWrite(REG_SELECT, LOW);
  SPI.transfer(nomad);
  digitalWrite(REG_SELECT, HIGH);
  /* І обертаємо біти вліво - в наступний раз загориться інший світлодіод */
  rotateLeft(nomad);

  delay(1000 / 8); // пробігаємо всі 8 світлодіодів за 1 секунду
}
