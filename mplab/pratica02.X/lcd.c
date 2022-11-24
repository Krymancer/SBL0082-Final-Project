#include "lcd.h"

void envia_nibble_lcd(char dado)
{
  lcd_enable = 1;
  lcd_db4 = dado & 0x01;
  lcd_db5 = (dado >> 1) & 0x01;
  lcd_db6 = (dado >> 2) & 0x01;
  lcd_db7 = (dado >> 3) & 0x01;

  __delay_us(50);
  lcd_enable = 0;
  __delay_us(50);
}

void envia_byte_lcd(char endereco, char dado)
{
  lcd_rs = endereco;
  __delay_us(5);
  envia_nibble_lcd(dado >> 4);
  envia_nibble_lcd(dado & 0x0f);
}

inline void escreve_lcd(char c) { envia_byte_lcd(1, c); };

void limpa_lcd(void)
{
  envia_byte_lcd(0, 0x01);
  __delay_ms(2);
}

void inicializa_lcd(void)
{
  lcd_enable = 0;
  lcd_db4 = 0;
  lcd_db5 = 0;
  lcd_db6 = 0;
  lcd_db7 = 0;

  __delay_ms(50);
  envia_nibble_lcd(0x03);
  __delay_ms(4);
  envia_nibble_lcd(0x03);
  __delay_us(100);
  envia_nibble_lcd(0x03);

  envia_nibble_lcd(0x02);

  envia_byte_lcd(0, 0x01);
  __delay_ms(2);

  envia_byte_lcd(0, 0x28);

  envia_byte_lcd(0, 0x0c);

  envia_byte_lcd(0, 0x06);
}

void caracter_inicio(char linha, char coluna)
{
  char posicao = 0;
  if (linha == 1)
  {
    posicao = 0x80;
  }
  if (linha == 2)
  {
    posicao = 0xc0;
  }

  posicao = posicao + coluna;

  envia_byte_lcd(0, posicao);
}
