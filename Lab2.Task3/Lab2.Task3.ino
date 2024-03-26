#include <SPI.h>

enum { REG_LATCH = 8 };


void setup()
{
  /* Вмикаємо UART і SPI */
  Serial.begin(9600);
  SPI.begin();
  /* Вмикаємо засувку */
  pinMode(REG_LATCH, OUTPUT);
  digitalWrite(REG_LATCH, HIGH);
}


void loop()
{
  static uint8_t last_input_states = 0;

  /* Виставимо на засувці спочатку низький, потім - високий рівні.
   * Зсувний регістр запам'ятає рівні сигналів на входах і зможе
   * їх нам потім віддати біт за бітом.
   */
  digitalWrite(REG_LATCH, LOW);
  digitalWrite(REG_LATCH, HIGH);
  /* Читаєм запам’ятовані стани входів. Нуль надсилаємо тільки тому,
   * що transfer() одночасно і надсилає, і приймає. Так до речі,
   * MOSI не подключена
   */
  uint8_t states = SPI.transfer(0);

  /* Якщо стан будь-якого входу змінився, розповімо по UART */
  if (states != last_input_states)
  {
    /* Старий добрий XOR дасть нам бітову різницю між старим і новим станами. */
    uint8_t changed = states ^ last_input_states;
    last_input_states = states; // запам'ятовуємо поточний стан

    for (int i = 0; i < 8; ++i)
    {
      if (changed & 1) // якщо стан кнопки змінився …
      {
        Serial.print("#");
        Serial.print(i); // надсилаєм номер кнопки
        Serial.print(" -> ");
        Serial.println(states & 1); // … і новий стан
      }

      /* Зсуваємо біти вправо. Тепер перший біт
       * буде вказувати стан наступної кнопки.
       */
      changed >>= 1;
      states >>= 1;
    }
  }
}
